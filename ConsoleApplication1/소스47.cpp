#include "stdafx.h"
#include <iostream>
#include <utility>

template<std::size_t INDEX, typename T, std::size_t N>
T& get(T(&t)[N]) {
	return t[INDEX];
}

int main() {
	int a[] = { 1,2,3,4 };
	std::cout << get<0>(a) << std::endl;
	get<0>(a) = 100;
	std::cout << get<0>(a) << std::endl;
}