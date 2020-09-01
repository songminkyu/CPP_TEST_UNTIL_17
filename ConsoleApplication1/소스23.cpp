#include "stdafx.h"
#include <iostream>
#include <filesystem>


namespace fs = std::filesystem;

int main()
{
	try {
		fs::remove(fs::path(R"(d:\wholink.exe)"));
		fs::create_symlink(fs::path(R"(c:\Users\USER\Downloads\afreecatvstreamer_installer.exe)"), fs::path(R"(d:\wholink.exe)"));

	}
	catch (fs::filesystem_error& err)
	{
		std::cout << err.what() << std::endl;
	}

	auto container = fs::directory_iterator(
		fs::path("d:\\"),
		fs::directory_options::skip_permission_denied |
		fs::directory_options::follow_directory_symlink
	);

	for (auto & p : container)
	{
		std::cout << "dir : " << p.is_directory();
		bool bsym = p.is_symlink();
		if (bsym)
		{
			std::cout << "symlinc : " << fs::read_symlink(p);
		}

		std::cout << "name : " << p.path().lexically_normal();
		std::cout << "size : " << p.file_size() << std::endl;
	}
}