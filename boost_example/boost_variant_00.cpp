#include <boost/variant.hpp>
#include <iostream>
#include <vector>

class cprint : public boost::static_visitor<>
{
public:
	template < typename T >
	void operator()(const T& t) const
	{
		std::cout << t;
	}
};

using var_t = boost::variant<int, double, std::string>;

void main() {
	std::vector<var_t> vars = { "hello", 1, 0.01 };
	std::for_each(vars.begin(), vars.end(), boost::apply_visitor(cprint())); // 모두 순회하며 콘솔에 출력한다.
}

