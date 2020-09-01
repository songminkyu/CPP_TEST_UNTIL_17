// 26_day_template_3day_sub.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tuple>
#include <iostream>
#include <vector>

//#define MAIN_ACTIVE

template<typename... T, typename F, std::size_t... i>
void impl_call_fn(std::tuple<T...> const & tuple, F const & f, std::index_sequence<i...>)
{
	int expand[] = { 0,(f(std::get<i>(tuple)),void(),0)... };
	(void)expand;
}

template<typename... T, typename F>
void call_fn(std::tuple<T...> const & tuple, F const & f)
{
	impl_call_fn(tuple, f, std::make_index_sequence< sizeof...(T)>{});
}

#ifdef MAIN_ACTIVE
int main()
{
	std::vector<int> asd;
	asd.push_back(1);
	asd.push_back(2);
	call_fn(std::make_tuple("song", 1, 2, 3), [](auto&&arg) { std::cout << arg << std::endl;});
	return 0;
}
#endif

