#include "stdafx.h"
#include <tuple>
#include <iostream>
#include <array>
#include <utility>

//#define MAIN_ACTIVE

using namespace std;
// Convert array into a tuple
template<typename Array, std::size_t... I>
decltype(auto) a2t_impl(const Array& a, std::index_sequence<I...>)
{
	return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
decltype(auto) a2t(const std::array<T, N>& a)
{
	return a2t_impl(a, Indices());
}

// pretty-print a tuple (from http://stackoverflow.com/a/6245777/273767)

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
	const Tuple & t,
	std::index_sequence<Is...>)
{
	using swallow = int[]; // guaranties left to right order
	(void)swallow {
		0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
	};
}

template<class Ch, class Tr, class... Args>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,
	const std::tuple<Args...>& t)
{
	os << "(";
	print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
	return os << ")";
}


template<typename Func, typename Tup, std::size_t... index>
decltype(auto) invoke_helper(Func&& func, Tup&& tup, std::index_sequence<index...>)
{
	return func(std::get<index>(std::forward<Tup>(tup))...);
}

template<typename Func, typename Tup>
decltype(auto) invoke(Func&& func, Tup&& tup)
{
	constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
	return invoke_helper(std::forward<Func>(func),
		std::forward<Tup>(tup),
		std::make_index_sequence<Size>{});
}

void foo(int a, const std::string& b, float c)
{
	std::cout << a << " , " << b << " , " << c << '\n';
}

#ifdef MAIN_ACTIVE
int main()
{
	std::array<float, 4> array = { 1.2
								  ,2.3,
								  3,4 };

	// convert an array into a tuple
	auto tuple = a2t(array);
	static_assert(std::is_same<decltype(tuple),std::tuple<float, float, float, float>>::value, "");

	// print it to cout
	std::cout << tuple << '\n';

	auto args = std::make_tuple(2, "Hello", 3.5);
	invoke(foo, args);
}
#endif