#include "stdafx.h"
#include <iostream>
#include <memory>
/*
C++17 ���Ŀ��� new-extended alignment�� �����ϵ��� allocation/deallocation function�� �߰���
(���� �ܺ� allocation(���� ��� jemalloc)�� ����ϴ� ����ڴ� ��а� ���ɽ����� ������ �ʿ䰡 ����. 
Ư�� replace ������� ����ϴ� ������ ������Ʈ�� ��޸��ų� ���� �Լ��� �߰��ؾ� ��).

(__STDCPP_DEFAULT_NEW_ALIGNMENT__�� �ʰ��ϴ� ����Ʈ�� alignment�� new-extended alignment��� ��. 
new �����ڿ��� Ȯ��� alignment�� �Ǵ���.)

���� Ư�� ���Ŀ��� ���� ����� ����ϴ� �䱸�ϴ� ��ġ�� ������ �� �ִ�.

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