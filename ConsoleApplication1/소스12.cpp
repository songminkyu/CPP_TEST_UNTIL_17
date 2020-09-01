#include "stdafx.h"
#include <memory>
#include <iostream>

int allocator_support_test1()
{
	using ALLOC = std::allocator<int>;
	using ALLOC_TRAITS = std::allocator_traits<ALLOC>;

	ALLOC alloc;
	int * a = alloc.allocate(1);

	ALLOC_TRAITS::construct(alloc, a, 7);
	std::cout << *a << std::endl;
	ALLOC_TRAITS::destroy(alloc, a);


	alloc.deallocate(a, 1);
}

template <class _Ty, class _Alloc = std::allocator<_Ty>>
class A {
	using ALLOC = _Alloc;
	using ALLOC_TRAITS = std::allocator_traits<ALLOC>;

	ALLOC _alloc;
	_Ty * _value = _alloc.allocate(1);
public:
	A() {
		ALLOC_TRAITS::construct(_alloc, _value, 7);
	}
	~A() {
		ALLOC_TRAITS::destroy(_alloc, _value);
		_alloc.deallocate(_value, 1);
	}
	void print() {
		std::cout << *_value << std::endl;
	}
};


template <class _Ty, class _Alloc = std::allocator<_Ty>>
class B {
	using ALLOC = _Alloc;
	using ALLOC_TRAITS = std::allocator_traits<ALLOC>; 

	ALLOC _alloc;
	_Ty * _value = _alloc.allocate(1);			//메모리 확보
public:
	B(std::allocator_arg_t, const _Alloc& alloc) : _alloc(alloc) {
		ALLOC_TRAITS::construct(_alloc, _value, 7.3); // 생성자 호출
	}
	B() {
		ALLOC_TRAITS::construct(alloc, _value, 7.3); // 생성자 호출
	}
	~B() {
		ALLOC_TRAITS::destroy(_alloc, _value); //소멸자 호출
		_alloc.deallocate(_value, 1);          //메모리 반납
	}
	void print() {
		std::cout << *_value << std::endl;
	}
};

template<class T, template<class> class _Alloc>
B(std::allocator_arg_t, const _Alloc<T>&)->B<T, _Alloc<T>>;

int main()
{
	allocator_support_test1();

	A<int> a;
	a.print();

	std::allocator<double> alloc;
	B b(std::allocator_arg, alloc);
	b.print();
}