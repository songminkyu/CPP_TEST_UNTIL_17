
#include "stdafx.h"
#include <iostream>
#include <memory>


//shared_ptr 객체가 같은 참조 카운터 객체를 사용하는지 조사하기

struct Foo
{
	int n1;
	int n2;
	Foo(int a, int b) : n1(a), n2(b){}
};

template< typename T1, typename T2>
bool same_ref(T1 && a, T2 && b)
{
	return !a.owner_before(b) && !b.owner_before(a);
}

void main()
{
	auto p1 = std::make_shared<Foo>(1, 2);
	std::shared_ptr<int> p2(p1, &p1->n1);
	std::shared_ptr<int> p3(p1, &p1->n2);
	std::shared_ptr<int> p4(p1, nullptr);

	auto z1 = std::make_shared<Foo>(1, 2);

	std::cout << std::boolalpha
		<< "p1 과 p2 는 같은 참조 카운터 객체 사용 ???" << same_ref(p1, p2) << std::endl
		<< "p3 과 p4 는 같은 참조 카운터 객체 사용 ???" << same_ref(p3, p4) << std::endl
		<< "p1 과 z1 는 같은 참조 카운터 객체 사용 ???" << same_ref(p1, z1) << std::endl;
}




