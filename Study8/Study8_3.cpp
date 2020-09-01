#include "stdafx.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>

///////////////////////////
// 무변동성 부분은 기반 클래스화.
template<class R>
struct _arg_impl;

template<class R, typename... ARGS>
struct _arg_impl<R(ARGS...)>
{
	virtual R operator()(ARGS&&...) {
		return R{};
	}
};

// 변동성 부분은 파생 클래스화.
template<typename Class >
struct _drived_impl;

//멤버 함수를 위한...
template<typename Class, class R, typename... ARGS >
struct _drived_impl<R(Class::*)(ARGS...)> :_arg_impl<R(ARGS...)>
{
	typedef R(Class::*MF)(ARGS...);

	Class& cls_;
	MF mfn_;

	_drived_impl(Class& cls, MF mfn) :cls_{ cls }, mfn_{ mfn } {}

	virtual R operator()(ARGS&&... args) {
		return (cls_.*mfn_)(std::forward<ARGS>(args)...);
	}
};

//일반 함수를 위한..
template<class R, typename... ARGS >
struct _drived_impl<R(ARGS...)> :_arg_impl<R(ARGS...)>
{
	typedef R(*MF)(ARGS...);

	MF mfn_;

	_drived_impl(MF mfn) :mfn_{ mfn } {}

	virtual R operator()(ARGS&&... args) {
		return (*mfn_)(std::forward<ARGS>(args)...);
	}
};

// Sugar Code
// 멤버 함수 바인딩..
template < typename Class, typename R, typename... ARGS >
auto make_member_function_bind(R(Class::*getter)(ARGS... arg), Class& obj)
{
	return std::make_shared< _drived_impl<R(Class::*)(ARGS...)> >(obj, getter);
}

// 일반 함수 바인딩..
template < typename R, typename... ARGS >
auto make_general_function_bind(R(*getter)(ARGS... arg))
{
	return std::make_shared< _drived_impl<R(ARGS...)> >(getter);
}

/////////////////////////////////////
class TTT
{
	typedef std::shared_ptr < _arg_impl<int(int, int)> > elem_type;

public:
	TTT() {};
public:
	void append(elem_type type) {
		datas_.push_back(type);
	}

	template<typename... ARGS>
	void run(ARGS&&... args) {
		for (auto& item : datas_) {
			std::cout << (*item)(std::forward<ARGS>(args)...) << std::endl;
		}
	}

public:
	std::vector <elem_type > datas_;
};

class SSS1
{
public:
	int test(int a, int b)
	{
		std::cout << "SSS1::test" << std::endl;
		return a + b;
	}
};

class SSS2
{
public:
	int test(int a, int b)
	{
		std::cout << "SSS2::test" << std::endl;
		return a * b;
	}
};

int sum1(int a, int b) {
	std::cout << "sum1" << std::endl;
	return a + b;
}

int sum2(int a, int b) {
	std::cout << "sum2" << std::endl;
	return a * b;
}
/////////////////////////////
//
//int main()
//{
//	TTT t;
//	SSS1 s1;
//	SSS2 s2;
//
//	t.append(make_member_function_bind(&SSS1::test, s1));
//	t.append(make_member_function_bind(&SSS2::test, s2));
//	t.append(make_general_function_bind(&sum1));
//	t.append(make_general_function_bind(&sum2));
//
//	t.run(2, 3);
//}