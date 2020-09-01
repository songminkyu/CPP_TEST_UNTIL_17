#include "stdafx.h"
#include <iostream>
#include <array>

template<typename Range, typename Func, typename T>
constexpr T SimpleAccumulate(const Range& range, Func func, T init) {
	for (auto &&elem : range) {
		init += func(elem);
	}	
	return init;
}

int main() {
	constexpr std::array arr{ 1, 2, 3 };

	auto r = SimpleAccumulate(arr, [](int i) {
		return i * i;
	}, 0);

	std::cout << r << std::endl;
	static_assert(SimpleAccumulate(arr, [](int i) {
		return i * i;
		}, 0) == 14);
}