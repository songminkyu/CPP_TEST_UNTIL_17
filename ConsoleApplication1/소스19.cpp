#include "stdafx.h"
#include <iostream>
#include <numeric>

int main()
{
	auto ret = std::lcm(2, 4);
	std::cout << ret << std::endl;

	ret = std::gcd(2, 4);
	std::cout << ret << std::endl;

}