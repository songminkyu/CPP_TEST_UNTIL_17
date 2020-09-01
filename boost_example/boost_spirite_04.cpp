#include <iostream>
#include <string>
#include <boost\config\warning_disable.hpp>
#include <boost\spirit\home\x3.hpp>

int main() {
	using namespace boost::spirit::x3;

	std::string input(
		"foo : bar , "
		"grop : smart , "
		"falcou : \"crazy freencman\" , "
		"name : sam "
	);

	auto name = alpha >> *alnum;
	auto quote = '"' >> lexeme[*(~char_('"'))] >> '"';
	
	phrase_parse(std::begin(input), std::end(input), (name >> ':' >> (quote | name)) % ',', space);

	// 이건 쓰잘대기 없는거같음..
	return 0;
}