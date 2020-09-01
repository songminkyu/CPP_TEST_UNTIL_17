#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <functional>
#include <optional>
#include <boost/type_index.hpp>

using namespace boost::typeindex;
struct tuple_cat_t {};
constexpr tuple_cat_t tuple_cat;

template<typename T>
auto operator,(const tuple_cat_t&, const T& a) { return std::make_tuple(a); }

template<typename N, typename T1, typename... T2>
auto operator,(const std::tuple<T1, T2...>&a, const N& b) {
	return std::tuple_cat(std::tuple<T1, T2...>(a), std::make_tuple(b));
}

void test1(int, float) { std::cout << "#1\n"; }
void test2(int, float, double) { std::cout << "#2\n"; }
void test3(int a, float b, std::optional<const char*> c) {

	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << *c << std::endl;

}

int main() {
	auto tuple_root = (tuple_cat, 10);
	std::apply(test1, (tuple_root, 20.0f));
	std::apply(test2, (tuple_root, 20.0f, 30.5));
	std::apply(test3, (tuple_root, 20.0f, std::optional("hello")));

	auto z = (tuple_root, 20.0f, std::optional("hello"));
	type_index ti = type_id_with_cvr<decltype(z)>();
	std::cout << ti.pretty_name();
}
