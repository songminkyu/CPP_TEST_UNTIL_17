#include "stdafx.h"
#include <tuple>
#include <iostream>

/*
내가 만든 클래스에 structured binding 기능 지원하기
내부 구현은 메타 프로그래밍 세상~
*/
struct A {
protected:
	int a = 1;
	int b = 2;
};

struct MyStruct : A {
protected:
	std::string c = "songminkyu";
	int d = 4;
public:
	template <std::size_t N>
	decltype(auto) get() const {
		if constexpr (N == 0) return a;
		else if constexpr (N == 1) return b;
		else if constexpr (N == 2) return c;
		else if constexpr (N == 3) return d;
	}
};

namespace std {
	template<>
	struct tuple_size<MyStruct> : std::integral_constant<std::size_t, 4> {};

	template <std::size_t N>
	struct tuple_element<N, MyStruct> {
		using type = decltype(std::declval<MyStruct>().get<N>());
	};
}

int main()
{
	MyStruct z{};
	auto[a, b, c, d] = z;
	std::cout << z.get<0>() << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
	std::cout << d << std::endl;
}
