#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <string>

template< std::size_t I, class T >
struct song_make_tuple_element;

// recursive case
template< std::size_t I, class Head, class... Tail >
struct song_make_tuple_element<I, std::tuple<Head, Tail...>>
	: std::tuple_element<I - 1, std::tuple<Tail...>> { };

// base case
template< class Head, class... Tail >
struct song_make_tuple_element<0, std::tuple<Head, Tail...>> {
	typedef Head type;
};

template <class... Args>
struct type_list
{
	template <std::size_t N>
	using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};

template <class... Args>
struct type_list_song
{
	template <std::size_t N>
	using type = typename song_make_tuple_element<N, std::tuple<Args...>>::type;
};

//int main()
//{
//	std::cout << std::boolalpha;
//	type_list<int, std::string, bool>::type<1> x = "song";
//	type_list_song<int, std::string, bool>::type<1> y = "min";
//	std::cout << x << " "<<  y << std::endl;
//}