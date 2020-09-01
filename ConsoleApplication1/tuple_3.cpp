#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

#define MAX 100
using namespace std;
// tuple_element

template <typename T>
void xtuple_size(const T& a)
{
	cout << T::N << endl;
}

// tuple (primary template)
template <typename ...Types> class xtuple;

// empty tuple (specialization)
template <> class xtuple<> {};

// recursive tuple definition (partial specialization)

template <typename T, typename ... Types>
class xtuple<T, Types...> : private xtuple<Types ...>
{
public:
	xtuple() {}
	xtuple(const T& a, Types ... args) : a_{ a,  args... }
	{}
	
	void print()
	{
		cout << a_[0] << "\n" << a_[1] << "\n" << a_[2] << endl;
	}
	enum { N = 1 + sizeof...(Types) };

public:
	T a_[MAX];
};


//void main()
//{
//	xtuple<double ,double, double> t(1.1,1.2,1.3);
//	t.print();
//}