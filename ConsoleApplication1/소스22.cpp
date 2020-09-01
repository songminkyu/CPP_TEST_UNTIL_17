#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void demo_perms(fs::perms p)
{
	std::cout 
		<< ((p & fs::perms::owner_read)   != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::owner_write)  != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::owner_exec)   != fs::perms::none ? "x" : "-")
		<< ((p & fs::perms::group_read)   != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::group_write)  != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::group_exec)   != fs::perms::none ? "x" : "-")
		<< ((p & fs::perms::others_read)  != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::others_exec)  != fs::perms::none ? "x" : "-") << '\n';
}

void main()
{
	try {
		std::ofstream("test.txt"); //create file
		std::cout << "Create file with permission : ";
		demo_perms(fs::status("test.txt").permissions());

		fs::permissions("test.txt", (fs::perms::owner_all | fs::perms::group_all),fs::perm_options::add);

		std::cout << "After replacing o+rwx and g+rwx :  ";
		demo_perms(fs::status("test.txt").permissions());
		fs::remove("test.txt");
	}
	catch (fs::filesystem_error & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
