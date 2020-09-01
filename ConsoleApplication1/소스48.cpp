#include "stdafx.h"
#include <iostream>

struct A {
	int val;
	A(int val) : val(val){}
	void test() & { std::cout << "test & " << val << std::endl; }
	void test() && { std::cout << "test && " << val << std::endl; }
};

int main()
{
	bool what_call{ false };

	A a{ 10 };
	/*****************   test 1 *****************************/	
	(what_call ? a : A{ 500 }).test();
	what_call = true;
	(what_call ? a : A{ 500 }).test();
	/*****************   test 1 *****************************/

	std::cout << "==========================================" << std::endl;

	/*****************   test 2 *****************************/

	a.test();
	A{ 500 }.test();
	/*****************   test 2 *****************************/
}