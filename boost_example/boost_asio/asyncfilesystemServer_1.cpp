#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>
//���� ��Ʈ�� 39800 ���� ����
unsigned short port = 39800;

//�������� ����� ����ϴ� tcpConnection Ŭ���� ����
class tcpConnection : public boost::enable_shared_from_this<tcpConnection>
{
	//�⺻���� �������� ����
private:
	boost::asio::streambuf requestBuf;
	boost::asio::ip::tcp::socket mySocket;
	boost::array<char, 40960> buf;
	size_t fileSize;
	std::ofstream outputFile;

	void handleReadRequest(const boost::system::error_code & err, std::size_t byteTransferred)
	{
		//������ �߻��� ���
		if (err)
		{
			return handleError(__FUNCTION__, err);
		}

		//������ �߻����� ���� ��� ���� �ٿ��� �⺻ ���� ���
		std::cout << __FUNCTION__ << "(" << byteTransferred << ")" \
			<< ", in_avail = " << requestBuf.in_avail() \
			<< ", size = " << requestBuf.size() \
			<< ", max_size = " << requestBuf.max_size() \
			<< ".\n";


		//���Ͽ� ���� ������ ���� ����
		std::istream requestStream(&requestBuf);
		std::string filePath;
		requestStream >> filePath;
		requestStream >> fileSize;

		//���� ���๮�� 2�� ����
		requestStream.read(buf.c_array(), 2);

		//���Ͽ� ���� ���� ���
		std::cout << filePath <<
			"size is" << fileSize <<
			",tellg = " << requestStream.tellg() << std::endl;

		//���� ����ǻ�Ϳ� ����� ������ ����.
		outputFile.open(filePath.c_str(), std::ios_base::binary);

		//��� ������ �������� �ʴ� ���
		if (!outputFile) {
			std::cout << "failed to open" << filePath << std::endl;
			return;
		}

		//������ ���� ����Ʈ�� �� ������
		do {

			requestStream.read(buf.c_array(), (std::streamsize)buf.size());
			std::cout << __FUNCTION__ << "write" << requestStream.gcount() << "byte. \n";
			outputFile.write(buf.c_array(), requestStream.gcount());

		} while (requestStream.gcount() > 0);

		//���ۿ� ������ �о� ��
		async_read(mySocket, boost::asio::buffer(buf.c_array(), buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent, shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	//������ ������ ���� ���
	void handleReadFileContent(const boost::system::error_code & err, std::size_t bytesTransferred)
	{
		if (bytesTransferred > 0) {
			outputFile.write(buf.c_array(), (std::streamsize) bytesTransferred);
			std::cout << __FUNCTION__ << "recv" << outputFile.tellp() << "bytes" << std::endl;
			if (outputFile.tellp() >= (std::streamsize) fileSize) {
				return;
			}
		}

		if (err) {
			return handleError(__FUNCTION__, err);

		}

		//���ۿ� ������ �о� ��
		async_read(mySocket, boost::asio::buffer(buf.c_array(), buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent, shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	
	//���� ó�� �Լ�
	void handleError(const std::string& functionName, const boost::system::error_code & err) {
		std::cout << __FUNCTION__ << "in " << functionName << "due to" << err << " " << err.message() << std::endl;
	}
public:
	tcpConnection(boost::asio::io_service & io_service)
		:mySocket(io_service), fileSize(0) 
	{
	}

	//���� �Լ�
	void start() 
	{
		std::cout << __FUNCTION__ << std::endl;
		async_read_until(mySocket, requestBuf, "\n\n",
			boost::bind(&tcpConnection::handleReadRequest,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	boost::asio::ip::tcp::socket& socket() {
		return mySocket;
	}

};

class tcpServer : private boost::noncopyable
{
// Ŭ���� ��� �ʿ��� �⺻ ���� ����
private:
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::acceptor acceptor;
public:
	//tcpConnection Ŭ������ ������ ���� ����
	using ptrTcpConnection = boost::shared_ptr<tcpConnection>;

	//���� ������
	tcpServer(unsigned short port) : acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), true)
	{
		//���� ����
		ptrTcpConnection newConnection(new tcpConnection(ioService));
		acceptor.async_accept(newConnection->socket(),
			boost::bind(&tcpServer::handleAccept, this, newConnection, boost::asio::placeholders::error));

		ioService.run();
	}

	void handleAccept(ptrTcpConnection currentConnection, const boost::system::error_code & err) {
		
		std::cout << __FUNCTION__ << " " << err << ", " << err.message() << std::endl;

		if (!err) {
			currentConnection->start();
		}
	}

	~tcpServer() {
		ioService.stop();
	}
};

int main(int argc, char* argv[]) {

	//���� ����

	try {
		
		if (argc == 2) 
		{
			port = atoi(argv[1]);
		}

		std::cout << argv[0] << " listen on port" << port << std::endl;

		tcpServer *myTcpServer = new tcpServer(port);

		delete myTcpServer;
		myTcpServer = nullptr;

	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}