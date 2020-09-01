#include "stdafx.h"
#include <stdio.h>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <string>

#define TYPE 0

//#define TEST1
#define TEST2
//#define TEST3

#ifdef TEST1
template <class T>
void limit_func();

template<>
void limit_func<int>() {
}

template<>
void limit_func<char>() {
}

template<>
void limit_func<long long int>() {
}

template <class T>
void limit_func_test() {
	limit_func<T>();
	printf("%s - type: %s success\n", __FUNCSIG__, typeid(T).name());
}
#endif


#ifdef TEST2
template <class T>
void limit_assert_test() {
	static_assert(std::is_same<T, int>::value | std::is_same<T, long long int>::value, __FUNCSIG__" < ----has been detected.");
}

#endif


#ifdef TEST3
template <class T>
class limit_class;

template <>
class limit_class<int> {};

template <>
class limit_class<char> {};

template <>
class limit_class<long long int> {};

template <class T>
void limit_class_func() {

	static_assert(std::is_same<T, int>::value | std::is_same<T, long long int>::value, __FUNCSIG__" < ----has been detected.");
	printf("%s - type: %s success\n", __FUNCTION__, typeid(T).name());
	limit_class<T>();

}
#endif

template<template<typename, typename>class checker, typename... Ts>
struct is_any_to_first : std::false_type {};

template<template<typename, typename>class checker, typename T0, typename T1, typename... Ts>
struct is_any_to_first<checker, T0, T1, Ts...> :
	std::integral_constant< bool, checker<T0, T1>::value || is_any_to_first<checker, T0, Ts...>::value>
{};

template<typename... Ts>
using is_any_same_to_first = is_any_to_first< std::is_same, Ts... >;


template<template<typename, typename>class checker, typename... Ts>
struct is_all : std::true_type {};

template<template<typename, typename>class checker, typename T0, typename T1, typename... Ts>
struct is_all<checker, T0, T1, Ts...> :
	std::integral_constant< bool, checker<T0, T1>::value && is_all<checker, T0, Ts...>::value>
{};

template<typename... Ts>
using is_all_same = is_all< std::is_same, Ts... >;

/***************************************************************************************************/

#if(TYPE) //김홍규
template <class T, class First, class... AS>
struct is_any//<T,First,AS...>
	: std::integral_constant<bool, (std::is_same<T, First>::value) || (is_any<T, AS...>::value)>
{};

template <class T, class First>
struct is_any<T, First>
	: std::is_same<T, First>
{};

#else // 유원상
template <class T, class... AS>
struct is_any : std::false_type {};

template <class T, class First>
struct is_any<T, First> : std::is_same<T, First> {};

template <class T, class First, class... AS>
struct is_any<T, First, AS...> : std::integral_constant<bool, (std::is_same<T, First>::value) || (is_any<T, AS...>::value)> {};

#endif


template <class T, class First, class... AS>
void type_bound() {
	static_assert(is_any<T, First, AS...>::value, __FUNCSIG__" <----- please add or delete ");
	std::cout << __FUNCSIG__ << std::endl;
}

template<>

void type_bound<unsigned int, unsigned int, char, size_t>()
{
	std::cout << "asdasd" << std::endl;
}


template<>
void type_bound<unsigned int, char>()
{
	std::cout << "good" << std::endl;
}


template< int A >
struct _A
{
	enum { _value = A };
	static_assert(_A< 20 >::_value == _A< 20 >::_value, "good");
};



void main() {

#ifdef TEST1
	limit_func_test<int>();
	limit_func_test<char>();
	limit_func_test<long long int>();
	//limit_func_test<double>(); // compile time error
#endif
#ifdef TEST2
	limit_assert_test<int>();
	//limit_assert_test<char>();
	limit_assert_test<long long int>();
	//limit_assert_test<double>(); // compile time error
#endif
#ifdef TEST3
	limit_class_func<int>();
	limit_assert_test<char>();
	limit_class_func<long long int>();
	//limit_class_func<double>(); // compile time error

	type_bound<unsigned int, unsigned int, char, size_t>();
	//type_bound<unsigned long, int, double, char, size_t>();
	//type_bound<unsigned long, int, double, char>();
#endif
#ifdef TYPE


	_A<0> a;

	type_bound<unsigned int, unsigned int, char, size_t>();
	type_bound<unsigned int, char>(); //error


	if (is_all_same<int, char>::value == is_all_same<double>::value) { std::cout << "똑같다" << std::endl; }
	else															 { std::cout << "다르다" << std::endl; }

#endif
}