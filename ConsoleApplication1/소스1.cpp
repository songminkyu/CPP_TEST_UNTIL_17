#include "stdafx.h"
#include <iostream>

class B{};
class A {
public:
	A(int){}

	static void * operator new(std::size_t v, B) {
		std::cout << "#1" << std::endl;
		return ::operator new(v);
	}
	static void operator delete(void* p, std::size_t v, B) {
		std::cout << "#1_1" << std::endl;
		return ::operator delete(p);
	}

	static void operator delete(void* p, B) {
		std::cout << "#1_2" << std::endl;
		return ::operator delete(p);
	}

	static void operator delete(void *p, std::size_t){
		std::cout << "#1_3" << std::endl;
		return ::operator delete(p);
	}

	static void operator delete(void *p){
		std::cout << "#1_4" << std::endl;
		return ::operator delete(p);
	}

};

int main()
{
	B b;
	std::cout << "no exception call" << std::endl;

	A* a = new (b) A(10);
	delete a;

	std::cout << "exception call" << std::endl;

	try {
		A* a = new(b) A((throw "Hellow World \n", 10));
		delete a;
	}
	catch (...)
	{
		std::cout << "..." << std::endl;
	}
}

	
