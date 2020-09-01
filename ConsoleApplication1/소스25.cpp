#include "stdafx.h"
#include <iterator>
#include <iostream>
#include <string>
#include <regex>
namespace std {
	std::sregex_iterator &begin(std::sregex_iterator &iter) { return iter; }
	std::sregex_iterator end(std::sregex_iterator &iter) { return {}; }

	std::sregex_token_iterator &begin(std::sregex_token_iterator &iter) { return iter; }
	std::sregex_token_iterator end(std::sregex_token_iterator &iter) { return {}; }

}

void regex_test_1()
{
	const std::string s = "Quick Brow fox.";

	std::regex word_regex("[^\\s]+");

	auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);

	for (auto && item : words_begin)
	{
		std::cout << item.str() << std::endl;
	}
}

void regex_test_2()
{
	std::string html = "<p><a href=\"http://google.com\">google</a>"
		"<a HREF = \"http://cppreference.com\">cppreference</a>\n</p>";

	std::regex url_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", std::regex::icase);

	auto words_begin = std::sregex_token_iterator(html.begin(), html.end(), url_re, 1);

	for (auto && item : words_begin)
	{
		std::cout << item.str() << std::endl;
	}
}

int main()
{
	std::cout << "### 정규식 테스트 1" << std::endl;
	regex_test_1();

	std::cout << "### 정규식 테스트 2" << std::endl;
	regex_test_2();
}