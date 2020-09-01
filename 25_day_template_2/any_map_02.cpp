#include "stdafx.h"
#include <iostream>
#include <map>
#include <any>
int main()
{
	std::map<std::string, std::any> Notebook;

	std::string name{ "Pluto" };
	int year = 2015;

	Notebook["PetName"] = name;
	Notebook["Born"] = year;

	std::string strS = std::any_cast<std::string>(Notebook["PetName"]);; // = "Pluto"
	int intI = std::any_cast<int>(Notebook["Born"]); // = 2015

	std::cout << strS.c_str() << std::endl;
	std::cout << intI << std::endl;

}