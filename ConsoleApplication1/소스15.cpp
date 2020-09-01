#include "stdafx.h"
#include <iostream>
#include <locale>

int main()
{
	auto & fn = std::use_facet< std::numpunct<char>>(std::cout.getloc());

	/*
	1. locae 은 facet를 element로 하는 container~
	2. 특정 facet element를 얻어낼 때 type를 템플릿 인자로 넘겨서 얻어오는 구조
	3. std::numpunct<char> : 얻기 원하는 facet element 임
	4. std::cout.getloc(); container 이다.
	*/

	std::cout << "thousands_sep : " << fn.thousands_sep() << std::endl;
	std::cout << "declmal_point : " << fn.decimal_point() << std::endl;
	std::cout << "falsename : " << fn.falsename() << std::endl;
	std::cout << "truename : " << fn.truename() << std::endl;
	std::cout << "grouping" << fn.grouping() << std::endl;

}