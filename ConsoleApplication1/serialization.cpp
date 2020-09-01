#include "stdafx.h"
#include <iostream>

struct A { int a; int b; };
struct B { int c; int d; };
struct C : A,B { int e; int f; };

std::ostream& operator<<(std::ostream& cout, const C& c) {
	std::cout << c.a << std::endl;
	std::cout << c.b << std::endl;
	std::cout << c.c << std::endl;
	std::cout << c.d << std::endl;
	std::cout << c.e << std::endl;
	std::cout << c.f << std::endl;

	return cout;
}

int main()
{
	C c{ {1,2},{3,4},5,6 };
	std::cout << c << std::endl;
}