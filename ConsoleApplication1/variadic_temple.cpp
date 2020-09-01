#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template< class T, class U, class P >
struct eval;

template<class T1> class A {

public:
	A() = default;
	~A() = default;

public:
	void value_setting_A(T1 A2_)
	{
		A1 = A2_;
	}
	void print()
	{
		cout << "A 호출 : " << A1 << endl;
	}

	void value_setting_vectype(vector<int> &A2_)
	{
		A2 = A2_;
	}
	void print_vectype()
	{
		int end = A2.size();

		for(int i = 0; i < end; i++)	cout << " vector call : " << A2[i] << endl;
	}
	
	T1 A1;
	vector<int> A2;
};

template<class T1, class T2> class B {

public:
	B() = default;
	~B() = default;

public:
	void value_setting_B(T1 A2_ , T2 S2_)
	{
		A_ = A2_;
		S_ = S2_;
	}
	void print()
	{
		cout << "B 호출 : " << A_  <<  " ," << S_ << endl;
	}

	T1 A_;
	T2 S_;
};

template<template<class, class...> class TT, class T, class... Rest, 
template<class, class...> class TT_, class T_, class... Rest_,
template<class, class...> class TT__, class T__, class... Rest__>
class eval<TT<T,Rest...>, TT_<T_, Rest_...> , TT__<T__, Rest__...>> {

public:

	eval() = default;
	~eval() = default;
	
	TT<T, Rest...> A;
	TT_<T_, Rest_...> B;
	TT__<T__, Rest__...> C;
};



int main()
{
	eval<A<int>, B<std::string, double>,A<vector<int>>> eA; // TT=> A,
	                 // T1 => int,
					// Rest => <>

	
	eA.A.value_setting_A(13);
	eA.A.print();
	
	eA.B.value_setting_B("mr.song", 1);
	eA.B.print();

	vector<int> vi;
	vi.emplace_back(14);
	eA.C.value_setting_vectype(vi);
	eA.C.print_vectype();
	//// TT=> B,
	//// T1 => int,
	//// Rest => float

	return 0;
}
