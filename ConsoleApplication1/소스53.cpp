#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
	std::vector<int> vec{ 0, 5, 2, 9, 7, 6, 1, 3, 4, 8 };

	size_t compCounter = 0;
	std::sort(vec.begin(), vec.end(), [&compCounter](int a, int b) {
		++compCounter;
		return a < b;
		});

	std::cout << "number of comparisons: " << compCounter << '\n';

	for (auto& v : vec)
		std::cout << v << ", ";
}