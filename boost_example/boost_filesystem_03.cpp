#include <iostream>
#include <boost\filesystem.hpp>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <string>

namespace fs = boost::filesystem;

int main(int argc, TCHAR* argv[]) {
	if (argc < 2)	return 0;

	fs::path file(argv[1]);
	do {
		if (!fs::exists(file)) {
			std::cout << "file not found.\n";
			break;
		}

		if (fs::is_directory(file)) {
			std::cout << file << "is a directory containing : \n";

			std::vector<std::string> v;
			for (auto && x : fs::directory_iterator(file)) {
				v.push_back(x.path().filename().string());
			}

			std::sort(std::begin(v), std::end(v));

			for (auto&& x : v) {
				std::cout << "\t" << x << std::endl;
			}
		}
	} while (false);

	return 0;
}