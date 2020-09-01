#include "stdafx.h"
#include <iostream>
#include <memory>
/*
C++17 이후에는 new-extended alignment를 지원하도록 allocation/deallocation function이 추가됨
(별도 외부 allocation(예를 들어 jemalloc)를 사용하는 사용자는 당분간 조심스럽게 접근할 필요가 있음. 
특히 replace 방식으로 사용하는 유저는 업데이트를 기달리거나 별도 함수를 추가해야 함).

(__STDCPP_DEFAULT_NEW_ALIGNMENT__를 초과하는 바이트의 alignment를 new-extended alignment라고 함. 
new 연산자에서 확장된 alignment로 판단함.)

이제 특정 정렬에서 가속 기능을 사용하는 요구하는 장치와 연동할 수 있다.

*/

struct A {
	alignas(128) int value;
};

template <typename T>
bool is_aligment(T* p)
{
	return (reinterpret_cast<int>(p) % alignof(T) == 0);
}

int main()
{
	{
		delete (new A);
	}

	A b1[10];
	std::cout << "align : " << alignof(A) << std::endl;

	for (auto && p : b1)
	{
		std::cout << is_aligment(&p) << std::endl;
	}

	alignas(2) unsigned int  i;

	i = 0x1234567891212233;

	std::cout << i << std::endl;


}