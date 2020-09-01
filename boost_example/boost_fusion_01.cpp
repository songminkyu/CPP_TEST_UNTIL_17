// helloFusion.cpp : Defines the entry point for the console application.
//


// std
#include <iostream>
#include <string>
// fusion 컨테이너
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/container/vector.hpp>
// fusion IO
#include <boost/fusion/sequence/io.hpp>
// fusion 알고리즘과 반복자
#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/iterator/advance.hpp>
// fustion contianer <--> MIL link header
#include <boost/fusion/mpl.hpp>

int main() {
	namespace fusion = boost::fusion;
	namespace mpl = boost::mpl;

	using I = fusion::list<double, std::string >;
	using R = fusion::vector<double, std::string, int, short>;

	static constexpr int r_size = fusion::result_of::size<R>::value;
	static constexpr int i_size = fusion::result_of::size<I>::value;
	static_assert(i_size <= r_size, "");

	using r_begin = fusion::result_of::begin<R>::type;
	using r_end = fusion::result_of::end<R>::type;
	using r_advance = fusion::result_of::advance_c<r_begin, i_size>::type;

	using mpl_r_advance = mpl::fusion_iterator<r_advance>;
	using mpl_r_end = mpl::fusion_iterator<r_end>;

	using tail = mpl::iterator_range<mpl_r_advance, mpl_r_end>;

	// manipulator
	std::cout << fusion::tuple_open('[');
	std::cout << fusion::tuple_close(']');
	std::cout << fusion::tuple_delimiter(", ");

	I i(123.456, "Hello");
	R r(fusion::join(i, tail()));
	std::cout << r << std::endl;
	return 0;
}