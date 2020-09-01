#include<iostream>
#include <boost\proto\proto.hpp>
#include <boost\proto\context.hpp>

namespace proto = boost::proto;
using proto::_;

template<int I>
struct placeholder {  };

proto::terminal< placeholder<1> >::type const _1 = { {} };
proto::terminal< placeholder<2> >::type const _2 = { {} };

struct calculator_context : proto::callable_context< calculator_context const >
{
	using result_type = double;
	double d[2];
	explicit calculator_context(double d1 = 0.0, double d2 = 0.) : d{ d1, d2 } { }

	template<int I>
	double operator()(proto::tag::terminal, placeholder<I>) const
	{
		return d[I - 1];
	}

};

template<typename Expr>
double evaluate(Expr const& expr, double d1 = 0., double d2 = 0.) {
	calculator_context const ctx( d1, d2 );
	// Evaluate the calculator
	return proto::eval(expr, ctx);
}

int main() {
	std::cout << evaluate(_1 + 2.0, 3.0) << std::endl;
	std::cout << evaluate(_1 + _2, 3.0, 2.0) << std::endl;
	std::cout << evaluate((_1 - _2) / _2 , 3.0, 2.0) << std::endl;
	
	return 0;
}
