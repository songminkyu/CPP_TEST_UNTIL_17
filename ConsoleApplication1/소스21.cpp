#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void demo_perms(fs::perms p)
{
	std::cout << ((p & fs::perms::owner_read) != fs::perms::no_perms ? "r" : "-")
		<< ((p & fs::perms::owner_write) != fs::perms::no_perms ? "w" : "-")
		<< ((p & fs::perms::owner_exe) != fs::perms::no_perms ? "x" : "-")
		<< ((p & fs::perms::group_read) != fs::perms::no_perms ? "r" : "-")
		<< ((p & fs::perms::group_write) != fs::perms::no_perms ? "w" : "-")
		<< ((p & fs::perms::group_exe) != fs::perms::no_perms ? "x" : "-")
		<< ((p & fs::perms::others_read) != fs::perms::no_perms ? "r" : "-")
		<< ((p & fs::perms::others_write) != fs::perms::no_perms ? "w" : "-")
		<< ((p & fs::perms::others_exe) != fs::perms::no_perms ? "x" : "-") << '\n';		      
}

void main()
{
	try {
		std::ofstream("test.txt"); //create file
		std::cout << "Create file with permission : ";
		demo_perms(fs::status("test.txt").permissions());

		fs::permissions("test.txt", (fs::perms::owner_all | fs::perms::group_all));

		std::cout << "After replacing o+rwx and g+rwx :  ";
		demo_perms(fs::status("test.txt").permissions());
		fs::remove("test.txt");
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
