#include <boost\config\warning_disable.hpp>
#include <boost\spirit\home\x3.hpp>
#include <iostream>
using boost::spirit::x3::unused_type;

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

int main() {
	using boost::spirit::x3::int_;
	using boost::spirit::x3::parse;
	using client::print_action;

	std::string cmd;
	print_action pa;

	do {
		std::cin >> cmd;
		if ("exit" == cmd) break;
		parse(std::cbegin(cmd), std::cend(cmd), '{' >> int_[pa] >> '}');
	} while (true);

	return 0;
}