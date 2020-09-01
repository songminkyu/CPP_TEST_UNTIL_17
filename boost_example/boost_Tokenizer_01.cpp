#include <iostream>
#include <string>
#include <boost\tokenizer.hpp>

//<><><Hellow><|><world><><|><foo><><bar><yow><baz><|><>

int main() {
	std::string str = ";;Hello|wWorld||-foo--bar;yow;baz|";

	using tokenizer = boost::tokenizer<boost::char_separator<char>>;

	boost::char_separator<char> sep("-;", "|", boost::keep_empty_tokens);

	tokenizer tokens(str, sep);

	for (auto && mitem : tokens) {
		std::cout << mitem << std::endl;
	}

	return 0;
}