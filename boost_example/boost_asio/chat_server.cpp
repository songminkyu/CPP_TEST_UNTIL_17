// boost_asio.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <memory>
#include <cstdlib>
#include <deque>
#include <list>
#include <set>
#include <utility>

using namespace std;

class chat_message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	chat_message()
		: body_length_(0)
	{
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	void body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(body_length_));
		std::memcpy(data_, header, header_length);
	}

private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;
};


using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{

public:

	chat_room() : _joinCnt(0)
	{}

	void join(chat_participant_ptr participant)
	{
		_joinCnt++;
		participants_.insert(participant);
		for (auto msg : recent_msgs_)
			participant->deliver(msg);
		std::cout << "현재 사용자는 : " << _joinCnt << " 명" << " 입니다" << std::endl;
	}

	void leave(chat_participant_ptr participant)
	{

		std::cout << _joinCnt << " 번째 사용자가 떠났습니다. \n";		
		_joinCnt--;
		std::cout << "현재 사용자는 : " << _joinCnt << " 명" << " 입니다";
		participants_.erase(participant);
	}

	void deliver(const chat_message& msg)
	{
		recent_msgs_.push_back(msg);
		while (recent_msgs_.size() > max_recent_msgs)
			recent_msgs_.pop_front();

		for (auto participant : participants_)
			participant->deliver(msg);
	}

private:
	std::set<chat_participant_ptr> participants_;
	enum { max_recent_msgs = 100 };
	chat_message_queue recent_msgs_;
	unsigned int _joinCnt;
};

//----------------------------------------------------------------------

class chat_session
	: public chat_participant,
	public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room)
		: socket_(std::move(socket)),
		room_(room)
	{
	}

	void start()
	{
		room_.join(shared_from_this());
		do_read_header();
	}

	void deliver(const chat_message& msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);

		std::cout.write(msg.body(), msg.length());
		std::cout << "\n";
		if (!write_in_progress)
		{
			do_write();
		}
	}

private:
	void do_read_header()
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), chat_message::header_length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && read_msg_.decode_header())
			{
				std::cout.write(read_msg_.data(), read_msg_.length());
				std::cout << "\n";
				do_read_body();
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	void do_read_body()
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				room_.deliver(read_msg_);
				do_read_header();
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	void do_write()
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_,
			boost::asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	tcp::socket socket_;
	chat_room& room_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
	chat_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: acceptor_(io_service, endpoint),
		socket_(io_service)
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<chat_session>(std::move(socket_), room_)->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
	chat_room room_;
};

//----------------------------------------------------------------------

void main()
{
	try
	{
		boost::asio::io_service io_service;

		std::list<chat_server> servers;
		for (int i = 0; i < 10; i++)
		{
			tcp::endpoint endpoint(tcp::v4(), std::atoi("8080"));
			servers.emplace_back(io_service, endpoint);
		}

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

}
