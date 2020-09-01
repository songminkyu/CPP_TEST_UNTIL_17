#include <iostream>
#include <string>
#include <boost\tokenizer.hpp>

int main() {
	std::string s = "This is, a test";

	boost::tokenizer<> tok(s);

	for (auto && str : tok) {
		std::cout << str << std::endl;
	}

	return 0;
}