#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <utility>

int add(int first, int second) { return first + second; }

template<typename T>
T add_generic(T first, T second) { return first + second; }

auto add_lambda = [](auto first, auto second) { return first + second; };

void test(int m1, std::string m2, double m3)
{
	std::cout << "m1 : " << m1 << std::endl;
	std::cout << "m2 : " << m2 << std::endl;
	std::cout << "m3 : " << m3 << std::endl;
}
struct A {
	void operator()(int m1, std::string m2, double m3) {
		std::cout << "m1 : " << m1 << std::endl;
		std::cout << "m2 : " << m2 << std::endl;
		std::cout << "m3 : " << m3 << std::endl;
	}
};
int main()
{
	// OK
	std::cout << std::apply(add, std::make_pair(1, 2)) << '\n';

	// Error: can't deduce the function type
	// std::cout << std::apply(add_generic, std::make_pair(2.0f, 3.0f)) << '\n'; 

	// OK
	std::cout << std::apply(add_lambda, std::make_pair(2.0f, 3.0f)) << '\n';

	A a;
	std::tuple m{ 10,"hellow",30.3 };
	std::apply(test, m);
	std::apply(a, m);

	return 0;
}