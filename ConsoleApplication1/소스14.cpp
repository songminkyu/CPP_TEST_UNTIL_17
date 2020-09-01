// ConsoleApplication13.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "stdafx.h"
#include <iostream>
#include <valarray>

void test1()
{
	std::valarray<int> data = { 0,1,2,3,4,5,6,7,8,9,10 };
	data[data > 5] = -1;
	for (auto && n : data)
	{
		std::cout << n << std::endl;
	}
}
void test2()
{
	std::valarray<int> data = { 0,1,2,3,4,5,6,7,8,9,10 };
	data[std::slice(0, 3, 2)] = 77;
	for (auto && n : data)
	{
		std::cout << n << std::endl;
	}
}

void test3()
{
	std::valarray data1 = { "songminkyu",10 };
	std::valarray data2 = { "MINKYU",5 };
	data1[std::slice(1, 5, 2)] = data2;
	for (auto && n : data1)
	{
		std::cout << n << std::endl;
	}
}
void test4()
{
	std::valarray<int> data = { 0,1,2,3,4,5,6,7,8,9,10 };

	for (auto && n : data.apply([](int n) {return n * 100; }))
	{
		std::cout << n << std::endl;
	}
}

void test5()
{
	std::valarray<int> data = { 0,1,2,3,4,5,6,7,8,9,10 };
	
	std::cout << "min :" << data.min() << ", " << "max :" << data.max() << std::endl;

}


int main() {
	test5();
}
