#include "stdafx.h"
#include <string>
#include <regex>
#include <iostream>

int main()
{
	std::regex re("GetValue|Get");
	std::cmatch m;
	{
		//���� ��Ī
		if (std::regex_match("111 GetValue 222", m, re)) {
			std::cout << m[0] << std::endl;
		}
		else
		{
			std::cout << "not matched \n";

		}
	}

	{
		//�κ� ��Ī
		if (std::regex_search("111 GetValues 222", m, re))
		{
			std::cout << m.prefix() << std::endl;
			std::cout << m[0] << std::endl;
			std::cout << m.suffix() << std::endl;
		}
		else
		{
			std::cout << "not matched \n";
		}
	}
}