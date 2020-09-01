#include <iostream>
#include <map>
#include "stdafx.h"
#include <vector>
#include <string>
#include <algorithm>

void foo(int x) { std::cout << x << std::endl; }
void foo(float x) { std::cout << x << std::endl; }

#define LIFT(foo) \
  [](auto&&... x) \
    noexcept(noexcept(foo(std::forward<decltype(x)>(x)...))) \
   -> decltype(foo(std::forward<decltype(x)>(x)...)) \
  { return foo(std::forward<decltype(x)>(x)...); }

int main()
{
	std::vector<int> vi;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	vi.push_back(4);
	vi.push_back(5);
	//std::for_each(vi.begin(), vi.end(), foo); // doesn't compile

	// very simple approach:
	std::for_each(vi.begin(), vi.end(), [](auto x) { return foo(x); });

	// full LIFT:
	std::for_each(vi.begin(), vi.end(), LIFT(foo));

}