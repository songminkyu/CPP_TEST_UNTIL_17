#include "stdafx.h"
#include <iostream>
#include <functional>
#include <string>

int add(std::string comment, int a, int b, int c)
{
	std::cout << comment;
	return a + b + c;
}

int main() {
	//auto fn1 = std::bind_front(&add, "comment", 1); C++20 Áö¿ø 
	//auto fn2 = std::bind_front(fn1, 2);
	//std::cout << fn1(2, 3);
	//std::cout << fn1(3);
}