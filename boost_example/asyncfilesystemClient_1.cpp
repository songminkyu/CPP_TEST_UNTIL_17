#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

class tcpClient {
private:
	tcp::resolver              resolver;
	tcp::socket                socket;
	boost::array<char, 1024>   buf;
	boost::asio::streambuf     request;
	std::ifstream              sourceFile;

	//서버에 세팅하는 Resolve 처리함수
	void handleResolve(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {


		if (!err) {
			//오류가 발생하지 않앗다면
			tcp::endpoint endpoint = *myiterator;
			socket.async_connect(endpoint, boost::bind(&tcpClient::handleConnect, this, boost::asio::placeholders::error, ++myiterator));

		}
		else {
			std::cout << "● 세팅 오류 ●" << std::endl;
			std::cout << "Error : " << err.message() << "\n";
		}
	}

	void handleConnect(const boost::system::error_code & err, tcp::resolver::iterator myiterator) {

		if (!err) 
		{
			std::cout << "● 파일 전송 ●" << std::endl;
			boost::asio::async_write(socket, request, boost::bind(&tcpClient::handleWriteFile, this, boost::asio::placeholders::error));	
		}
		else if (myiterator != tcp::resolver::iterator()) 
		{
			socket.close();
			tcp::endpoint endpoint = *myiterator;
			socket.async_connect(endpoint, boost::bind(&tcpClient::handleConnect, this, boost::asio::placeholders::error, ++myiterator));
		}
		else 
		{
			std::cout << "Error : " << err.message() << "\n";
		}

	}

	void handleWriteFile(const boost::system::error_code & err) 
	{

		if (!err) 
		{

			if (sourceFile.eof() == false) 
			{

				sourceFile.read(buf.c_array(), (std::streamsize) buf.size());
				if (sourceFile.gcount() <= 0)
				{
					std::cout << "read file error" << std::endl;
					return;
				}

				std::cout << "send" << sourceFile.gcount() << "bytes, total:" << sourceFile.tellg() << "bytes \n";

				boost::asio::async_write(socket, boost::asio::buffer(buf.c_array(),sourceFile.gcount()),
					boost::bind(&tcpClient::handleWriteFile,this,boost::asio::placeholders::error));

				if (err) 
				{
					std::cout << "send error : " << err << std::endl;
					return;
				}
				else 
				{
					return;
				}
			}
		}
		else
		{
			std::cout << "Error : " << err.message() << "\n";
		}
	}
public:

	tcpClient(boost::asio::io_service &io_service, const std::string &server, const std::string & path)
		:resolver(io_service), socket(io_service) {
		
		size_t pos = server.find(":");

		if (pos == std::string::npos) {
			return;

		}
		std::string portString = server.substr(pos + 1);
		std::string serverIP = server.substr(0, pos);

		sourceFile.open(path.c_str(), std::ios_base::binary | std::ios_base::ate);

		if (!sourceFile) {
			std::cout << "failed to open" << path << std::endl;
			return;
		}

		size_t fileSize = sourceFile.tellg();
		sourceFile.seekg(0);

		std::ostream requestStream(&request);
		requestStream << path << "\n"
			<< fileSize << "\n\n";
		std::cout << "request size : " << request.size() << std::endl;

		tcp::resolver::query query(serverIP, portString);
		resolver.async_resolve(query, boost::bind(&tcpClient::handleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));

	}
};

int main(int argc, char * argv[])
{
	try {
		boost::asio::io_service io_service;
		tcpClient client(io_service, "127.0.0.1:39800", "c:\\Users\\USER\\Downloads\\12312312321321344.txt");
		io_service.run();

		std::cout << "send file complate " << std::endl;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	
	system("pause");

	return 0;
}