#include <iostream>
#include <string>
#include <sstream>
#include <boost\optional\optional_io.hpp>

boost::optional<int> convert(std::string text) {
	std::stringstream ss(text);

	int i;
	ss >> i;
	if (ss && ss.eof())	return i;
	return boost::none;
}

int main() {
	auto num_1 = convert("1000");

	if (num_1) {
		std::cout << "data good : " << num_1.value() << std::endl;
		std::cout << num_1.get() << std::endl;
	}
	else {
		std::cout << "input bug : " << std::endl;
	}

	auto num_2 = convert("hellow");

	if (num_2) {
		std::cout << "data good : " << num_2.value() <<std::endl;
		std::cout << num_2.get() << std::endl;
	}
	else {
		std::cout << "input bug " << std::endl;
		
	}

	return 0;
}