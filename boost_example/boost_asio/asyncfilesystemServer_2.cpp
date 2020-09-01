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

//���� ��Ʈ�� 39800���� ����
unsigned short port = 39800;
unsigned short userCount = 0;
std::array<std::string, 50> userList;

//**************�������� �۾�ó�� �κ�***************//
class tcpConnection :public boost::enable_shared_from_this<tcpConnection>
{
private:
	boost::asio::streambuf       _requestBuf;
	boost::asio::ip::tcp::socket _mySocket;
	boost::array<char, 40960>    _buf;
	std::ofstream                _outputFile;
	std::size_t                  _fileSize;

	//Ŭ���̾�Ʈ �ʿ��� ������ ��û �ٷ��
	void handleReadRequest(const boost::system::error_code err, std::size_t bytesTransferred) {

		//������ �߻��� ���
		if (err) {
			return handleError(__FUNCTION__, err);
		}

		// 5. Ŭ���̾�Ʈ�� ��û �ϴ� ������ ����.
		std::cout << "�� ��û ���� ��" << std::endl;
		std::cout << __FUNCTION__ << "(" << bytesTransferred << ")"
			<< " ,��û ���� ó�� ��� ũ�� = " << _requestBuf.in_avail()
			<< " ,��û ���� ũ�� = " << _requestBuf.size()
			<< " ,��û �ִ� ���� ũ�� = " << _requestBuf.max_size()
			<< ".\n";

		std::istream requestStream(&_requestBuf);
		std::string Instruction;

		requestStream >> Instruction;

		// 6.���⼭���� ��û�� ���� ������ ������ ����� ó��.
		if (Instruction == "enter") {

			std::string userName;
			std::string userInfo;

			requestStream >> userName >> userInfo;

			requestStream.read(_buf.c_array(), 2);

			std::cout << "�� ����� ���� ��" << std::endl;
			std::cout << "����� �̸� : " << userName << ", ����� ���� : "
				<< userInfo << std::endl;

			//����� ����Ʈ�� �ش� ����� �߰�.
			userList[userCount++] = userName;
		}

		if (Instruction == "fileSend")
		{
           //���2 - ����ڰ� ������ �����°��.

			std::istream requestStream(&_requestBuf);
			std::string filePath;
			std::string temp;

			requestStream >> filePath;
			std::getline(requestStream,temp,'\n');
			filePath += temp;
			requestStream >> _fileSize;

			//��� ������ filePath���� ���� ���, fileSize���� ������ ũ�Ⱑ ��ⵥ ��
			
			//���� ���๮�� 2�� ����
			requestStream.read(_buf.c_array(), 2);

			//���Ͽ� ���� ���� ���
			std::cout << "�� �������� ��" << std::endl;
			std::cout << filePath << "ũ�� : " << _fileSize << " bytes" << std::endl;
			size_t pos = filePath.find_last_of('\\');

			if (pos != std::string::npos) {
				filePath = filePath.substr(pos + 1);
			}

			//���� �� ��ǻ�Ϳ� ����� ������ ����.
			_outputFile.open(filePath.c_str(), std::ios_base::binary);

			if (!_outputFile) {
				std::cout << "�� ���� ���� ��" << std::endl;
				std::cout << "failed to open" << filePath << std::endl;
				return;
			}

			std::cout << "�� ���� ���� ��" << std::endl;

			//������ ���� ����Ʈ�� �᳻����.
			do {
				requestStream.read(_buf.c_array(), (std::streamsize) _buf.size());
				std::cout << __FUNCTION__ << " ���� ���� " << requestStream.gcount() <<
					" bytes" << std::endl;;

				_outputFile.write(_buf.c_array(), requestStream.gcount());
			} while (requestStream.gcount() > 0);

			//���� ������ �о� ��.
			std::cout << "�� ���� ���� �ٿ� ��" << std::endl;
			async_read(_mySocket, boost::asio::buffer(_buf.c_array(), _buf.size()),
				boost::bind(&tcpConnection::handleReadFileContent,
					shared_from_this(), boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

		}

	}

	//���� ������ �д� ���

	void handleReadFileContent(const boost::system::error_code & err, std::size_t byteTransferred) {

		//����Ʈ ����
		if (byteTransferred > 0) {

			_outputFile.write(_buf.c_array(), (std::streamsize) byteTransferred);
			std::cout << __FUNCTION__ << " �ٿ�ε� : " << _outputFile.tellp() <<
				" bytes." << std::endl;

			if (_outputFile.tellp() >= (std::streamsize) _fileSize) {
				return;
			}
		}

		if (err) {
			std::cout << "�� ���� �߻� ��" << std::endl;
			return handleError(__FUNCTION__, err);
;		}

		async_read(_mySocket, boost::asio::buffer(_buf.c_array(), _buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}

	void handleError(const std::string & functionName, const boost::system::error_code & err) {
		std::cout << "�� ���� �߻� ��" << std::endl;
		std::cout << __FUNCTION__ << "����" << functionName << "���� : " << err << " " << err.message() << std::endl;
	}

public:
	tcpConnection(boost::asio::io_service& io_service) :
		_mySocket(io_service) //������ ��Ĺ �ʱ�ȭ.
	{
	}

	//4 Ŭ���̾�Ʈ�� ��û�� ó���ϴ� �Լ� ����.
	void start()
	{
		std::cout << "�� �Լ� ���� ��" << std::endl;
		std::cout << __FUNCTION__ << std::endl;

		async_read_until(_mySocket, _requestBuf,"\n\n",
			boost::bind(&tcpConnection::handleReadRequest,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	
	boost::asio::ip::tcp::socket& socket()
	{
		return _mySocket;
	}
};

/**********���� �۵� �� ���� ó�� �κ�****************/

//tcpServer��� ���� Ŭ������ ����
class tcpServer :private boost::noncopyable
{
private:
	boost::asio::io_service        _ioService;
	boost::asio::ip::tcp::acceptor _acceptor;
public:
	using ptrTcpConnection = boost::shared_ptr<tcpConnection>;

	//���� ������
	tcpServer(unsigned short port) 
		:_acceptor(_ioService,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port),true)
	{
		std::cout << "������Դϴ�." << std::endl;

		//2. ������ ��Ʈ ��ȣ�� ���ο� ������ accept ó����.
		ptrTcpConnection newConnection(new tcpConnection(_ioService));
		_acceptor.async_accept(newConnection->socket(),
			boost::bind(&tcpServer::handle_Accept, this, newConnection, boost::asio::placeholders::error));

		_ioService.run();
	}

	//3. ������ ������ ó���ϴ� �Լ�
	void handle_Accept(ptrTcpConnection currentConnection, const boost::system::error_code & err) {
		std::cout << "�� ���� ó�� ��" << std::endl;
		std::cout << __FUNCTION__ << std::endl;

		if (!err)
		{
			currentConnection->start();
		}
	}
	~tcpServer()
	{
		_ioService.stop();
	}
};

//1. ���� �۵� �Լ�
void operate()
{
	try
	{
		while (true)
		{
			std::cout << "----------------------" << std::endl;
			std::cout << "��   ���� �۵�    ��" << std::endl;
			std::cout << "----------------------" << std::endl;

			std::cout << "�� ���� �����ڴ� " << userCount << "�� �Դϴ�" << std::endl;
			for (int i = 0; i < userCount; i++)
			{
				std::cout << "����� �̸� : " << userList[i] << std::endl;
			}
			
			std::cout << "�� ���� ���� ������ ���� �Դϴ�" << std::endl;
			boost::filesystem::directory_iterator end;
			for (boost::filesystem::directory_iterator iterator(boost::filesystem::current_path()); iterator != end; iterator++)
			{
				std::cout << iterator->path().leaf() << std::endl;
			}
			std::cout << "----------------------" << std::endl;
			tcpServer * myTcpServer = new tcpServer(port);
			delete myTcpServer;
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "�˼� ���� ����" << std::endl;
	}
}

int main() {
	std::cout << "******************************" << std::endl;
	std::cout << "C++ boost ���� Ž����" << std::endl;
	std::cout << "******************************" << std::endl;
	std::cout << "��Ʈ ��ȣ : " << port << std::endl;
	operate();

	system("pause");
	return 0;
}
