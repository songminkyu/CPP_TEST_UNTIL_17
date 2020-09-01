#include "stdafx.h"
#include <iostream>

template <typename T> struct eval {
	void print() {
		std::cout << "#1" << std::endl;
	}
};

template<template<typename,typename...> typename TT, typename T1, typename...Rest>
struct eval<TT<T1, Rest...>> {
	void print() {
		std::cout << "#2" << std::endl;
	}
};

template <typename T1>									struct A;
template <typename T1, typename T2>						struct B;
template <int N>										struct C;
template <class T1, int N>								struct D;
template <typename T1,typename T2, int N = 17>			struct E;

int main() {
	eval<A<int>> a;
	a.print();

	eval<B<int, float>> b;
	b.print();

	eval<C<17>> c;
	c.print();

	eval<D<int,17>> D;
	D.print();

	eval<E<int, float>> E;
	E.print();


}

