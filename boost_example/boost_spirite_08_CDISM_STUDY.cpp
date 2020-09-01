#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>


// Qi Sample 2

namespace client {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	//char 를 unsigned 로 매핑한다는 의미로 사용
	//이 기술 용어는 symbol table 이라 한다.
	struct ones_ : qi::symbols<char, unsigned> { // first::charactor type => second::attribute
		ones_() {
			this->add
			("I", 1)
			("II", 2)
			("III", 3)
			("IV", 4)
			("V", 5)
			("VI", 6)
			("VII", 7)
			("VIII", 7)
			("IX", 9);
		}
	}ones;

	struct tens_ : qi::symbols<char, unsigned> { // first::charactor type => second::attribute
		tens_() {
			add
			("X", 10)
			("XX", 20)
			("XXX", 30)
			("XL", 40)
			("L", 50)
			("LX", 60)
			("LXX", 70)
			("LXXX", 80)
			("XC", 90);
		}
	}tens;

	struct hundreds_ : qi::symbols<char, unsigned> { // first::charactor type => second::attribute
		hundreds_() {
			add
			("C", 100)
			("CC", 200)
			("CCC", 300)
			("CD", 400)
			("D", 500)
			("DC", 600)
			("DCC", 700)
			("DCCC", 800)
			("CM", 900);
		}
	}hundreds;

	template<typename iterator>
	struct roman : qi::grammar<iterator, unsigned()>//container parser // composite design pattern
	{
		roman() : roman::base_type(start, "roman grammer")
		{
			using qi::eps;
			using qi::lit;
			using qi::_val;
			/*
			_val is another Phoenix placeholder representing the rule's synthesized attribute
			_r1 ...r10 These Phoenix placeholder refer to the Nonterminal's inherited attributes.
			*/
			using qi::_1;

			start = eps[_val = 0] >>
			(
			lit('M')[_val += 1000] ||
			hundreds[_val += _1] ||
			tens[_val += _1] ||
			ones[_val += _1]
			);		
		}

		qi::rule<iterator, unsigned()> start;

	};

	template<typename iterator>
	bool parse_process(iterator first, iterator last) {
		if (first == last)	return false;
		using qi::lit;
		using qi::phrase_parse;
		using qi::_1;
		using qi::eps;//parser => 파싱 하기전에 초기화 하고 파싱 끝나고 나서도 초기화에 사용한다. 수학 입실론에 의미를 부여한것.
		using ascii::space;
		using phoenix::ref;
		auto skip_parser = space;

		// #1
		unsigned int val1 = 0;
		auto today_parse1 = eps[phoenix::ref(val1) = 0] >> *( 
			                ones[phoenix::ref(val1) += _1] || 
			                tens[phoenix::ref(val1) += _1] ||
			                hundreds[phoenix::ref(val1) += _1] ||
			                lit("M")[phoenix::ref(val1) += 1000]);
		auto last1 = [&]() {std::cout << val1 << std::endl; };

		// #2
		unsigned int val2 = 0;
		roman<iterator> roman_parser2;
		auto today_parser2 = *(roman_parser2[phoenix::ref(val2) += _1]);
		auto last2 = [&]() {std::cout << val2 << std::endl; };

		// #3
		std::vector<unsigned int> datas;
		roman<iterator> roman_parser3;
		auto last3 = [&]() {
			std::cout << "data list : " << std::endl;
			for (auto && mitem : datas) {
				std::cout << mitem << std::endl;
			}
		};

		bool result = phrase_parse(
			first, last,
			*roman_parser3,
			skip_parser,
			datas
		);

		if (first != last) return false; //partial match deny
		last3();
		return true;                     // full match
	}
}

int main()
{
	std::string cmd;
	while (getline(std::cin, cmd))
	{
		if (cmd.empty() || cmd[0] == 'q' || cmd[0] == 'Q')	break;

		if (client::parse_process(std::begin(cmd), std::end(cmd)))
		{
			std::cout << std::endl << "Parsing OK" << std::endl;
		}
		else
		{
			std::cout << std::endl << "Parsing FAIL" << std::endl;
		}
	}
	return 0;

}