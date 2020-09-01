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
		std::cout << "우리 unique_ptr 물건을 훔치다니!!! \n";
		std::cout << "욕심많은 shared_ptr !! \n";
	}
}
