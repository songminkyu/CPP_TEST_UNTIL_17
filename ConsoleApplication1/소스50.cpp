#include "stdafx.h"
#include <iostream>

int main()
{
	int a[] = { 1,2,3,4 };
	const int(&ca)[4] = a;
	int(&oa)[4] = const_cast<int(&)[4]>(ca);

	for (auto && item : ca) {
		std::cout << item << std::endl;
	}

}

