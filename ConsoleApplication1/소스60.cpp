#include "stdafx.h"
#include <iostream>

int main()
{

	constexpr auto Square = [](int n) { return n * n; }; // implicitly constexpr
	static_assert(Square(2) == 4);

}