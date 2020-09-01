#include "stdafx.h"
#include <iostream>

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
struct foo_storage
{
	foo_storage()
	{
		std::cout << "foo_storage" << std::endl;
	}
	Arg data;
};

template<typename Seq, typename... Args>
struct foo_helper
{
	foo_helper()
	{
		std::cout << "foo_helper1" << std::endl;
	}
};

template<int s0, int... s, typename A0, typename... Args>
struct foo_helper<seq<s0, s...>, A0, Args...> : foo_storage<s0, A0>, foo_helper < seq<s...>, Args... >
{
	foo_helper()
	{
		std::cout << "foo_helper2" << std::endl;
	}
};

template<typename... Args>
struct foo : foo_helper<MakeSeq<sizeof...(Args)>, Args...>
{
	foo()
	{
		std::cout << "foo" << std::endl;
	}
};

template<int N, typename T>
T& get(foo_storage<N, T>& f)
{
	return f.data;
}

template<int N, typename T>
T const& get(foo_storage<N, T> const& f)
{
	return f.data;
}

void main()
{
	foo<int, char* , float> f;

	get<0>(f) = 7;
	get<1>(f) = "song";
	get<2>(f) = 3.14f;
	std::cout << get<0>(f) << ", " << get<1>(f) << ", " << sizeof(f) << "\n";

	const char conChar[2][100] = { {"kim tea hong," }, { "babo" } };

	for (const char & item : *conChar) {
		std::cout << item << std::endl;
	}
}
