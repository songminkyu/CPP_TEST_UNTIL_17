#include <boost\config\warning_disable.hpp>
#include <iostream>
#include <ccomplex>
#include <fstream>
#include <iterator>
#include <boost\spirit\include\phoenix.hpp>
#include <boost\spirit\include\qi.hpp>
#include <boost\spirit\include\karma.hpp>
#include <boost\spirit\include\support_adapt_adt_attributes.hpp>


/*
qi : parser ������ �����ϰ�
karma : generator �� �����͸� ���ڿ��� ��ȯ �����ִ� ����

�� �� �ΰ��� ��� ���踦 ���� �ִ�

qi <---> karma

*/

BOOST_FUSION_ADAPT_ADT(
	std::complex<double>,
	(bool,bool,obj.imag()!=0, ())
	(double,double,obj.real(),obj.reaul(val))
	(double,double,obj.imag(),obj.imag(val))
)
/*
BOOST_FUSION_ADAPT_ADT(
type_name,
(attribute_type0, attribute_const_type0, get_expr0, set_expr0)
(attribute_type1, attribute_const_type1, get_expr1, set_expr1)
...
)
*/

namespace client {

	//���Ҽ� ���·� ����� ����
	template<typename Iterator>
	bool parse_complex(Iterator first, Iterator last, std::complex<double> & c)
	{
		if (first == last) return false;
		namespace ascii = boost::spirit::ascii;
		namespace qi = boost::spirit::qi;
		using qi::double_;
		using ascii::space;

		using qi::_1;
		using qi::phrase_parse;
		using boost::phoenix::ref;

		double rN = 0.0;
		double iN = 0.0;

		bool r = qi::phrase_parse(
			first,
			last,
			(
				'(' >> double_[ref(rN) = _1] // [ref(rN) = _1] -> ���ٿ� ���� �ǹ���, �� ������ ����
				>> -(',' >> double_[ref(iN) = _1]) >> ')'
				| double_[ref(rN) = _1]
				),
			space);
		// 3��° ������ ������
		// (1,4) �޸��� �ִ°�� '(' >> double_[ref(rN) = _1] ���� �ϰ�
		// �޸��� ������ -(',' >> double_[ref(iN) = _1]) ���� �Ѵ�.

		if (first != last) return false;
		c = std::complex<double>(rN, iN);
		return r;
	}

	template<typename OutputIterator>
	bool generate_complex(OutputIterator sink, std::complex<double> const & c) {
		namespace karma = boost::spirit::karma;

		using karma::double_;
		using karma::bool_;
		using karma::true_;
		using karma::omit;

		//(�Ǽ�, ���) => ���Ҽ�
		//(1,0) => ����ΰ� ������ �Ǽ��� ��� [1]
		//(1,2) => �Ǽ��� ����� �ΰ��� ���� �ϴ� [1,2] �Ǽ� ��� ��� ���
		//parsing �� �����ʹ� ���������� sink�� �Ҵ�
		return karma::generate(
			sink,
			(
				&true_ << "[" << double_ << "," << double_ << ']'
				| 
				omit[bool_] << "[" << double_ << "]"
				),
			c
		);

	}
}
using sink_type = std::ostream_iterator<const char>;

void process(sink_type & sinker) {
	std::string str;
	std::complex<double> c;
	while (std::cout << "\nparsing:", std::getline(std::cin, str)) {
		if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		do {
			if(!client::parse_complex(str.begin(), str.end(), c)) break;
			if (!client::generate_complex(sinker, c)) break;

			*sinker = '\n';
			std::cout << "process ending : \n";
		} while (false);
	}
}
void main() {
	std::ofstream op("test.txt");
	sink_type sinker1(op);
	process(sinker1);
}