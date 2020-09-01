#include <iostream>
#include <boost\multiprecision\cpp_int.hpp>

using namespace boost::multiprecision;
int main() {

	int128_t v = 1;

	for (unsigned int i = 1; i < 20; ++i) {
		v *= i;
	}
	std::cout << v << std::endl;

	std::cout << "**********************************" << std::endl;
	cpp_int u = 1;

	for (unsigned int i = 1; i <= 1000; i++)
	{
		u *= i;
	}
	std::cout << u << std::endl;
}