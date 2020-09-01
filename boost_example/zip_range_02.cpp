#include <iostream>
#include <utility>
#include <vector>
#include <tuple>

/*

If you like operator overloading, here are three possibilities. The first two are using std::pair<> and std::tuple<>, 
respectively, as iterators; the third extends this to range-based for. Note that not everyone will like these definitions of 
the operators, so it's best to keep them in a separate namespace and have a using namespace in the functions (not files!) 
where you'd like to use these.

*/
// put these in namespaces so we don't pollute global
namespace pair_iterators
{
	template<typename T1, typename T2>
	std::pair<T1, T2> operator++(std::pair<T1, T2>& it)
	{
		++it.first;
		++it.second;
		return it;
	}
}

namespace tuple_iterators
{
	// you might want to make this generic (via param pack)
	template<typename T1, typename T2, typename T3>
	auto operator++(std::tuple<T1, T2, T3>& it)
	{
		++(std::get<0>(it));
		++(std::get<1>(it));
		++(std::get<2>(it));
		return it;
	}

	template<typename T1, typename T2, typename T3>
	auto operator*(const std::tuple<T1, T2, T3>& it)
	{
		return std::tie(*(std::get<0>(it)),
			*(std::get<1>(it)),
			*(std::get<2>(it)));
	}

	// needed due to ADL-only lookup
	template<typename... Args>
	struct tuple_c
	{
		std::tuple<Args...> containers;
	};

	template<typename... Args>
	auto tie_c(const Args&... args)
	{
		tuple_c<Args...> ret = { std::tie(args...) };
		return ret;
	}

	template<typename T1, typename T2, typename T3>
	auto begin(const tuple_c<T1, T2, T3>& c)
	{
		return std::make_tuple(std::get<0>(c.containers).begin(),
			std::get<1>(c.containers).begin(),
			std::get<2>(c.containers).begin());
	}

	template<typename T1, typename T2, typename T3>
	auto end(const tuple_c<T1, T2, T3>& c)
	{
		return std::make_tuple(std::get<0>(c.containers).end(),
			std::get<1>(c.containers).end(),
			std::get<2>(c.containers).end());
	}

	// implement cbegin(), cend() as needed
}

int main()
{
	using namespace pair_iterators;
	using namespace tuple_iterators;

	std::vector<double> ds = { 0.0, 0.1, 0.2 };
	std::vector<int   > is = { 1,   2,   3 };
	std::vector<char  > cs = { 'a', 'b', 'c' };

	// classical, iterator-style using pairs
	for (auto its = std::make_pair(ds.begin(), is.begin()),
		end = std::make_pair(ds.end(), is.end()); its != end; ++its)
	{
		std::cout << "1. " << *(its.first) + *(its.second) << " " << std::endl;
	}

	// classical, iterator-style using tuples
	for (auto its = std::make_tuple(ds.begin(), is.begin(), cs.begin()),
		end = std::make_tuple(ds.end(), is.end(), cs.end()); its != end; ++its)
	{
		std::cout << "2. " << *(std::get<0>(its)) + *(std::get<1>(its)) << " "
			<< *(std::get<2>(its)) << " " << std::endl;
	}

	// range for using tuples
	for (const auto& d_i_c : tie_c(ds, is, cs))
	{
		std::cout << "3. " << std::get<0>(d_i_c) + std::get<1>(d_i_c) << " "
			<< std::get<2>(d_i_c) << " " << std::endl;
	}
}

