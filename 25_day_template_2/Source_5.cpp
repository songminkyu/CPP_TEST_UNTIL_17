#include "stdafx.h"
#include <iostream>
#include <array>

//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//

//#define TEST_CASE_1
//#define TEST_CASE_2

//--------------------------------------------------------------------------------//

#ifdef TEST_CASE_1
template <class R, class B, class...AS, class...AS2>
R call_mem_fn(R(B::*fn)(AS...), B* obj, AS2&&...args) {
	printf("overroad ver 1 : ");
	printf("obj is rvalue : %-6s / is ptr : %-6s / result : ",
		(std::is_rvalue_reference<decltype(obj)>::value) ? "true" : "false",
		(std::is_pointer<decltype(obj)>::value) ? "true" : "false");
	return (obj->*fn)(std::forward<AS2>(args)...).i;
}
template <class R, class B, class B2, class...AS, class...AS2>
R call_mem_fn(R(B::*fn)(AS...), B2&& obj, AS2&&...args) {
	printf("overroad ver 2 : ");
	printf("obj is rvalue : %-6s / is ptr : %-6s / result : ",
		(std::is_rvalue_reference<decltype(obj)>::value) ? "true" : "false",
		(std::is_pointer<decltype(obj)>::value) ? "true" : "false");
	return (obj.*fn)(std::forward<AS2>(args)...).i;
}
#endif

//--------------------------------------------------------------------------------//

#ifdef TEST_CASE_2
template <class R, class B, class...AS>
R call_mem_fn(R(B::*fn)(AS...), B* obj, AS...args) {
	printf("overroad ver 1 : ");
	printf("obj is rvalue : %-6s / is ptr : %-6s / result : ",
		(std::is_rvalue_reference<decltype(obj)>::value) ? "true" : "false",
		(std::is_pointer<decltype(obj)>::value) ? "true" : "false");
	return (obj->*fn)(args...).i;
}

template <class R, class B, class B2, class...AS>
R call_mem_fn(R(B::*fn)(AS...), B2&& obj, AS...args) {
	printf("overroad ver 2 : ");
	printf("obj is rvalue : %-6s / is ptr : %-6s / result : ",
		(std::is_rvalue_reference<decltype(obj)>::value) ? "true" : "false",
		(std::is_pointer<decltype(obj)>::value) ? "true" : "false");
	return (obj.*fn)(args...).i;
}
#endif

//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//

//#define  TEST_FUNC_1
//#define  TEST_FUNC_2
//#define  TEST_FUNC_3

//--------------------------------------------------------------------------------//

#ifdef  TEST_FUNC_1
template <class T>
class test {
public:
	T sum(T a, T b, T c) { return a + b + c; }
};
#endif

//--------------------------------------------------------------------------------//

#ifdef  TEST_FUNC_2
template <class T>
class test {
public:
	T sum(T& a, T& b, T& c) { return a + b + c; }
};
#endif

//--------------------------------------------------------------------------------//
//R_VALUE_IMPLICIT_TEST, L_VALUE_IMPLICIT_TEST 테스트 할때
#ifdef  TEST_FUNC_3
template <class T>
class test {
public:
	T sum(T&& a, T&& b, T&& c) { return a + b + c; }
};
#endif

//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//

class Intwrap {
public:
	Intwrap() :i(1) {/*printf("constructor:(%d)\n",i);*/ };
	Intwrap(int a_i) :i(a_i) {/*printf("constructor:(%d)\n",i);*/ };

	Intwrap(Intwrap const &a) :i(a.i) { printf("cOpy"); };
	void operator=(Intwrap const &b) { this->i = b.i; printf("cOpy"); }

	Intwrap& operator+(Intwrap c) { this->i += c.i; return *this; }

	int i;
};

//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//

//#define  VARIABLE
//#define  R_VALUE_TEST
//#define  L_VALUE_TEST
//#define  R_VALUE_IMPLICIT_TEST
//#define  L_VALUE_IMPLICIT_TEST

//--------------------------------------------------------------------------------//

//#define MAIN_ACTIVA // 메인 활성화 시킬떄 주석 해제

//--------------------------------------------------------------------------------//

#ifdef MAIN_ACTIVA
int main(int argc, const char * argv[]) {

	//--------------------------------------------------------------------------------//
#ifdef  VARIABLE
	auto T = new test<Intwrap>;
	test<Intwrap> T2;

	int i = 0;
	auto A = std::array<Intwrap, 9>();
	for (auto& a : A)
		a.i = ++i;

	int j = 0;
	auto B = std::array<int, 9>();
	for (auto& b : B)
		b = ++j;
#endif
	//--------------------------------------------------------------------------------//
#ifdef  R_VALUE_TEST
	//      <* rvalue argument test *> (Intwrap -> Intwrap)
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T, Intwrap(0), Intwrap(1), Intwrap(2)).i); // lvalue pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T2, Intwrap(3), Intwrap(4), Intwrap(5)).i); // lvalue not pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, test<Intwrap>(), Intwrap(6), Intwrap(7), Intwrap(8)).i); // rvalue not pointer
	printf("\n");
#endif
	//--------------------------------------------------------------------------------//
#ifdef  L_VALUE_TEST
	//      <* lvalue argument test *> (Intwrap -> Intwrap)
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T, A[0], A[1], A[2]).i); // lvalue pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T2, A[3], A[4], A[5]).i); // lvalue not pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, test<Intwrap>(), A[6], A[7], A[8]).i); // rvalue not pointer
	printf("\n");
#endif
	//--------------------------------------------------------------------------------//
#ifdef  R_VALUE_IMPLICIT_TEST
	//      <* rvalue: implicit conversion test *> (int -> Intwrap)
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T, 0, 1, 2).i); // lvalue pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T2, 3, 4, 5).i); // lvalue not pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, test<Intwrap>(), 6, 7, 8).i); // rvalue not pointer
	printf("\n");
#endif
	//--------------------------------------------------------------------------------//
#ifdef  L_VALUE_IMPLICIT_TEST
	//	<* lvalue: implicit conversion test *> (int -> Intwrap)
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T, B[0], B[1], B[2]).i); // lvalue pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, T2, B[3], B[4], B[5]).i); // lvalue not pointer
	printf("%-3d\n", call_mem_fn(&test<Intwrap>::sum, test<Intwrap>(), B[6], B[7], B[8]).i); // rvalue not pointer
	printf("\n");
#endif
	//--------------------------------------------------------------------------------//

	return 0;
}
#endif
//--------------------------------------------------------------------------------//
