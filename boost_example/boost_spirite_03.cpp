#include <iostream>
#include <string>
#include <boost\config\warning_disable.hpp>
#include <boost\spirit\home\x3.hpp>

namespace client {
	namespace x3 = boost::spirit::x3;
	using x3::_attr;

	struct print_action {
		template<typename Context>
		void operator()(Context const & ctx) const
		{
			std::cout << _attr(ctx) << std::endl;
		}
	};
}

int main()
{
	using namespace boost::spirit::x3;
	using client::print_action;

	std::string input("876 1234.56");
	auto iter = input.begin();
	auto end_iter = input.end();
	parse(std::begin(input), std::end(input), int_[print_action()] >> " " >> double_[print_action()]);
	
	return 0;

	
}