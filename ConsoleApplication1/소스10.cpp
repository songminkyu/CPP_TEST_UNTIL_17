#include "stdafx.h"
#include <iostream>
#include <memory>

struct A {
	A() = default;
	~A() { std::cout << "~A" << std::endl; }
};

int main()
{
	auto _unique_ptr = std::make_unique<A>();
	auto s_ptr = std::shared_ptr<A>(std::move(_unique_ptr));

	if (!_unique_ptr)
	{
		std::cout << "�츮 unique_ptr ������ ��ġ�ٴ�!!! \n";
		std::cout << "��ɸ��� shared_ptr !! \n";
	}
}
