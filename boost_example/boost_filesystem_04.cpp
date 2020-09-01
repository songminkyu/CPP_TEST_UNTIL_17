#include <iostream>
#include <boost/filesystem.hpp>
#include <Windows.h>
#include <chrono>
#include <cstring>
namespace fs = boost::filesystem;


std::vector<std::wstring> get_all_files_names_within_folder(std::wstring folder)
{

	std::vector<std::wstring> names;
	std::wstring search_path = folder + TEXT("/*.*");
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if ((fd.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY))) 
			{
				//여긴 시스템 디렉토리 필터 부분
				continue;
			}
			else 
			{
				names.push_back(folder + TEXT("\\") + fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


int main(int argc, TCHAR* argv[]) {

	fs::path file(R"(C:\)");
	//fs::path file(R"(d:\test\)");
	do {
		if (!fs::exists(file)) {
			std::cout << "file not found.\n";
			break;
		}

		if (!fs::is_directory(file)) break;

		fs::recursive_directory_iterator it(file);
		fs::recursive_directory_iterator fend;

		for (auto && get : it)
		{
			for (auto && item : get_all_files_names_within_folder(get.path().wstring()))
			{				
				std::wcout << item << std::endl;
			}
		}
	} while (false);

	return 0;
}

