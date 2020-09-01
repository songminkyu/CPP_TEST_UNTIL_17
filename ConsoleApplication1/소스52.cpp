#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>

int foo()
{
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	const auto val = std::count_if(v.begin(), v.end(),
		[](int v) { return v > 2 && v <= 6; });

	return val;
}
int main()
{
	std::cout << foo() << std::endl;
	return 0;
}