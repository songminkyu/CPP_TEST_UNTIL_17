#include "stdafx.h"
#include <string>
#include <regex>
#include <iostream>
int main()
{
	std::regex re("GetValue|Get");
	std::cmatch m;
	{
		if (std::regex_search("GetValue", m, re)) {
			std::cout << m[0] << std::endl;
		}
		else
		{
			std::cout << "not matched \n";

		}
	}

	{
		if (std::regex_search("Gets", m, re))
		{
			std::cout << m[0] << std::endl;
		}
		else
		{
			std::cout << "not matched \n";
		}
	}
}