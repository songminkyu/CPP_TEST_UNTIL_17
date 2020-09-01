#include "stdafx.h"
#include <iostream>
#include <type_traits>


// 이소스가 만들어진 이유

// CRPT 패턴
/*

문제의 발단:
crtp 로 무언가를 구현. Image 가 베이스 TiffImage 가 파생클래스
(Image<TiffImage>* pImage = new TiffImage).Draw(); 호출
Image 에서 Draw 를 강제했으나 파생클래스에서 Draw 를 구현하지 않음.
컴파일 성공(?) -> 이유를 봤더니 Image<TiffImage>::Draw(){ TiffImage(); } 인데
상속관계이고 같은 이름이기때문에 TiffImage() 에서는 베이스클래스의 Draw() 함수를 호출.
재귀의 반복 -> segment fault


문제해결의 시작 제시 1: 원상
재귀를 막기위해 베이스 클래스의 Draw 함수를 private 으로 상속하고
public 으로 이 함수를 호출하는 함수를 만듬(래핑함수).
(강제효과도 적용되고, 파생클래스에서 Draw 함수가 없다고해도 베이스클래스의
Draw 함수는 private 이기때문에 재귀호출의 문제 해결할수있음)


문제해결의 제시2: 윤훈남 선생님
이것을 타입검사를 함으로서 차단해버리자. static_assert 로 타입 검사하고 Draw()를 호출하는
새로운 함수를 만들고 직접호출대신 새로만든 함수를 통해서 사용하자.

제안 : 새로운 함수를 사용하면 타입검사가 되지만 사용자 입장에서는 직접호출도 개방되어있기때문에
문제의 여지가 있을거같다.


최종 결론 : 윤훈남 선생님
그러면 타입 검사하는 static_assert 부분을 기본클래스의 Draw() 함수에 넣어버리자.
문제가 발생하는건 재귀호출이기때문에 재귀호출을 완전히 차단해버리자.

*/


//결론 컴파일 할때 "C2338 not direct method" 표출되는게 정상.
// 당황하지말것.


struct A {
	void test() { std::cout << "#1" << std::endl; }
};

struct B :A {
	void test() { std::cout << "#2" << std::endl; }
};

template<typename D, typename B>
struct Direct_Function {
	static constexpr bool value = false;
};

template<typename D, typename C, typename R, typename... Args>
struct Direct_Function<D, R(C::*)(Args...)> {
	static constexpr bool value = std::is_same<D, C>::value;
};

//**************************
template <class Derived>
struct base {
	void foo() {
		static_assert(!Direct_Function<base<Derived>, decltype(&Derived::foo)>::value, "not direct method");
		static_cast<Derived *>(this)->foo();
	};
};

struct my_type : base<my_type> {
	void foo() { std::cout << "my_type" << std::endl; } // required to compile.
};

struct your_type : base<your_type> {
	void foo() { std::cout << "your_type" << std::endl; } // required to compile.
};

template <class T> // T is deduced at compile-time
void bar(base<T> & obj) {
	obj.foo(); // will do static dispatch
}

struct not_derived_from_base { }; // notice, not derived from base

struct not_implement_foo_for_base : base<not_implement_foo_for_base> {
	void foo() { std::cout << "ather type" << std::endl; } // required to compile.
	// 위에 foo 함수를 주석 처리 할경우 에러 
}; // notice, not derived from base

																		  //**************************
int main() {
	std::cout << Direct_Function<A, decltype(&B::test)>::value << std::endl;
	std::cout << Direct_Function<B, decltype(&B::test)>::value << std::endl;

	my_type my_instance;
	your_type your_instance;

	not_derived_from_base invalid_instance;
	not_implement_foo_for_base invalid_instance2;

	bar(my_instance); // will call my_instance.foo()
	bar(your_instance); // will call your_instance.foo()
						//bar(invalid_instance); // compile error, cannot deduce correct overload
	bar(invalid_instance2); // compile error, cannot deduce correct overload
	return 0;
}

