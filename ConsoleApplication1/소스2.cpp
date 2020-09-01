#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <string>

void main()
{
	int a;
	std::string b;
	double c;
	
	std::tuple<int&, std::string&, double&> ref_tuple(a, b, c);
	ref_tuple = std::tuple(1, "hellow", 3.4);
	std::cout << a << " ," << b << " ," << c << std::endl;

	std::tie(a, b, c) = std::tuple(100, "song", 20.56);
	std::cout << a << " ," << b << " ," << c << std::endl;
		
	//struct binding
	{
		auto &&[a, b, c] = std::tuple{ 999,"minkyu",300.30 };
		std::cout << a << " ," << b << " ," << c << std::endl;
	}

}