#include "stdafx.h"
#include <iostream>
#include <variant>
#include <string>
void main()
{
	std::variant<int, std::string> V1{ 200 };
	std::variant<int, std::string> V2{ "hellow" };
	
	std::visit(
		[](auto A, auto B) {std::cout << A <<  B << std::endl; 
		},V1, V2);
	
}