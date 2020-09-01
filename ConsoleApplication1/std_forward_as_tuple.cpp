#include "stdafx.h"
#include <iostream>
#include <map>
#include <tuple>
#include <string>
//
//void main()
//{
//http://en.cppreference.com/w/cpp/utility/tuple/forward_as_tuple
//	std::map<int, std::string> m;
//
//	m.emplace(std::piecewise_construct,
//		std::forward_as_tuple(100),
//		std::forward_as_tuple(20, 'a'));
//	std::cout << "m[100] = " << m[100] << '\n';
//
//	// The following is an error: it produces a
//	// std::tuple<int&&, char&&> holding two dangling references.
//	//
//	// auto t = std::forward_as_tuple(20, 'a');
//	// m.emplace(std::piecewise_construct, std::forward_as_tuple(10), t);
//}
