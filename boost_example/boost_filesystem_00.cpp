#include <iostream>
#include <boost\filesystem.hpp>
#include <Windows.h>
namespace fs = boost::filesystem;

int main(int argc, TCHAR* argv[]) {
	std::cout << "current_path : " << fs::current_path() << std::endl;

	if (argc < 2) return 0;
	auto size = fs::file_size(argv[1]);

	std::cout << "file size :" << size << std::endl;

	return 0;
}