#include "stdafx.h"
#include <iostream>


#define MAIN_ACTIVATION 0

struct //타입없는 구조체
{
	int a_;
}ab;
//ab 변수를 빼면 타입이다.

#if(MAIN_ACTIVATION)
int main()
{
	using ZT = decltype(ab);
	using test_typ = int(int, int); 

	int a[40];
	using a_type = int[40];
	a_type &b = a;
	a_type * ab = &a;
}
#endif