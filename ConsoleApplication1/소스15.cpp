#include "stdafx.h"
#include <iostream>
#include <locale>

int main()
{
	auto & fn = std::use_facet< std::numpunct<char>>(std::cout.getloc());

	/*
	1. locae �� facet�� element�� �ϴ� container~
	2. Ư�� facet element�� �� �� type�� ���ø� ���ڷ� �Ѱܼ� ������ ����
	3. std::numpunct<char> : ��� ���ϴ� facet element ��
	4. std::cout.getloc(); container �̴�.
	*/

	std::cout << "thousands_sep : " << fn.thousands_sep() << std::endl;
	std::cout << "declmal_point : " << fn.decimal_point() << std::endl;
	std::cout << "falsename : " << fn.falsename() << std::endl;
	std::cout << "truename : " << fn.truename() << std::endl;
	std::cout << "grouping" << fn.grouping() << std::endl;

}