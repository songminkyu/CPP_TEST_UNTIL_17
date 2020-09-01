#include <boost\config\warning_disable.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <boost\spirit\include\karma.hpp>
#include <boost\lambda\lambda.hpp>

using namespace boost::spirit;
using boost::spirit::ascii::space;
using boost::lambda::_1;

namespace client {
	namespace karma = boost::spirit::karma;
	using provide_type = std::string;

	template< typename OutputIterator>
	struct test_grammar : karma::grammar<OutputIterator, std::vector<provide_type>()>
	{
		test_grammar() : test_grammar::base_type(query)
		{
			query = *(karma::string << '\n');
		}

		karma::rule<OutputIterator, std::vector<provide_type>()> query;
	};
	
}

void main() {
	
	/*
	
	* iterator adapter
	2 + 3;
	+ 2 3 ; test(1,2) ==> test(), 1 2 -> function tpy ������(operator)
	2 3 +

	* C++ concepts
	//iterator basic concept
	for( IT it = begin(); it != end(); ++it){ //++it, !=
	   std::cout << *it;					  //*it

	}


	*/
	using sink_type = std::ostream_iterator<const char>;

	//sink_type sinker(std::cout);
	std::ofstream op("karma_test.txt");
	sink_type sinker(op);

	sinker = 'a';
	std::string h = "\nhello\n";
	std::copy(std::begin(h), std::end(h), sinker);

	std::vector<int> v(8); //������ 8�� �Ҵ�
	std::generate(v.begin(), v.end(), std::rand);

	auto generator1 = *int_; //���� ��� ������ ������ {1,2,3} => 123 ���κ��� ������
	std::cout << karma::format(
		generator1,
		v) // v �� ��Ʈ����Ʈ Ÿ�� ��, ���� ������� ������
		<< std::endl;

	karma::generate(
		sinker,//output iterator <<-generatored characters
		generator1,  // generator
		v    //attribute : output type
	);
	sinker = '\n';

	std::cout <<
		karma::format_delimited(
			generator1, // generator 
			space,      // generator
			v
		) << std::endl;

	auto generator2 = lit('[') << *int_ << ']';
	std::cout <<
		karma::format_delimited(
			generator2, // generator 
			space,      // generator
			v
		) << std::endl;


	auto generator3 = int_ % ","; // �޸��� ��������Ʈ �Դϴ�.
	std::cout <<
		karma::format_delimited(
			generator3, // generator 
			space,      // generator
			v
		) << std::endl;

	/*
	html list ����
	*/
	auto generator4 = "<ol>" << *verbatim[" <li>" << int_ << "</li>"] << "</ol>";
	std::cout <<
		karma::format_delimited(
			generator4,
			'\n',
			v
		) << std::endl;
		

	auto generator5 = *verbatim["|" << right_align[int_] << "|"]; //left_align, right_align, center
	std::cout <<
		karma::format_delimited(
			generator5,
			'\n',
			v
		) << std::endl;

	auto lambda = [](int& val) {
		val *= 100;

	};
	auto generator6 = *int_[lambda];

	std::cout <<
		karma::format_delimited(
			generator6,
			'\n',
			v
		) << std::endl;


	auto generator7 = *int_[::_1 = -::_1 * 100];

	std::cout <<
		karma::format_delimited(
			generator7,
			'\n',
			v
		) << std::endl;

	
	std::vector<std::string> sourcer;
	sourcer.push_back("hellow");
	sourcer.push_back("world");

	client::test_grammar<sink_type> sink_grammar;

	karma::generate_delimited(
		sinker,
		sink_grammar,
		"^_^",
		sourcer
	);
	

}