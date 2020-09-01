#include "stdafx.h"
#include <iostream>
#include <string>
//

#define MAIN_ACTIVATION 0

//Base specifiers and member initializer lists
//���ٴ� ���� ������ ���������� ������ ������ �Ұ����ϴ�.
template<typename...T>
struct A : public T...
{
	A(T&...t) : T(t)... {}
};

#if(MAIN_ACTIVATION)
int main()
{
	auto fn_1 = [](int a, int b) {return (a + b);}; //operator call
	auto fn_2 = [](int a, int b, int c) {return (a + b + c);}; //operator call
	A<decltype(fn_1), decltype(fn_2)> m(fn_1, fn_2);
	//���� �������̵�.
	//�������� �����ϱ⿡ ����.
	std::cout << m(10, 20) << std::endl;
	std::cout << m(10, 20, 30) << std::endl;
}
#endif