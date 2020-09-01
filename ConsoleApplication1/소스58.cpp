#include "stdafx.h"
#include <iostream>

int main()
{
	auto foo = [](auto x) { std::cout << x << '\n'; };
	foo(10);
	foo(10.1234);
	foo("hello world");
}