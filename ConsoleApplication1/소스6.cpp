#include "stdafx.h"
#include <iostream>
#include <map>
#include <tuple>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

struct Foo {
	Foo(std::tuple<int, float>)
	{
		std::cout << "Constructed a Foo from a tuple\n";
	}
	Foo(int, float)
	{
		std::cout << "Constructed a Foo from an int and a float\n";
	}
};

int main()
{
	std::map<int, std::string> m;
	m.emplace(std::piecewise_construct,
		std::forward_as_tuple(10),
		std::forward_as_tuple(20, 'a'));

	std::cout << "m[10] = " << m[10] << std::endl;

	std::tuple<int, float> t(1, 3.14);
	std::pair<Foo, Foo> p1(t, t);
	std::pair<Foo, Foo> p2(std::piecewise_construct, t, t);
	

	std::pair < std::string, std::vector<int> >
		foo(
			std::piecewise_construct,
			std::forward_as_tuple("test"),
			std::forward_as_tuple(3, 10)
		);
	std::cout << "foo.first: " << foo.first << '\n';
	std::cout << "foo.second:";
	for (int& x : foo.second) std::cout << ' ' << x;
	std::cout << '\n';


}