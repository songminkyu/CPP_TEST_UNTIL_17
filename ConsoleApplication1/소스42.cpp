

/*
Multicharacter literal�� �̷� ������ Ȱ���� �� �ְڱ�~.

C++���� Multicharacter literal�� int Ÿ���̰� 
implementation-defined value�� ���´�.
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