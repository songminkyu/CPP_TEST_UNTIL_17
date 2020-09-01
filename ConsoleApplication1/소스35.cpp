#include "stdafx.h"
#include <iostream>

//aggregate initialization ����
//C++ 17���� ����
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
	//most derived class �� aggregate type �̸� aggregate initialization �� ��� �Ҽ� �ְ�
	//direct base class �� �����ڰ� ���ٸ� member ������ aggregate initialization�� ��� �Ҽ� �ִ�.
	// indirect base class��  aggregate initialization ����� �ƴϴ�.
	// ���� #1,#2,#3  ������ aggregate �� �� �ִ�.
	
	derived d1{ {1,2},{3,4},5,6 };

	std::cout << d1.a << std::endl;
	std::cout << d1.b << std::endl;
	std::cout << d1.c << std::endl;
	std::cout << d1.d << std::endl;
	std::cout << d1.e << std::endl;
	std::cout << d1.f << std::endl;

}