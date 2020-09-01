#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>

// Qi Sample 1

#define GENERAL_TEST (0)
#define PASER_SEMANTIC_ACTIONS (1)
#define HOW_TO_USE_THE_SUM (2)
#define HOW_TO_USE_THE_DETAIL_LIST (3)
#define HOW_TO_USE_THE_FUNCTION (4)
#define HOW_TO_USE_THE_FUNCTOR (5)
#define HOW_TO_USE_THE_ARTTRIBUTE_CODE (6)
#define SELECT_ITEM (HOW_TO_USE_THE_ARTTRIBUTE_CODE)

namespace client {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	//Context pass:bool
	void raw_print(double & val, qi::unused_type, qi::unused_type) {
		std::cout << "raw function : " << val << std::endl;
	}
	void print(double & val) {
		std::cout << "normal iterator : " << val << std::endl;
	}
	struct print_object {
		void operator()(double & val) const
		{
			std::cout << "functor call : " << val << std::endl;
		}
	};

	struct print_object2 {
		void print(double & val) const
		{
			std::cout << "member function :" << val << std::endl;
		}
	};
	
	template<typename iterator>
	bool parse_process(iterator first, iterator last) {
		if (first == last)	return false;
		using qi::double_;
		using qi::char_;
		using ascii::space;
		using qi::phrase_parse;
		using qi::_1;
		using phoenix::push_back;
		
		auto skip_parser = space;

#if SELECT_ITEM == GENERAL_TEST
		auto today_parser0 = double_ >> *(char_(',') >> double_);
		//24.5
		//24.5 78.9 1,2,3
#elif SELECT_ITEM == PASER_SEMANTIC_ACTIONS
		//Paser Semantic Actions
		auto boost_actor1 = std::cout << _1 << std::endl;
		auto today_parser1 = double_[boost_actor1] % char_(',');


#elif SELECT_ITEM == HOW_TO_USE_THE_SUM
		//how to sum
		double sum = 0.0;
		auto init_actor = (phoenix::ref(sum) = _1);
		auto add_actor = (phoenix::ref(sum) += _1);
		auto today_parser2 = double_[init_actor] >> *(char_(',') >> double_[add_actor]);
		auto last2 = [&]() {std::cout << "sum : " << sum << std::endl; };

#elif SELECT_ITEM == HOW_TO_USE_THE_DETAIL_LIST
		//how to data list
		std::vector<double> datas;
		auto push_back_actor = push_back(phoenix::ref(datas), _1);
		auto today_parser3 = double_[push_back_actor] >> *(char_(',') >> double_[push_back_actor]);
		auto last3 = [&]() {
			std::cout << "data list : " << std::endl;

			for (auto & mitem : datas) {
				std::cout << mitem << std::endl;
			}
		};

#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTION
		//how to use the function
		auto & fn1 = raw_print;
		auto & fn2 = print;
		auto & today_parser4 = double_[&fn1] >> *(char_(',') >> double_[&fn2]);
		auto last4 = [&]() {std::cout << std::endl; };

#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTOR
		//how to use the functor
		auto func_obj = print_object();
		auto obj2 = print_object2();
		auto mem_fn = boost::bind(&print_object2::print ,&obj2, ::_1);//바인드가 정의한 플래스홀더를 사용해야함 qi가 만든것 사용 안됨 (X)
		auto today_parser5 = double_[func_obj] >> *(char_(',') >> double_[mem_fn]);
		auto last5 = [&]() {std::cout << std::endl; };

#elif SELECT_ITEM == HOW_TO_USE_THE_ARTTRIBUTE_CODE
		// 아래와 같은 attribute code는 최적의 코드라고 함. 
		// 여태 위에서 작성된 방법은 최대한 사용을 하지않도록 지양 해야함.
		std::vector<double> attrs;
		auto today_parser6 = double_ % char_(',');
		auto last6 = [&]() {
			std::cout << "attrs list: " << std::endl;
			for (auto & mitem : attrs) {
				std::cout << mitem << std::endl;
			}
		};
#else
#endif

#if SELECT_ITEM == GENERAL_TEST 
		bool resuit = phrase_parse(
			first, last,
			today_parser0,
			skip_parser
		);
#elif SELECT_ITEM == PASER_SEMANTIC_ACTIONS
		bool resuit = phrase_parse(
			first, last,
			today_parser1,
			skip_parser
		);
#elif SELECT_ITEM == HOW_TO_USE_THE_SUM
		bool resuit = phrase_parse(
			first, last,
			today_parser2,
			skip_parser
		);
#elif SELECT_ITEM == HOW_TO_USE_THE_DETAIL_LIST
		bool resuit = phrase_parse(
			first, last,
			today_parser3,
			skip_parser
		);
#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTION
		bool resuit = phrase_parse(
			first, last,
			today_parser4,
			skip_parser
		);
#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTOR
		bool resuit = phrase_parse(
			first, last,
			today_parser5,
			skip_parser
		);
#else
		bool resuit = phrase_parse(
			first, last,
			today_parser6,
			skip_parser,
			attrs
		);
#endif

		if (first != last) return false; //partial match deny\

#if SELECT_ITEM == HOW_TO_USE_THE_SUM
		last2();
#elif SELECT_ITEM == HOW_TO_USE_THE_DETAIL_LIST
		last3();
#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTION
		last4();
#elif SELECT_ITEM == HOW_TO_USE_THE_FUNCTOR
		last5();
#else
		last6();
#endif
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