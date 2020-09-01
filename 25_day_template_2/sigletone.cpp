#include "stdafx.h"
#include "sigletone.h"
#include <iostream>


class A : public CSAbstractVarialdicDynamicSingleton< A >
{
public:
	A() = default;
	A(char *_b, int * _c) : b(_b), c(_c){

	}
	~A() = default;

	void print() {
		std::cout << b << "   " << (int)c << std::endl;
	}
private:
	char * b;
	int *  c;
};

template <typename T> T * CSAbstractVarialdicDynamicSingleton<T>::_instance = nullptr;

int main() {
	A * a = A::GetInstance((char*)"song",(int*)100);
	a->print();
}