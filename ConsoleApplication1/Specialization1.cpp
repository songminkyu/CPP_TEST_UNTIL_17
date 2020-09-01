#include "stdafx.h"
#include <iostream>

namespace NS {
	inline namespace detail {
		template <typename T>
		void test();
	}

	template<>
	void test<int>() {
		std::cout << "test<int>" << std::endl;
	}

	template<>
	void test<double>() {
		std::cout << "test<double>" << std::endl;
	}

}

int main()
{
	using namespace NS;
	test<int>();
	test<double>();

}