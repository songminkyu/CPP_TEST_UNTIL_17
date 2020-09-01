#include "stdafx.h"
#include <iostream>
#include <variant>
#include <string>
#include <type_traits>
int main()
{
	std::variant<int, double, std::string> v[3];

	v[0] = 100;
	v[1]= 12.33;
	v[2]= "songminkyu";


	for (auto && i : v)
	{
		if (i.index() == 0)
		{
			std::cout << std::get<0>(i) << std::endl;
		}
		else if (i.index() == 1)
		{
			std::cout << std::get<1>(i) << std::endl;
		}
		else if (i.index() == 2)
		{
			std::cout << std::get<2>(i) << std::endl;
		}
	}
	
}