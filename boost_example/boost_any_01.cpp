#include <iostream>
#include <list>
#include <boost\any.hpp>
#include <string>

using namespace boost;
using namespace std::literals;

int main() {
	std::cout << std::boolalpha;

	any a = 42;

	std::cout << any_cast<int>(a) << std::endl;

	auto aslong = any_cast<long>(&a);
	std::cout << (nullptr == aslong) << std::endl;

	try {
		any_cast<bool>(a);
	}
	catch (bad_any_cast &e) {
		std::cout << e.what() << std::endl;
	}

	a = "Hellow"s;
	std::cout << a.type().name() << std::endl;

	any a2 = a;
	std::cout << any_cast<std::string>(a2) << std::endl;

	auto& s1 = any_cast<std::string&>(a);
	s1 = "bye";
	std::cout << any_cast<std::string>(a) << std::endl;
	std::cout << any_cast<std::string>(a2) << std::endl;

	return 0;
}