#include <iostream>
#include <boost\filesystem.hpp>
#include <Windows.h>

namespace fs = boost::filesystem;

int main(int argc, TCHAR* argv[]) {

	if (argc < 2) return 0;

	fs::path file(argv[1]);
	do
	{
		if (!fs::exists(file)) {
			std::cout << "file not found.\n";
			break;
		}

		if (fs::is_directory(file)) {
			std::cout << file << "is a directory containing : \n";
			for (fs::directory_entry & x : fs::directory_iterator(file)) {
				std::cout << "\t" << x.path() << std::endl;
			}
		}
	} while (false);
}