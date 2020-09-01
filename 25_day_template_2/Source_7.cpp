#include "stdafx.h"
#include <cstdio>
#include <memory>


//#define MAIN_ACTIVE

#define AUTO_DOT_TEST

#if defined AUTO_DOT_TEST
template <class O, class F, class...AS>
auto auto_dot_call_mem_func(O&& o, F&& f, AS&&... args)
{
	return (autodot(o).*f)(std::forward<AS>(args)...);
}
#endif

template <class T>
auto autodot(T&& a) -> decltype(a) {
	return a;
}
template <class T>
auto autodot(T* a) -> decltype(*a) {
	return *a;
}

struct structA {

	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("process : %d + %d = %d\n", a, b, c);
		return c;
	}
};

#if defined MAIN_ACTIVE
int main() {
	structA A; structA* B = new structA();
	int x = 1; int y = 2; int z = 3;
	auto_dot_call_mem_func(A, &structA::print, x, y, z);
	auto_dot_call_mem_func(B, &structA::print, x, y, z);
	return 0;
}
#endif