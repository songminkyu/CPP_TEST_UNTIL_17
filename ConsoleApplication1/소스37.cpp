#include "stdafx.h"
#include <iostream>

struct X {  };

template<bool(FN)(X, X)> struct Y
{
	void call()
	{
		FN({}, {});
	}
};

bool test(X, X) { std::cout << "test function call \n"; return true; }
bool operator<=(X,X) { std::cout << "operator<= functor call \n"; return true; }

int main()
{
	Y<test> a;
	a.call();
	Y<operator<=> b;

	b.call();
}