#include "stdafx.h"
#include <iostream>
#include <string>

//람다 함수 상속
// - 람도 함수도 다른 함수도 다른 함수 처럼 overload를 지원 할 수 있는가?
// - 제한적이지만 구현 할수 있다. 일반화된 방법은 표준에서 해결 할듯.

template<class ...FN>
struct my_overloaded : FN...
{
	my_overloaded(FN... fn) : FN(fn)... {}
};

template<class ...FN>
my_overloaded<FN...> my_overload(FN... f)
{
	return my_overloaded<FN...>(f...);
}

int main()
{
	auto visitor = my_overload(
		[](int i, int j) {std::cout << "#1 overload" << std::endl;},
		[](int i, std::string const & j) {std::cout << "#2 overload" << std::endl;},
		[](auto const &i, auto const  &j) {std::cout << "#3 overload" << std::endl;}
	);

	visitor(1, std::string{ "2" });
	visitor(1.2f, .08f);
	return 0;
}