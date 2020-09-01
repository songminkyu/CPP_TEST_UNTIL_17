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
qi : parser 역할을 수행하고
karma : generator 즉 데이터를 문자열로 변환 시켜주는 역할

즉 저 두개는 상관 관계를 갖고 있다

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

	//복소수 형태로 만들기 위해
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
				'(' >> double_[ref(rN) = _1] // [ref(rN) = _1] -> 람다와 같은 의미임, 즉 람다의 일종
				>> -(',' >> double_[ref(iN) = _1]) >> ')'
				| double_[ref(rN) = _1]
				),
			space);
		// 3번째 인자의 역할은
		// (1,4) 콤마가 있는경우 '(' >> double_[ref(rN) = _1] 수행 하고
		// 콤마가 없으면 -(',' >> double_[ref(iN) = _1]) 수행 한다.

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

		//(실수, 허수) => 복소수
		//(1,0) => 허수부가 없으니 실수만 출력 [1]
		//(1,2) => 실수와 허수부 두개다 공존 하니 [1,2] 실수 허수 모두 출력
		//parsing 된 데이터는 최종적으로 sink로 할당
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