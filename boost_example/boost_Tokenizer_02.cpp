#include <iostream>
#include <string>
#include <boost\tokenizer.hpp>

int main() {
	std::string str = "Hellow,, world, foo, bar, yow, baz,";
	using tokenizer = boost::tokenizer<boost::char_separator<char >>;

	boost::char_separator<char> sep(",", nullptr, boost::drop_empty_tokens);
	tokenizer tokens(str, sep);

	for (auto && mitem : tokens) {
		std::cout << mitem << std::endl;
	}

	return 0;
}