#include <iostream>
#include <boost/proto/proto.hpp>
#include <boost/typeof/std/ostream.hpp>

namespace proto = boost::proto;

template<typename Expr>
void evaluate(Expr const& expr) {
	proto::default_context ctx;
	proto::eval(expr, ctx);
}

int main() {
	proto::terminal<std::ostream&>::type cout_ = { std::cout };
	evaluate(cout_ << "Hellow" << "," << "world");
	return 0;
}