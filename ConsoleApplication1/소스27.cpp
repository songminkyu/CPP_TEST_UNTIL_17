#include "stdafx.h"
#include <string>
#include <regex>
#include <iostream>
int main()
{
	std::regex re("Get|GetValue");
	std::cmatch m;
	{
		if (std::regex_match("GetValue", m, re)) {
			std::cout << m[0] << std::endl;
		}
		else
		{
			std::cout << "not matched \n";

		}
	}

	{
		if (std::regex_match("GetValuesss", m, re))
		{
			std::cout << m[0] << std::endl;
		}
		else
		{
			std::cout << "not matched \n";
		}
	}
}