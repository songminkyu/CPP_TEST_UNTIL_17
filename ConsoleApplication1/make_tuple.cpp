#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <functional>


#if 0
std::tuple<int, int> f() // this function returns multiple values
{
	int x = 5;
	return std::make_tuple(x, 7); // return {x,7}; in C++17
}

int main()
{

http://en.cppreference.com/w/cpp/utility/tuple/make_tuple

	// heterogeneous tuple construction
	int n = 1;
	auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
	n = 7;
	std::cout << "The value of t is " << "("
		<< std::get<0>(t) << ", " << std::get<1>(t) << ", "
		<< std::get<2>(t) << ", " << std::get<3>(t) << ", "
		<< std::get<4>(t) << ")\n";

	// function returning multiple values
	int a, b;
	std::tie(a, b) = f();
	std::cout << a << " " << b << "\n";
}
#endif

template <class T>
struct unwrap_refwrapper
{
	using type = T;
};

template <class T>
struct unwrap_refwrapper<std::reference_wrapper<T>>
{
	using type = T&;
};

template <class T>
using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;


//리턴 부분에서 auto 와 같은 의미를 가짐.
//auto == std::tuple<special_decay_t<Types>...>
template <class... Types>
auto song_make_tuple(Types&&... args)
{
	return std::tuple<special_decay_t<Types>...>(std::forward<Types>(args)...);
}

std::tuple<int, std::string, double, std::string> f() // this function returns multiple values
{
	int x = 5;
	return song_make_tuple(x, "Hello", -0.1, "song"); // return {x,7}; in C++17
}

int main()
{

	//http://en.cppreference.com/w/cpp/utility/tuple/make_tuple

	// heterogeneous tuple construction
	int n = 1;
	auto t = song_make_tuple(10, "Test", 3.14, std::ref(n), n);
	n = 7;
	std::cout << "The value of t is " << "("
		<< std::get<0>(t) << ", " << std::get<1>(t) << ", "
		<< std::get<2>(t) << ", " << std::get<3>(t) << ", "
		<< std::get<4>(t) << ")\n";

	// function returning multiple values
	int Q;
	std::string W;
	double E;
	std::string R;
	std::tie(Q, W, E, R) = f();
	std::cout << Q << " " << W << " " << E << " " << R << "\n";
}