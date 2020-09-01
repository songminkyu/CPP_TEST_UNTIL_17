#include <iostream>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <vector>
#include <string>

using var_t = boost::variant<int, const char*, std::string>;

int main() {
	std::vector<var_t> values;
	values.push_back(10);
	values.push_back("Hellow");
	values.push_back(std::string("magic"));

	for (auto && item : values) {
		switch (item.which()) {
		case 0:
			std::cout << "int:  " << item << std::endl;
			break;

		case 1: 
			std::cout << "const char*: " << item << std::endl;
			break;

		case 2: 
			std::cout << "std::string:  " << item << std::endl;
			break;
		}
	}
	return 0;
}