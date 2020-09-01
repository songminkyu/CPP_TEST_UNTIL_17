#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

#define QUERYSORTING (1)

namespace client
{
	namespace qi = boost::spirit::qi;

#if QUERYSORTING == 0
	using pairs_type = std::map<std::string, std::string>;
#else
	using pairs_type = std::vector<std::pair<std::string, std::string>>;
#endif

	template< typename Iterator >
	struct query_parser : qi::grammar<Iterator, pairs_type()> {
		query_parser() : query_parser::base_type(query) {

			key = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
			value = +qi::char_("a-zA-Z_0-9");
			pair = key >> -('=' >> value);

			query = pair >> *((qi::lit(';') | '&') >> pair);
		}
	private:
		qi::rule<Iterator, std::string()> key;
		qi::rule<Iterator, std::string()> value;
	    qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
		qi::rule<Iterator, pairs_type()> query;
	};
	template<typename Iterator>
	bool parse_process(Iterator first ,Iterator last, pairs_type & value) {
		if (first == last) return false;
		client::query_parser<Iterator> q_parser;
		parse(first, last, q_parser, value); // parse 스페이스 없이 연이어 파싱 할떄 사용 , 파싱 된 결과는 value 저장된다.
		if (first != last) return false;
	};
}
int main() {
	std::string cmd;
	while (getline(std::cin, cmd))
	{
		if (cmd.empty() || cmd[0] == 'q' || cmd[0] == 'Q')	break;
		
		client::pairs_type value;
		if (client::parse_process(std::begin(cmd), std::end(cmd), value))
		{
			for (const auto & mitem : value) {
				std::cout << '[' << mitem.first << ":" << mitem.second << "]" << std::endl;
			}

			std::cout << std::endl << "Parsing OK" << std::endl;
		}
		else
		{
			std::cout << std::endl << "Parsing FAIL" << std::endl;
		}
	}
	return 0;
}