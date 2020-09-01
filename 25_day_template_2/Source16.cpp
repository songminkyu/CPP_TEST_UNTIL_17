#include "stdafx.h"
#include <iostream>

// using ... declaration <--- 나중 검색

struct A {
	void g( const char * wchar ){
		std::cout << wchar << std::endl;
	}
};


struct B {
	void g(int _v) {
		std::cout << _v << std::endl;
	}
};

template<typename... bases>
struct X : bases...{
	using bases::g...;
};

int main() {
	X<A, B> x;
	x.g("song");
	x.g(100);

}

