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

//연결 포트를 39800으로 설정
unsigned short port = 39800;
unsigned short userCount = 0;
std::array<std::string, 50> userList;

//**************실질적인 작업처리 부분***************//
class tcpConnection :public boost::enable_shared_from_this<tcpConnection>
{
private:
	boost::asio::streambuf       _requestBuf;
	boost::asio::ip::tcp::socket _mySocket;
	boost::array<char, 40960>    _buf;
	std::ofstream                _outputFile;
	std::size_t                  _fileSize;

	//클라이언트 쪽에서 보내는 요청 다루기
	void handleReadRequest(const boost::system::error_code err, std::size_t bytesTransferred) {

		//오류가 발생한 경우
		if (err) {
			return handleError(__FUNCTION__, err);
		}

		// 5. 클라이언트가 요청 하는 정보를 읽음.
		std::cout << "● 요청 정보 ●" << std::endl;
		std::cout << __FUNCTION__ << "(" << bytesTransferred << ")"
			<< " ,요청 버퍼 처리 기능 크기 = " << _requestBuf.in_avail()
			<< " ,요청 버퍼 크기 = " << _requestBuf.size()
			<< " ,요청 최대 버퍼 크기 = " << _requestBuf.max_size()
			<< ".\n";

		std::istream requestStream(&_requestBuf);
		std::string Instruction;

		requestStream >> Instruction;

		// 6.여기서부터 요청에 따라 서버가 각각의 기능을 처리.
		if (Instruction == "enter") {

			std::string userName;
			std::string userInfo;

			requestStream >> userName >> userInfo;

			requestStream.read(_buf.c_array(), 2);

			std::cout << "● 사용자 접속 ●" << std::endl;
			std::cout << "사용자 이름 : " << userName << ", 사용자 정보 : "
				<< userInfo << std::endl;

			//사용자 리스트에 해당 사용자 추가.
			userList[userCount++] = userName;
		}

		if (Instruction == "fileSend")
		{
           //기능2 - 사용자가 파일을 보내는경우.

			std::istream requestStream(&_requestBuf);
			std::string filePath;
			std::string temp;

			requestStream >> filePath;
			std::getline(requestStream,temp,'\n');
			filePath += temp;
			requestStream >> _fileSize;

			//결과 적으로 filePath에는 파일 경로, fileSize에는 파일의 크기가 담기데 됨
			
			//앞의 개행문제 2개 제거
			requestStream.read(_buf.c_array(), 2);

			//파일에 대한 정보 출력
			std::cout << "● 파일정보 ●" << std::endl;
			std::cout << filePath << "크기 : " << _fileSize << " bytes" << std::endl;
			size_t pos = filePath.find_last_of('\\');

			if (pos != std::string::npos) {
				filePath = filePath.substr(pos + 1);
			}

			//이제 내 컴퓨터에 출력할 파일을 열음.
			_outputFile.open(filePath.c_str(), std::ios_base::binary);

			if (!_outputFile) {
				std::cout << "● 파일 오류 ●" << std::endl;
				std::cout << "failed to open" << filePath << std::endl;
				return;
			}

			std::cout << "● 파일 쓰기 ●" << std::endl;

			//파일의 여분 바이트를 써내려감.
			do {
				requestStream.read(_buf.c_array(), (std::streamsize) _buf.size());
				std::cout << __FUNCTION__ << " 파일 쓰기 " << requestStream.gcount() <<
					" bytes" << std::endl;;

				_outputFile.write(_buf.c_array(), requestStream.gcount());
			} while (requestStream.gcount() > 0);

			//버퍼 정버를 읽어 옴.
			std::cout << "● 파일 내용 다운 ●" << std::endl;
			async_read(_mySocket, boost::asio::buffer(_buf.c_array(), _buf.size()),
				boost::bind(&tcpConnection::handleReadFileContent,
					shared_from_this(), boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

		}

	}

	//파일 내용을 읽는 기능

	void handleReadFileContent(const boost::system::error_code & err, std::size_t byteTransferred) {

		//바이트 전송
		if (byteTransferred > 0) {

			_outputFile.write(_buf.c_array(), (std::streamsize) byteTransferred);
			std::cout << __FUNCTION__ << " 다운로드 : " << _outputFile.tellp() <<
				" bytes." << std::endl;

			if (_outputFile.tellp() >= (std::streamsize) _fileSize) {
				return;
			}
		}

		if (err) {
			std::cout << "● 오류 발생 ●" << std::endl;
			return handleError(__FUNCTION__, err);
;		}

		async_read(_mySocket, boost::asio::buffer(_buf.c_array(), _buf.size()),
			boost::bind(&tcpConnection::handleReadFileContent,
				shared_from_this(), boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}

	void handleError(const std::string & functionName, const boost::system::error_code & err) {
		std::cout << "● 오류 발생 ●" << std::endl;
		std::cout << __FUNCTION__ << "오류" << functionName << "원인 : " << err << " " << err.message() << std::endl;
	}

public:
	tcpConnection(boost::asio::io_service& io_service) :
		_mySocket(io_service) //현재의 소캣 초기화.
	{
	}

	//4 클라이언트의 요청을 처리하는 함수 시작.
	void start()
	{
		std::cout << "● 함수 시작 ●" << std::endl;
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

/**********서버 작동 및 연결 처리 부분****************/

//tcpServer라는 서버 클래스를 정의
class tcpServer :private boost::noncopyable
{
private:
	boost::asio::io_service        _ioService;
	boost::asio::ip::tcp::acceptor _acceptor;
public:
	using ptrTcpConnection = boost::shared_ptr<tcpConnection>;

	//서버 생성자
	tcpServer(unsigned short port) 
		:_acceptor(_ioService,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port),true)
	{
		std::cout << "대기중입니다." << std::endl;

		//2. 지정된 포트 번호로 새로운 연걸을 accept 처리함.
		ptrTcpConnection newConnection(new tcpConnection(_ioService));
		_acceptor.async_accept(newConnection->socket(),
			boost::bind(&tcpServer::handle_Accept, this, newConnection, boost::asio::placeholders::error));

		_ioService.run();
	}

	//3. 실제로 연결을 처리하는 함수
	void handle_Accept(ptrTcpConnection currentConnection, const boost::system::error_code & err) {
		std::cout << "● 연결 처리 ●" << std::endl;
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

//1. 서버 작동 함수
void operate()
{
	try
	{
		while (true)
		{
			std::cout << "----------------------" << std::endl;
			std::cout << "●   서버 작동    ●" << std::endl;
			std::cout << "----------------------" << std::endl;

			std::cout << "※ 현재 접속자는 " << userCount << "명 입니다" << std::endl;
			for (int i = 0; i < userCount; i++)
			{
				std::cout << "사용자 이름 : " << userList[i] << std::endl;
			}
			
			std::cout << "※ 현재 서버 폴더의 정보 입니다" << std::endl;
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
		std::cout << "알수 없는 에러" << std::endl;
	}
}

int main() {
	std::cout << "******************************" << std::endl;
	std::cout << "C++ boost 파일 탐색기" << std::endl;
	std::cout << "******************************" << std::endl;
	std::cout << "포트 번호 : " << port << std::endl;
	operate();

	system("pause");
	return 0;
}
