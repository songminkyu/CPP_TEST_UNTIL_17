#include "stdafx.h"
#include <cstdio>
#include <memory>
#include <iostream>
#include <string>


/*
다형성에대해서도 동작하도록 만들어봤습니다.
단 object 에 rvalue 를 넣어주면 에러가 납니다.
예를 들면 structA() - > rvalue
*/

#define MAIN

#define AUTO_DOT_TEST

#if defined AUTO_DOT_TEST
template <class O, class D, class R, class...AS1, class...AS2>
R auto_dot_call_mem_func(O&& o, R(D::*f)(AS1...), AS2&&... args)
{
	return (static_cast<D*>(autoarrow(o))->*f)(std::forward<AS2>(args)...);
}
#endif

//-------------------------

template <class T> auto autoarrow(T&& a) {  return &a;  }
template <class T> auto autoarrow(T* a) { return a; }

//-------------------------

struct structA {
	
	int print(const int& a, const int& b, int& c, std::string& d) {
		c = a + b;
		printf("A : process : %d + %d = %d  %s\n", a, b, c, d.c_str());
		return c;
	}
};

struct structB : public structA {
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("B : process : %d + %d = %d\n", a, b, c);
		return c;
	}
};


template<typename T>
struct TY {
public:
	template<typename... AS>
	void test(AS... as) {
		T* obj = static_cast<T*>(this);
		obj->rawTest(std::forward<AS>(as)...);
	}
};

struct AB :TY<AB> {
	void rawTest(int& a) { std::cout << "나 인자 1개" << std::endl; }
	void rawTest(int& a, std::string& b) { std::cout << "나 인자 2개" << std::endl; }
};

struct CD : TY<CD> {
	void rawTest(int& a, int& b, std::string& C) { std::cout << "나 인자 3개" << std::endl; }
	void rawTest(int& a, int& b, int& c, std::string& D) { std::cout << "나 인자 4개" << std::endl; }
};



#ifdef MAIN
int main() {
	
	TY<AB> TY__AB;
	TY<CD> TY__CD;

	structA A;
	structA* B = new structB();
	int x = 1; int y = 2; int z = 3; std::string s = "good";

	//auto_dot_call_mem_func(A, &structA::print, x, y, z);
	auto_dot_call_mem_func(B, &structA::print, x, y, z, s);

	auto_dot_call_mem_func(TY__AB, &TY<AB>::test<int&>, x);
	auto_dot_call_mem_func(TY__AB, &TY<AB>::test<int&, std::string>, x, s);
	auto_dot_call_mem_func(TY__CD, &TY<CD>::test<int&, int&, std::string>, x, y, s);
	auto_dot_call_mem_func(TY__CD, &TY<CD>::test<int&, int&, int&, std::string>, x, y, z, s);

	return 0;
}
#endif