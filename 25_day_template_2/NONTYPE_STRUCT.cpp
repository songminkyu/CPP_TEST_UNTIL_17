#include "stdafx.h"
#include <iostream>


#define MAIN_ACTIVATION 0

struct //Ÿ�Ծ��� ����ü
{
	int a_;
}ab;
//ab ������ ���� Ÿ���̴�.

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