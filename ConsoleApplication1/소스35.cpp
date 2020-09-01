#include "stdafx.h"
#include <iostream>

//aggregate initialization 구현
//C++ 17부터 가능
struct base1
{
	int a, b; // #1
};

struct base2 {
	int c, d; // #2
};

struct derived : base1, base2
{
	int e, f; // #3
};

int main()
{
	//most derived class 가 aggregate type 이면 aggregate initialization 를 사용 할수 있고
	//direct base class 가 생성자가 없다면 member 순으로 aggregate initialization를 사용 할수 있다.
	// indirect base class는  aggregate initialization 대상이 아니다.
	// 따라서 #1,#2,#3  순으로 aggregate 할 수 있다.
	
	derived d1{ {1,2},{3,4},5,6 };

	std::cout << d1.a << std::endl;
	std::cout << d1.b << std::endl;
	std::cout << d1.c << std::endl;
	std::cout << d1.d << std::endl;
	std::cout << d1.e << std::endl;
	std::cout << d1.f << std::endl;

}