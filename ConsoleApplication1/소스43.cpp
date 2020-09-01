#include "stdafx.h"
#include <iostream>
#include <string>
struct A {
	void operator()(int,int){} //#1
	using FNREF = void(&)(std::string, std::string);//#2
	operator FNREF() { //#3
		extern void fn1(std::string, std::string);
		return fn1;
	}
};

struct B :A {
	void operator()(int){} //#2 -- #1과 같은 이름 operator()임으로 #1을 hidden 한다.
	using FNREF = void(&)(std::string);//#2
	operator FNREF() { //#4 -- #3과 #4 은 이름이 서로 다르다, 따라서 hidden 되지 않는다.
		extern void fn2(std::string);
		return fn2;
	}
};

void fn1(std::string, std::string) { std::cout << "#3" << std::endl; }
void fn2(std::string ) { std::cout << "#4" << std::endl; }

int main()
{
	B b;
	b(1);

	b("song");
	b("he", "song");
	
}