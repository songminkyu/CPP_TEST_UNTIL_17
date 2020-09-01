#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

//부스트의 tcp 아비르러리 사용

using boost::asio::ip::tcp;
bool is_send = true;
/************************파일을 송신 하는 부분*************************************/
class fileTcpClient {
private:
	tcp::resolver			  _resolver;
	tcp::socket				  _socket;
	boost::array<char, 8192>  _buf;
	boost::asio::streambuf	  _request;
	std::ifstream			  _sourceFile;
	boost::asio::io_service & _io_service;

	//실제로 서버에 접속을 설정 하는 Resolveing 처리 함수
	void handleResolve(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err) {
			//오류가 발생하지 않앗다면
			tcp::endpoint endpoint = *myiterator;
			_socket.async_connect(endpoint, 
				boost::bind(&fileTcpClient::handleConnect, 
					this, boost::asio::placeholders::error, 
					++myiterator));
		}
		else {
			std::cout << "●   세팅 오류   ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	void handleConnect(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		//오류가 발생 하지 않은 경우
		if (!err)
		{
			std::cout << "●   파일 전송   ●" << std::endl;
			boost::asio::async_write(_socket,
				_request, 
				boost::bind(&fileTcpClient::handleWriteFile, 
					this, 
					boost::asio::placeholders::error));
		}
		else if (myiterator != tcp::resolver::iterator())
		{
			//리스트의 첫 번째 endpoint 로 연결 시도
			
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
			std::cout << "●   전송 오류   ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	//실제로 파일을 쓰기 위한 함수
	void handleWriteFile(const boost::system::error_code & err) 
	{
		// 오류가 발생 하지 않은 경우
		if (!err) 
		{
			//보내는 파일이 끝날 때까지
			if (_sourceFile.eof() == false)
			{
				//버퍼에 담음
				_sourceFile.read(_buf.c_array(), (std::streamsize) _buf.size());

				if (_sourceFile.gcount() <= 0)
				{
					std::cout << "●   파일 오류   ●" << std::endl;
					std::cout << "read file error" << std::endl;
					return;
				}
				else
				{
					//보내는 파일 정보.
					std::cout << _sourceFile.gcount() << " bytes 전송 완료, 전체 :"
						<< _sourceFile.tellg() << " bytes. \n";

					//실제로 파일을 전송 하는 async_write 함수 실행
					boost::asio::async_write(_socket,
						boost::asio::buffer(_buf.c_array(), _sourceFile.gcount()),
						boost::bind(&fileTcpClient::handleWriteFile,
							this,
							boost::asio::placeholders::error));
				}
				//오류가 발생 하는경우.
				if (err)
				{
					std::cout << "●   전송 오류   ●" << std::endl;
					std::cout << "send error : " << err << std::endl;
					return;
				}
			}
			else
				return;
		}
		else 
		{
			std::cout << "●   전송 오류   ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";		
		}
	}
public:
	fileTcpClient(boost::asio::io_service & io_service, const std::string & server, const std::string &path)
		:_resolver(io_service), _socket(io_service), _io_service(io_service)
	{

		//서버의 IP와 포트 주소를 나누어 저장.
		size_t pos = server.find(":");
		if (pos == std::string::npos)
			return;

		std::string portString = server.substr(pos + 1);
		std::string serverIP = server.substr(0, pos);

		//자신의 보내고 싶은 소스 파일을 열음.
		_sourceFile.open(path.c_str(), std::ios_base::binary | std::ios_base::ate);

		//소스파일이 존재 하지 않는경우.
		if (!_sourceFile) {
			std::cout << "●   파일 오류   ●" << std::endl;
			std::cout << "파일 불러오기 실패 : " << path << std::endl;
			return;
		}

		//파일에 대한 정보를 담는 변수.
		size_t fileSize = _sourceFile.tellg();
		_sourceFile.seekg(0);

		//먼저 서버에 파일의 경로와 파일의 크기를 전송.
		std::cout << "●   파일 정보 전송   ●" << std::endl;
		std::ostream requestStream(&_request);
		requestStream << "fileSend" << "\n" << path << "\n" << fileSize << "\n\n";
		std::cout << "요청 크기 : " << _request.size() << std::endl;

		//서버와 서비스의 이름을 번역하기위해 비동기식으로 resolving 처리
		//endpoints의 리스트 처리.
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
			//오류가 발생하지 않앗다면
			tcp::endpoint endpoint = *myiterator;
			socket.async_connect(endpoint, boost::bind(&userTcpClient::handleConnect, 
				this, boost::asio::placeholders::error, 
				++myiterator));

		}
		else {
			std::cout << "●   세팅 오류   ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	void handleConnect(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err)
		{
			std::cout << "●   정보 전송   ●" << std::endl;
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
			std::cout << "●   전송 오류   ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
			is_send = false;
		}
	}
	void handleWriteUser(const boost::system::error_code & err) 
	{

		if (err)
		{
			std::cout << "●   전송 오류   ●" << std::endl;
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

		
		std::cout << "● 사용자 정보 전송 ●" << std::endl;
		std::ostream requestStream(&request);
		requestStream << "enter" << "\n" << userName << "\n"
			<< userInfo << "\n\n";
		std::cout << "사용자 이름 : " << userName << ", 사용자 정보 : " << userInfo << std::endl;

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
		std::cout << "        파일 전송        " << std::endl;
		std::cout << "-----------------------" << std::endl;

		std::string * IP = new std::string("127.0.0.1:39800");
		std::string FILE;
		std::cout << "전송 할 파일 경로 : ";
		std::getline(std::cin, FILE, '\n');

		boost::asio::io_service io_service;
		fileTcpClient client(io_service, *IP, FILE);
		io_service.run();

		std::cout << "●   전송 결과   ●" << std::endl;
		std::cout << "파일" << FILE << "전송에 성공 하였습니다. \n";
	}
	catch (std::exception & e) {
		
		std::cout << "●   전송 오류   ●" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void SendUser(std::string userName, std::string userInfo) {

	try {
		std::cout << "-----------------------" << std::endl;
		std::cout << "        사용자 전송        " << std::endl;
		std::cout << "-----------------------" << std::endl;

		std::string * IP = new std::string("127.0.0.1:39800");
		boost::asio::io_service io_service;
		userTcpClient client(io_service, *IP, userName, userInfo);
		io_service.run();

		std::cout << "●   전송 결과   ●" << std::endl;

		if (is_send == true) {
			std::cout << "   사용자 정보 전송에 성공 했습니다.   " << std::endl;
			
		}
		else
		{
			std::cout << "   사용자 정보 전송에 실패 했습니다.   " << std::endl;
		}
	}
	catch (std::exception & e) {
		std::cout << "●   전송 오류   ●" << std::endl;
		std::cerr << e.what() << std::endl;
	}

}

int main() {
	std::string input;
	std::string userName;
	std::string userInfo;

	std::cout << "사용자 이름을 입력 하세요" << std::endl;
	std::getline(std::cin, userName, '\n');

	std::cout << "사용자 정보를 입력 하세요" << std::endl;
	std::getline(std::cin, userInfo, '\n');

	std::cout << "사용자 등록을 시도 합니다" << std::endl;
	SendUser(userName, userInfo);

	std::cout << "●   1.파일 송신   ●" << std::endl;
	std::cout << "●   2.프로그램 종료   ●" << std::endl;

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