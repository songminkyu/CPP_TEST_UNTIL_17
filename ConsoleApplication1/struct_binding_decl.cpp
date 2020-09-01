#include "stdafx.h"
#include <iostream>

template <typename T, typename U>
bool all_equal(T && t, U&& u)
{
	return (t == u);
}

template<typename T, typename U, typename ...Ts>
bool all_equal(T && t, U&& u, Ts&&... args)
{
	return (t == u) && all_equal(u, std::forward<Ts>(args)...);
}

int main()
{
	//sturctred binding declaration의 3가지 방법
	int ar[] = { 1,2 };
	auto[x0, y0] = ar;
	auto[x1, y1](ar);
	auto[x2, y2]{ar};
	std::cout << all_equal(x0, x1, x2) << std::endl;
	std::cout << all_equal(y0, y1, y2) << std::endl;

}