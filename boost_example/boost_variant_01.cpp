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
		if (typeid(std::string) == item.type()) {
			auto val = boost::get<std::string>(item);
			std::cout << "std::string :  " << val << std::endl;
			continue;
		}
		else if (typeid(const char*) == item.type()) {
			const char* val = boost::get<const char*>(item);
			std::cout << "const char*:  " << val << std::endl;
			continue;
		}
		else
		{
			auto val = boost::get<int>(item);
			std::cout << "int:  " << val << std::endl;
			continue;
		}
	}
	return 0;
}