#include"stdafx.h"
#include <stdio.h>
#include <memory>
#include <typeinfo>
#include <initializer_list>

//#define MAIN_ACTIVE
#define CPP14
#define CPP11
int general_function_print(const int& a, const int& b, int& c);

#if defined CPP11
template <class T, class...AS>
auto auto_return_test_cpp11(T&& a, AS&&...args) -> decltype(a(std::forward<AS>(args)...))
{
	return a(std::forward<AS>(args)...);
}

template <class O, class F, class...AS>
auto cpp11_call_mem_func(O&& o, F&& f, AS&&... args) -> decltype((o.*f)(std::forward<AS>(args)...))
{
	return (o.*f)(std::forward<AS>(args)...);
}

template <class O, class F, class...AS>
auto cpp11_call_mem_func(O&& o, F&& f, AS&&... args) -> decltype((o->*f)(std::forward<AS>(args)...))
{
	return (o->*f)(std::forward<AS>(args)...);
}
#endif


#if defined CPP14 || defined CPP17
template <class T, class...AS>
decltype(auto) auto_return_test_cpp14(T&& a, AS&&...args)
{
	return a(std::forward<AS>(args)...);
}

template <class O, class F, class...AS>
decltype(auto) cpp14_call_mem_func(O&& o, F&& f, AS&&... args)
{
	return (o.*f)(std::forward<AS>(args)...);
}

template <class O, class F, class...AS>
decltype(auto) cpp14_call_mem_func(O* o, F&& f, AS&&... args)
{
	return (o->*f)(std::forward<AS>(args)...);
}
#endif



struct structA {
	/*int print(const int&& a, const int&& b) const{
	int c = a+b;
	printf("process : %d + %d = %d",a,b,c);
	return c;
	}*/
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("process : %d + %d = %d", a, b, c);
		return c;
	}
};


int general_function_print(const int& a, const int& b, int& c) {
	c = a + b;
	printf("process : %d + %d = %d", a, b, c);
	return c;
}

#if defined MAIN_ACTIVE
int main() {
	structA A;
	int x = 3; int y = 4; int z = 5;
	// A.print(x,y,z); printf(" / result : %d\n",z);
	printf(" / result : %d\n", auto_return_test_cpp14(general_function_print, x, y, z));
	printf(" / result : %d\n", cpp14_call_mem_func(A,&structA::print, x, y, z));


}
#endif