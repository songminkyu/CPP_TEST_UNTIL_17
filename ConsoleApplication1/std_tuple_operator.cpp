#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <string>
//int main()
//{
//
//http://en.cppreference.com/w/cpp/utility/tuple/operator_cmp
//	std::vector<std::tuple<int, std::string, float>> v;
//	v.emplace_back(2, "baz", -0.1);
//	v.emplace_back(2, "bar", 3.14);
//	v.emplace_back(1, "foo", 100.1);
//	std::sort(v.begin(), v.end());
//
//
//	int A = NULL;
//	std::string B;
//	float C = NULL;
//
//
//	for (auto p : v) {
//		A = std::get<0>(p);
//		B = std::get<1>(p);
//		C = std::get<2>(p);
//		std::cout << "(" << A << ", " << B
//			<< ", " << C << ")\n";
//	}
//}