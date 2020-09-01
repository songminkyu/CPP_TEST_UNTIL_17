#include "stdafx.h"
#include <variant>
#include <iostream>

struct A
{
	A( int,int )
	{
		std::cout << "hellow world" << std::endl; 
	}
};

int main()
{
	std::variant<A, int>(std::in_place_type<A>, 1, 2);
}