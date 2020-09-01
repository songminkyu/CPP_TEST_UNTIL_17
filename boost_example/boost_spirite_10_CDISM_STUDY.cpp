#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

// Qi Sample 4

namespace client {
	namespace fusion = boost::fusion;
	namespace phoenix = boost::phoenix;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	struct mini_xml;
	using mini_xml_node = boost::variant<boost::recursive_wrapper<mini_xml>, std::string>;
	struct mini_xml {
		std::string name;
		std::vector<mini_xml_node> children;
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	client::mini_xml,
	(std::string, name)
	(std::vector<client::mini_xml_node>, children)
)
namespace client {
	template<typename Iterator>
	struct mini_xml_grammar : qi::grammar < Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type > {
		mini_xml_grammar()
			: mini_xml_grammar::base_type(xml, "xml") {
			using qi::lexeme;
			using ascii::char_;
			using qi::lit;
			using ascii::string;

			//Lazy evaluated
			using phoenix::val;   //val(1) << 3; //Lazy evaluated
			using phoenix::construct; //There are lazy version of contructor calls

			//error handle
			using qi::on_error;
			using qi::fail;

			//place holder
			using qi::_1;
			using qi::_2;
			using qi::_3;
			using qi::_4;
			using qi::_r1; // _r1 is Phoenix placeholder for the first inherited attribute
			using qi::_a; //_a is Phoenix placeholder for the local variable.

			start_tag %=
				'<'
				>> !lit('/')
				> lexeme[+(char_ - '>')]
				> '>'
				;
			end_tag =
				"</"
				> string(_r1) // _r1 is Phoenix placeholder for the first inherited attribute
				> '>'
				;
			xml %=
				start_tag[_a = _1] //_a is Phoenix placeholder for the local variable.
				> *node
				> end_tag(_a)
				;
			text %= lexeme[+(char_ - '<')];
			node %= xml | text;

			on_error<fail> 
			(
				xml
				,std::cout
				/*<< 5 만약에 정수 5 값을 넣게 된다면 람다가 되는 평가도중에 "5" 값이 출력 된다 주의 필요
				 이유는 늦은 바인딩 즉, 늦은 평가로 인해서 람다가 형성되는데, 정수 5라는 값을 넣은 이유로 
				 평가 규칙이 깨져버림. 그래서 늦은 평가 또는 늦은 바인딩이 되기위해 5 -> Val(5) 대체하여 사용해야함.
				*/
				<< val("Error! Expecting ")
				<< _4   // what
				<< val(" here : \"")
				<< construct<std::string>(_3,_2) //error-pos -> last
				<< val("\"")
				<< std::endl
			);
		}

	private:
		qi::rule<Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type> xml;
		qi::rule<Iterator, mini_xml_node(), ascii::space_type>                     node;
		qi::rule<Iterator, std::string(), ascii::space_type>                       text;
		qi::rule<Iterator, std::string(), ascii::space_type>                       start_tag;
		qi::rule<Iterator, void(std::string), ascii::space_type>                   end_tag;
	};
	template<typename Iterator>
	bool parse_process(Iterator first, Iterator last, client::mini_xml& ast) {
		if (first == last) return false;
		using ascii::space;
		client::mini_xml_grammar<Iterator> xml_parser;
		phrase_parse(first, last, xml_parser, space, ast);
		if (first != last) return false;
	}
	//xml pinrter
	int const tabsize = 4;

	struct mini_xml_printer {
		mini_xml_printer(std::ostream& ostr, int indent = 0)
			: indent(indent), out(ostr)
		{
		}
		void operator()(mini_xml const & xml) const;
	private:
		int indent;
		std::ostream& out;
	};
	struct mini_xml_node_printer : boost::static_visitor<> {
		mini_xml_node_printer(std::ostream & ostr, int indent = 0)
			:indent(indent), out(ostr)
		{

		}

		void operator()(mini_xml const & xml) const {
			mini_xml_printer(out, indent + tabsize)(xml);
		}
		void operator()(std::string const & text) const {
			out << std::string(indent + tabsize, ' ');
			out << "text:\"" << text << '"' << std::endl;

		}
	private:
		int indent;
		std::ostream& out;
	};
	void mini_xml_printer::operator()(mini_xml const& xml) const {
		out << std::string(indent, ' ');
		out << "tag: " << xml.name << std::endl;
		out << std::string(indent, ' ');
		out << '{' << std::endl;

		for (auto const & node : xml.children) {
			boost::apply_visitor(mini_xml_node_printer(out, indent), node);
		}

		out << std::string(indent, ' ');
		out << '}' << std::endl;
	}

	std::ostream&
		operator << (std::ostream& out, client::mini_xml & xml) {
		client::mini_xml_printer print(out);
		print(xml);
		return out;
	};
}

int main()
{
	std::ifstream in("3.toyxml", std::ios_base::in);
	if (!in)
	{
		std::cout << "file is not found." << std::endl;
		return 0;
	}

	std::string cmd;
	in.unsetf(std::ios::skipws);
	std::copy(
		std::istream_iterator<char>(in),
		std::istream_iterator<char>(),
		std::back_inserter(cmd));

	client::mini_xml ast;
	if (client::parse_process(std::begin(cmd), std::end(cmd), ast)) {
		std::cout << ast;
		std::cout << "Parsing OK" << std::endl;
	}
	else
	{
		std::cout << "Parsing FAIL" << std::endl;
	}

	return 0;
}