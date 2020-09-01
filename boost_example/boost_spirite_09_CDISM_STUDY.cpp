#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

// Qi Sample 3

namespace client {
	struct employee {
		int             age;
		std::string     surname;
		std::string     forename;
		double	        salary;
	};
}
namespace client {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	template<typename iterator>
	struct employee_parser : qi::grammar<iterator, employee(), ascii::space_type>
	{
		employee_parser() : employee_parser::base_type(entry)
		{
			using qi::int_;
			using qi::double_;
			using ascii::char_;
			using qi::lit;
			using qi::lexeme;
			// 1. (char_-'"') 는 문자열 가운데 " 를 제거하기위해 사용.
			// 2. lexeme['"' >> + (char_-'"') >> '"'] 합성타입이 quoted_string 저장하게된다.
			quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];
			// % ???
			// quoted_string %= lexeme['"' >> + (char_-'"') >> '"'][_val = _1]; 합성타입이라고 함.
			// the rule's synthesized attribute.
			// a : A , b : B, c : C ---> ( a >> b >> c ) : fusion::vector<A,B,C>
			// a,b,c 는 따로로 볼수 있지만 fusion::vector 입장에서는 ABC 타입을 묵은 하나의 타입으로 볼수 있음.
			// a : A , b : B ---> (a | b) : variant<A,B>
			entry %=
				lit("employee")
				>> '{'
				>> int_ >> ','
				>> quoted_string >> ','
				>> quoted_string >> ','
				>> double_
				>> '}';
			//employee{12,"song","min",100,0}

		}
		qi::rule<iterator, employee(), ascii::space_type> entry;
		qi::rule<iterator, std::string(), ascii::space_type> quoted_string;
	};

	template<typename iterator>
	bool parse_process(iterator first, iterator last, client::employee& emp)
	{
		if (first == last) return false;

		using qi::phrase_parse;
		using ascii::space;
		employee_parser<iterator> em_parser;
		phrase_parse(first, last, em_parser, space, emp);
		if (first != last) return false;
		return true;
	};
}
BOOST_FUSION_ADAPT_STRUCT(
	client::employee,
	(int, age)
	(std::string, surname)
	(std::string, forename)
	(double, salary)
)
//client::employee ==> fusion::vector<int,std::string, std::string, double>

int main() {
	std::string cmd;
	while (getline(std::cin, cmd))
	{
		if (cmd.empty() || cmd[0] == 'q' || cmd[0] == 'Q')	break;
		client::employee emp;
		if (client::parse_process(std::begin(cmd), std::end(cmd), emp))
		{
			std::cout << boost::fusion::tuple_open('[');
			std::cout << boost::fusion::tuple_close(']');
			std::cout << boost::fusion::tuple_delimiter(',');
			std::cout << "result : " << boost::fusion::as_vector(emp) << std::endl;
			
			std::cout << std::endl << "Parsing OK" << std::endl;
		}
		else
		{
			std::cout << std::endl << "Parsing FAIL" << std::endl;
		}
	}
	return 0;
}