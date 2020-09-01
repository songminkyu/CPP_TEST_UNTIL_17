#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "60_stack4test.h"


#define MAIN_ACTIVATION 0
#define Implementation_IN_Main 0
/*

1.type ==> typename T
2.non-type ==> int N
3.template => template<typename T> class ZZ;  여기에 타입네임 쓰면 오류다; 17스펙에는 타입네임 쓰도록 허용될 예정
4. template<class T, int MAXSIZE = 20>   타입 파라미터


*/

constexpr int sum(int a, int b)
{
	return a+b;
}

#if(MAIN_ACTIVATION)
int main()
{
	try { // 런타임시간에 생성

#if(Implementation_IN_Main)
		int z = 50;
		const int m = z;
		Stack<int, m>         int40Stack;   //문제 발생 - 런타임에 만들어지니 다른곳에 사용 불가.		
#else
		const int z = 30;
		Stack<int, sum(z,20)>         int20Stack;     // stack of up to 20 ints
		//가능한 문법

		Stack<int, 40>         int40Stack;     // stack of up to 40 ints
		Stack<std::string, 40> stringStack;    // stack of up to 40 strings
#endif										   // manipulate stack of up to 20 ints
		int20Stack.push(7);
		std::cout << int20Stack.top() << std::endl;
		int20Stack.pop();

		// manipulate stack of up to 40 strings
		stringStack.push("hello");
		std::cout << stringStack.top() << std::endl;
		stringStack.pop();
		stringStack.pop();
	}
	catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;  // exit program with ERROR status
	}
}
#endif