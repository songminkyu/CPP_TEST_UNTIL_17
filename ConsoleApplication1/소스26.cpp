#include "stdafx.h"
#include <iostream>
#include <iterator>
#include <regex>
#include <string>

int main()
{
	std::string text = "Quick brow fox";
	std::regex vowel_re("a|e|i|o|u");

	std::regex_replace(std::ostream_iterator<char>(std::cout),
		text.begin(), text.end(), vowel_re, "*");

	std::cout << '\n' << std::regex_replace(text, vowel_re, "[$&]") << std::endl;
}