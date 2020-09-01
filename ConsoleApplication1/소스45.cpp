#include "stdafx.h"
#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

void h1(int x()) {
	std::cout << type_id_with_cvr<decltype(x)>().pretty_name() << std::endl;
	std::cout << type_id_with_cvr<decltype(h1)>().pretty_name() << std::endl;
}

void h2(int x[3]) {
	std::cout << type_id_with_cvr<decltype(x)>().pretty_name() << std::endl;
	std::cout << type_id_with_cvr<decltype(h2)>().pretty_name() << std::endl;
}
void h3(int* const x) {
	std::cout << type_id_with_cvr<decltype(x)>().pretty_name() << std::endl;
	std::cout << type_id_with_cvr<decltype(h3)>().pretty_name() << std::endl;
}
int fn() { return 10; }

void main()
{
	h1(fn);

	int xx[] = { 1,2,3 };
	h2(xx);

	int x = 3;
	h3(&x);
}