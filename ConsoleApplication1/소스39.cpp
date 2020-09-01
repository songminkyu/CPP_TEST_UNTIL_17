#include "stdafx.h"
#include <iostream>
#include <algorithm>

template<typename ...T>
struct Alighedunion {
	alignas(T...) unsigned char data[std::max(sizeof(T)...)];
};

int main()
{
	Alighedunion <int, char> au;

	au.data[0] = 0;
	au.data[1] = 1;
	au.data[2] = 2;
	au.data[3] = 3;

	for (const auto & i : au.data)
	{
		std::cout <<i << std::endl;
	}

	

}