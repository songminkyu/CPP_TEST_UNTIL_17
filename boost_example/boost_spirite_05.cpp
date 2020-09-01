#include <iostream>
#include <string>
#include <boost\config\warning_disable.hpp>
#include <boost\spirit\home\x3.hpp>

int main() {
	using namespace boost::spirit::x3;

	std::string input("1234");

	int val;

	parse(std::begin(input), std::end(input), int_, val);

	std::cout << val << std::endl;

}