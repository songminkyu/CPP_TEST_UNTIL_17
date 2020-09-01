#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>


template<int...>
struct seq
{
};

template<int max, int... s>
struct make_seq : make_seq < max - 1, max - 1, s... >
{
};

template<int... s>
struct make_seq < 0, s... >
{
	typedef seq<s...> type;
};

template<int max>
using MakeSeq = typename make_seq<max>::type;

template<int x, typename Arg>
struct tuple_storage
{
	tuple_storage()
	{
		std::cout << "tuple_storage" << std::endl;
	}
	Arg data;
};

template<typename Seq, typename... Args>
struct tuple_helper
{
	tuple_helper()
	{
		std::cout << "tuple_helper1" << std::endl;
	}
};

template<int s0, int... s, typename A0, typename... Args>
struct tuple_helper<seq<s0, s...>, A0, Args...> : tuple_storage<s0, A0>, tuple_helper < seq<s...>, Args... >
{
	tuple_helper()
	{
		std::cout << "tuple_helper2" << std::endl;
	}
};

template<typename... Args>
struct foo : tuple_helper<MakeSeq<sizeof...(Args)>, Args...>
{
	foo()
	{
		std::cout << "foo" << std::endl;
	}
};

template<int N, typename T>
T& get(tuple_storage<N, T>& f)
{
	return f.data;
}

template<int N, typename T>
T const& get(tuple_storage<N, T> const& f)
{
	return f.data;
}

void main()
{
	foo<std::vector<int>, float, int, std::string> f;

	
	get<0>(f).emplace_back(11);
	for (auto && item : get<0>(f)) std::cout << item << std::endl;
	
	auto && B = get<1>(f) = 3.14f;
	auto && C = get<2>(f) = 6;
	auto && D = get<3>(f) = "123";
	//std::cout << A << ", " << B << ", " << C << "," << D + "456" << "\n";
}