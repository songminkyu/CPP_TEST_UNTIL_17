

/*
Multicharacter literal를 이런 식으로 활용할 수 있겠군~.

C++에서 Multicharacter literal는 int 타입이고 
implementation-defined value를 갖는다.
*/
#include "stdafx.h"
#include <iostream>
#include <string>

union  A
{
	int all = 'jpg';
	char ext[4];
};

void test1()
{
	std::cout << "========Test 1=======" << std::endl;
	A a;
	for (auto && item : a.ext) {
		std::cout << item << std::endl;
	}
}


enum FCC {
	JPG = 'JPG',
	BMP = 'BMP'
};
void test2()
{
	FCC fcc = BMP;

	switch (fcc) {
	case 'JPG' : 
		std::cout << "JPG" << std::endl;
		break;
	case 'BMP' : 
		std::cout << "BMP" << std::endl;
		break;
	}
}
void main()
{
	test1();
	test2();
}