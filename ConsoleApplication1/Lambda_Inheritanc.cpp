#include "stdafx.h"
#include <iostream>
#include <string>

//���� �Լ� ���
// - ���� �Լ��� �ٸ� �Լ��� �ٸ� �Լ� ó�� overload�� ���� �� �� �ִ°�?
// - ������������ ���� �Ҽ� �ִ�. �Ϲ�ȭ�� ����� ǥ�ؿ��� �ذ� �ҵ�.

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