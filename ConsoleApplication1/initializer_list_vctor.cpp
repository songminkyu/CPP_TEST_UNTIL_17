#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <initializer_list>
#include <algorithm>

//#define MAIN_ACTIVE

std::initializer_list<int> test_function();

template <class T, int S>
class test {
public:
	using type = T;
	size_t size() { return S; }

	test() {};

	template <class...AS>
	test(AS... args) : data{ { args... } } {};

	template <int S2>
	test& operator=(const std::array<T, S2>& i) {
		size_t copy_size = (S < S2) ? S : S2;
		std::copy_n(i.begin(), copy_size, data.begin());
		return *this;
	}

	template <int S2>
	test& operator=(test<T, S2>& i) {
		size_t copy_size = (S < S2) ? S : S2;
		std::copy_n(i.begin(), copy_size, data.begin());
		return *this;
	}

	auto begin() {
		return data.begin();
	}

	void printtest() {
		for (auto& iter : data) {
			std::cout << iter << " ";
		}
	}

private:
	std::array<T, S> data;
};

class MyNumber
{
public:

	MyNumber(const std::initializer_list<int> &v)
	{
		for (auto itm : v)
		{
			mVec.push_back(itm);
		}
	}

	void print() 
	{
		for (auto itm : mVec) 
		{
			std::cout << itm << " ";
		}
	}
private:
	std::vector<int> mVec;
	int array_[100];
};

class A
{
public:
	int mx;
	double my;
};

class B
{
public:
	B(int x, double y) : mx{ x }, my{ y } {}

	int mx;
	double my;
};

class C
{
public:
	C(int x, double y) : mx{ x }, my{ y } {}
	C(const std::initializer_list<int>& v) {
		mx = *(v.begin());
		my = *(v.begin() + 1);
	}

	int mx;
	double my;
};

class D
{
	int arr[3];
public:
	D(int x, int y, int z) : arr{ x, y, z }
	{

	}
	void print()
	{
		std::cout << std::endl;
		std::cout << "D : " << arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
	}
};

std::initializer_list<int> test_function()
{
	return{ 1,2,3 };
}



#ifdef MAIN_ACTIVE
int main()
{
	auto p = test_function();

	test<char, 4> t_char_4{ 'a','b','c','d' };
	test<char, 2> t_char_2{ 'e','f' };
	MyNumber m = { 1, 2, 3, 4 };
	// Aggregate initialization
	A a{ 1, 3.7 };
	// Regular constructor
	B b{ 2, 9.8 };
	// Initializer_list
	C c{ 3, 7 };

	D d{ 1,2,3 };
	std::cout << a.mx << " " << a.my << std::endl;
	std::cout << b.mx << " " << b.my << std::endl;
	std::cout << c.mx << " " << c.my << std::endl;
	m.print();  // 1 2 3 4
	d.print();

	std::cout << "***********************************" << std::endl;
	
	
	for (auto && m : p) {
		std::cout << m << std::endl;
	}

	/********************************************************************/

	t_char_4.printtest();
	printf("\n");
	t_char_2.printtest();
	printf("\n");

	t_char_4 = t_char_2;
	t_char_4.printtest();
	printf("\n");
	t_char_2.printtest();
	printf("\n");
	
	return 0;
}
#endif