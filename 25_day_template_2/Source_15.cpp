#include "stdafx.h"
#include <iostream>

template< typename T>	class A {};
template< typename T, typename U = T> class B{};
template< typename... Args>	class C{};
template< typename T, typename ...Args>	class D {};
template< typename T, typename T1, typename T3, typename ...Args > class E {};

//template <auto N>				class D{}; 역시 마소는 쓰레기다....

template <template<typename> typename P>	  class X{};
template <template<typename...> typename Q>	  class Y{};
template <template<int> class R> class Z{};

int main() {
	// [[maybe_unused]] 이거 워닝 문구가 발생되지 않음.
	[[maybe_unused]] X<A> xa;
	[[maybe_unused]] X<B> xb;
	[[maybe_unused]] X<C> xc;
	[[maybe_unused]] X<D> xd;
	//[[maybe_unused]] X<E> xd; //Error
	[[maybe_unused]] Y<A> ya;
	[[maybe_unused]] Y<B> yb;
	[[maybe_unused]] Y<C> yc;
	[[maybe_unused]] Y<D> yd;
	[[maybe_unused]] Y<E> ye;
	
}