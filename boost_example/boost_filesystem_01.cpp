#include <iostream>
#include <boost\filesystem.hpp>
#include <Windows.h>

namespace fs = boost::filesystem;


int main(int argc, TCHAR * argv[]) {
	if (argc < 2) return 0;

	fs::path file(argv[1]);

	do {
		if (!fs::exists(file)) {
			std::cout << "file not found. \n";
			break;
		}

		if (!fs::is_regular_file(file)){
			std::cout << "regular file. \n";
			break;
		}

		if (!fs::is_directory(file)) {
			std::cout << "directory \n";
			break;
		}
		
	} while (false);

	return 0;
}