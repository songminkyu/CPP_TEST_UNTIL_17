#include "stdafx.h"
#include <iostream>
#include <type_traits>


struct myString {

};

struct ccString1 : public myString {
	
};

struct ccString2 : public myString {

};

#if (_MSC_VER >= 1700) // 11문법 부터.
template<typename T>
struct check_type
	: std::false_type
{ // determine whether T is type that we want

};
template<>
struct check_type<myString>
	: std::true_type
{ // determine whether T is type that we want
};

template<>
struct check_type<ccString2>
	: std::true_type
{ // determine whether T is type that we want

};

#else
template <class T, T val>
struct integral_constant
{
	typedef integral_constant<T, val>  type;
	typedef T                          value_type;
	static const T value = val;
};


template<typename T>
struct check_type : integral_constant<bool, false>
{ // determine whether T is type that we want

};

template<>
struct check_type<myString> : integral_constant<bool, true>
{ // determine whether T is type that we want

};

template<>
struct check_type<ccString2> : integral_constant<bool, true>
{ // determine whether T is type that we want

};
#endif
template <bool, class T = void>
struct enable_if
{};

template <class T>
struct enable_if<true, T>
{
	typedef T type;
};

template <class T>
typename enable_if<check_type<T>::value, T>::type
foo(T t)
{
	// …
	return t;
}

int main()
{
	ccString1 str1;
	ccString2 str2;
	foo<ccString1>(str1);
	foo<ccString2>(str2);
	return 0;

}