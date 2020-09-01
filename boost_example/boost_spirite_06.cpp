#include <iostream>
#include <string>
#include <boost\config\warning_disable.hpp>
#include <boost\spirit\home\x3.hpp>

int main() {
	using namespace boost::spirit::x3;

	std::string input("pizza");
	std::string str_re;

	parse(std::begin(input), std::end(input), *char_, str_re);

	std::cout << str_re << std::endl;

	std::cout << input << std::endl;

}