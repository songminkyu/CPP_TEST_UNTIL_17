#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

//�ν�Ʈ�� tcp �ƺ񸣷��� ���

using boost::asio::ip::tcp;
bool is_send = true;
/************************������ �۽� �ϴ� �κ�*************************************/
class fileTcpClient {
private:
	tcp::resolver			  _resolver;
	tcp::socket				  _socket;
	boost::array<char, 8192>  _buf;
	boost::asio::streambuf	  _request;
	std::ifstream			  _sourceFile;
	boost::asio::io_service & _io_service;

	//������ ������ ������ ���� �ϴ� Resolveing ó�� �Լ�
	void handleResolve(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err) {
			//������ �߻����� �ʾѴٸ�
			tcp::endpoint endpoint = *myiterator;
			_socket.async_connect(endpoint, 
				boost::bind(&fileTcpClient::handleConnect, 
					this, boost::asio::placeholders::error, 
					++myiterator));
		}
		else {
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	void handleConnect(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		//������ �߻� ���� ���� ���
		if (!err)
		{
			std::cout << "��   ���� ����   ��" << std::endl;
			boost::asio::async_write(_socket,
				_request, 
				boost::bind(&fileTcpClient::handleWriteFile, 
					this, 
					boost::asio::placeholders::error));
		}
		else if (myiterator != tcp::resolver::iterator())
		{
			//����Ʈ�� ù ��° endpoint �� ���� �õ�
			
			_socket.close();
			tcp::endpoint endpoint = *myiterator;
			_socket.async_connect(endpoint, 
				boost::bind(&fileTcpClient::handleConnect, 
					this,
					boost::asio::placeholders::error, 
					++myiterator));
		}
		else
		{
			std::cout << "handleConnect" << std::endl;
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	//������ ������ ���� ���� �Լ�
	void handleWriteFile(const boost::system::error_code & err) 
	{
		// ������ �߻� ���� ���� ���
		if (!err) 
		{
			//������ ������ ���� ������
			if (_sourceFile.eof() == false)
			{
				//���ۿ� ����
				_sourceFile.read(_buf.c_array(), (std::streamsize) _buf.size());

				if (_sourceFile.gcount() <= 0)
				{
					std::cout << "��   ���� ����   ��" << std::endl;
					std::cout << "read file error" << std::endl;
					return;
				}
				else
				{
					//������ ���� ����.
					std::cout << _sourceFile.gcount() << " bytes ���� �Ϸ�, ��ü :"
						<< _sourceFile.tellg() << " bytes. \n";

					//������ ������ ���� �ϴ� async_write �Լ� ����
					boost::asio::async_write(_socket,
						boost::asio::buffer(_buf.c_array(), _sourceFile.gcount()),
						boost::bind(&fileTcpClient::handleWriteFile,
							this,
							boost::asio::placeholders::error));
				}
				//������ �߻� �ϴ°��.
				if (err)
				{
					std::cout << "��   ���� ����   ��" << std::endl;
					std::cout << "send error : " << err << std::endl;
					return;
				}
			}
			else
				return;
		}
		else 
		{
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";		
		}
	}
public:
	fileTcpClient(boost::asio::io_service & io_service, const std::string & server, const std::string &path)
		:_resolver(io_service), _socket(io_service), _io_service(io_service)
	{

		//������ IP�� ��Ʈ �ּҸ� ������ ����.
		size_t pos = server.find(":");
		if (pos == std::string::npos)
			return;

		std::string portString = server.substr(pos + 1);
		std::string serverIP = server.substr(0, pos);

		//�ڽ��� ������ ���� �ҽ� ������ ����.
		_sourceFile.open(path.c_str(), std::ios_base::binary | std::ios_base::ate);

		//�ҽ������� ���� ���� �ʴ°��.
		if (!_sourceFile) {
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "���� �ҷ����� ���� : " << path << std::endl;
			return;
		}

		//���Ͽ� ���� ������ ��� ����.
		size_t fileSize = _sourceFile.tellg();
		_sourceFile.seekg(0);

		//���� ������ ������ ��ο� ������ ũ�⸦ ����.
		std::cout << "��   ���� ���� ����   ��" << std::endl;
		std::ostream requestStream(&_request);
		requestStream << "fileSend" << "\n" << path << "\n" << fileSize << "\n\n";
		std::cout << "��û ũ�� : " << _request.size() << std::endl;

		//������ ������ �̸��� �����ϱ����� �񵿱������ resolving ó��
		//endpoints�� ����Ʈ ó��.
		tcp::resolver::query query(serverIP, portString);
		_resolver.async_resolve(query, boost::bind(&fileTcpClient::handleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
	}
};

class userTcpClient
{
private:
	tcp::resolver resolver;
	tcp::socket socket;
	boost::array<char, 8192> buf;
	boost::asio::streambuf request;
	boost::asio::io_service & _io_service;

	void handleResolve(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err) {
			//������ �߻����� �ʾѴٸ�
			tcp::endpoint endpoint = *myiterator;
			socket.async_connect(endpoint, boost::bind(&userTcpClient::handleConnect, 
				this, boost::asio::placeholders::error, 
				++myiterator));

		}
		else {
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	void handleConnect(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err)
		{
			std::cout << "��   ���� ����   ��" << std::endl;
			boost::asio::async_write(socket, request, boost::bind(&userTcpClient::handleWriteUser,
				this, 
				boost::asio::placeholders::error));
		}
		else if (myiterator != tcp::resolver::iterator())
		{
			socket.close();
			tcp::endpoint endpoint = *myiterator;
			socket.async_connect(endpoint, 
				boost::bind(&userTcpClient::handleConnect, 
					this, boost::asio::placeholders::error, 
					++myiterator));
		}
		else
		{
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
			is_send = false;
		}
	}
	void handleWriteUser(const boost::system::error_code & err) 
	{

		if (err)
		{
			std::cout << "��   ���� ����   ��" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
			is_send = false;
		}
	}
public:
	userTcpClient(boost::asio::io_service & io_service, const std::string & server, std::string userName, std::string userInfo)
		:resolver(io_service), socket(io_service), _io_service(io_service)
	{
		size_t pos = server.find(":");
		if (pos == std::string::npos)
			return;

		std::string portString = server.substr(pos + 1);
		std::string serverIP = server.substr(0, pos);

		
		std::cout << "�� ����� ���� ���� ��" << std::endl;
		std::ostream requestStream(&request);
		requestStream << "enter" << "\n" << userName << "\n"
			<< userInfo << "\n\n";
		std::cout << "����� �̸� : " << userName << ", ����� ���� : " << userInfo << std::endl;

		tcp::resolver::query query(serverIP, portString);
		resolver.async_resolve(query, 
			boost::bind(&userTcpClient::handleResolve, 
			this, boost::asio::placeholders::error, 
			boost::asio::placeholders::iterator));
	}
	/*userTcpClient& operator=(const userTcpClient& utc)
	{
		return *this;
	}*/
	void close()
	{
		_io_service.post([this]() { socket.close(); });
	}
};


void SendFile() {

	try {
		std::cout << "-----------------------" << std::endl;
		std::cout << "        ���� ����        " << std::endl;
		std::cout << "-----------------------" << std::endl;

		std::string * IP = new std::string("127.0.0.1:39800");
		std::string FILE;
		std::cout << "���� �� ���� ��� : ";
		std::getline(std::cin, FILE, '\n');

		boost::asio::io_service io_service;
		fileTcpClient client(io_service, *IP, FILE);
		io_service.run();

		std::cout << "��   ���� ���   ��" << std::endl;
		std::cout << "����" << FILE << "���ۿ� ���� �Ͽ����ϴ�. \n";
	}
	catch (std::exception & e) {
		
		std::cout << "��   ���� ����   ��" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void SendUser(std::string userName, std::string userInfo) {

	try {
		std::cout << "-----------------------" << std::endl;
		std::cout << "        ����� ����        " << std::endl;
		std::cout << "-----------------------" << std::endl;

		std::string * IP = new std::string("127.0.0.1:39800");
		boost::asio::io_service io_service;
		userTcpClient client(io_service, *IP, userName, userInfo);
		io_service.run();

		std::cout << "��   ���� ���   ��" << std::endl;

		if (is_send == true) {
			std::cout << "   ����� ���� ���ۿ� ���� �߽��ϴ�.   " << std::endl;
			
		}
		else
		{
			std::cout << "   ����� ���� ���ۿ� ���� �߽��ϴ�.   " << std::endl;
		}
	}
	catch (std::exception & e) {
		std::cout << "��   ���� ����   ��" << std::endl;
		std::cerr << e.what() << std::endl;
	}

}

int main() {
	std::string input;
	std::string userName;
	std::string userInfo;

	std::cout << "����� �̸��� �Է� �ϼ���" << std::endl;
	std::getline(std::cin, userName, '\n');

	std::cout << "����� ������ �Է� �ϼ���" << std::endl;
	std::getline(std::cin, userInfo, '\n');

	std::cout << "����� ����� �õ� �մϴ�" << std::endl;
	SendUser(userName, userInfo);

	std::cout << "��   1.���� �۽�   ��" << std::endl;
	std::cout << "��   2.���α׷� ����   ��" << std::endl;

	while (std::getline(std::cin, input, '\n')) {

		if (input == "1") {
			SendFile();
		}
		else if (input == "2") {
	
			system("pause");
			return 0;
		}
	}

}