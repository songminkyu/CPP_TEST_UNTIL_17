#include <iostream>
#include <list>
#include <boost\any.hpp>
#include <string>

using many = std::list<boost::any>;

int main() {

	many values;

	values.push_back(10);
	values.push_back("test");
	values.push_back(std::string("string test"));

	for (auto && item : values) {
		if (item.type() == typeid(int)) {
			std::cout << boost::any_cast<int&>(item) << std::endl;
		}
		if (item.type() == typeid(const char*)) {
			std::cout << boost::any_cast<const char*&>(item) << std::endl;
		}
		if (item.type() == typeid(std::string)) {
			std::cout << boost::any_cast<std::string&>(item) << std::endl;
		}
	}
	return 0;
}