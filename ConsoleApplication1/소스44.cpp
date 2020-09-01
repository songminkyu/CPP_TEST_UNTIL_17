#include "stdafx.h"
#include <iostream>
#include <type_traits>
#include <boost/type_index.hpp>

//Ÿ���� ������~ declarations�� �ٸ� �� �ִ�

using boost::typeindex::type_id_with_cvr;

int fn1(const int a) {
	std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << std::endl;
}
int fn2(int a) {
	std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << std::endl;
}

int main() {
	std::cout << std::is_same_v<decltype(fn1), decltype(fn2)> << std::endl;
	fn1(3);
	fn2(3);
}