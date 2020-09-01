#include "stdafx.h"
#include <iostream>
#include <type_traits>

template<typename V, typename Tag >
struct A {
	V value_;
};


template<typename T, typename Tag>
constexpr auto Named_Type() {
	auto fn = []() {};
	return A<T, decltype(fn)>{};
}

using Age = decltype(Named_Type<int, struct _Age>());
using Weight = decltype(Named_Type<int, struct _Weight>());

struct Person {
	Age   age_ = { 1 };
	Weight weight_ = { 2 };
};

int main() {
	Person obj;
	obj.age_ = Age{ 10 };
	obj.weight_ = Weight{ 100 };
	//obj.age_ = Weight{10}; // ERROR

	printf("%d", obj.weight_);
}