
#include <boost/config/warning_disable.hpp>

#include <iostream>
#include <string>
#include <iterator>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

namespace client
{
	template<typename OutputIterator>
	bool classify_character(OutputIterator sink, char c) {
		
		namespace ascii = boost::spirit::ascii;
		namespace sprite = boost::spirit;
		namespace karma = sprite::karma;
		
		using karma::generate;

		using ascii::char_;//문자
		using ascii::digit;//숫자
		using ascii::xdigit;//16진수에 사용되는 0x0A ~ 0x0F
		using ascii::alpha;//알파벳
		using ascii::punct;//구두점
		using ascii::space;//스페이스
		using ascii::cntrl;//컨트롤 문자

		if (!sprite::char_encoding::ascii::isascii_(c))
		{
			return false;
		}

		return generate(
			sink,
			(
				"The character'" << char_ << "'is"
				<<
				(
					&digit << "a digit" // 숫자면 출력
					| &xdigit << "a xdigit" // A~F 핵사 면 출력
					| &alpha << "a alpha" // 알파벳인가?
					| &punct << "a punct" // 구두점인가?
					| &space << "a space" // 스페이스인가
					| &cntrl << "a cntrl" //컨트롤 문자
					|"of unknown type" // 이도저도 아닌 타입
				)
				<< "\n"
			),
			c, c
		);
	}
}

int main()
{
	std::string str;
	while (getline(std::cin, str)) {
		if (str.empty()) break;
		std::string gen;
		std::back_insert_iterator<std::string> sink(gen);

		for (auto & chr : str) {
			client::classify_character(sink, chr);
		}

		std::cout << gen << std::endl;
	}
	return 0;
}