#include "stdafx.h"
#include <iostream>
#include <string>

#if 1
template<class R,  class ARG>
auto fmap_impl(R(&fn)(ARG))
{
	auto lambda_fn = 
		[](decltype(fn)& dn)
	{
		auto arg_fn = [dn = dn](auto && arg)->R {return dn(arg);};
		return arg_fn;
	};
	return lambda_fn(fn);
}
template<class R, class ARG, class...RS, class... ARGS>
auto fmap_impl(R(&fn)(ARG), RS(&...fns)(ARGS))
{
	auto lambda_fn = 
		[fns...,fn](auto arg)
	{
		return fmap_impl(fns...)(fmap_impl(fn)(arg));
	};
	return lambda_fn;
}
template<class... RS, class... ARGS>
auto fmap(RS(&...fns)(ARGS))
{
	return fmap_impl(fns...);
}

int add10(int a) { return a + 10; };
int add20(int a) { return a + 20; };
int add30(int a) { return a + 30; };
int add40(int a) { return a + 40; };

std::string hellostring(int a)
{
	return std::to_string(a) + "=>hello world";
}
//int main()
//{
//	auto fn = fmap(add10, add20, add30, add40 ,hellostring);
//	std::cout << fn(30) << std::endl;
//	return 0;
//}
#else
template< typename R, typename ARG>
auto fmap_impl(R(&fn)(ARG)) {
	auto lambda_fn =
		[](auto& dn) {
		auto arg_fn = [&dn](auto arg) { return dn(arg); };
		return arg_fn;
	};
	return lambda_fn(fn);
}

template< typename R, typename ARG, typename... RS, typename... ARGS>
auto fmap_impl(R(&fn)(ARG), RS(&...fns)(ARGS)) {
	auto lambda_fn =
		[&fns..., &fn](auto arg) {
		return fmap_impl(fns...)(fmap_impl(fn)(arg));
	};
	return lambda_fn;
}

template< typename... RS, typename... ARGS>
auto fmap(RS(&...fns)(ARGS)) {
	return fmap_impl(fns...);
}

int add10(int a) { return a + 10; }
int add20(int a) { return a + 20; }

std::string hellostring(std::string a) {
	return std::string(a) + "  =>hello world";
}

std::string hellostring_(int a) {
	return std::to_string(a) + "  =>hello world";
}
int stringlen(std::string& str) {
	return str.size();
}

int main() {
	auto fn = fmap(add10, add20, add10, add20, hellostring_);
	std::cout << fn(30) << std::endl;


	auto fns = fmap(hellostring);
	std::cout << fns("so so") << std::endl;
	return 0;
}
#endif