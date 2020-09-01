#include "stdafx.h"
#include <iostream>
#include <string>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

int main() {
	std::variant<int, float, std::string> intFloatString{ "Hello" };
	std::visit(overload{
		[](const int& i) { std::cout << "int: " << i; },
		[](const float& f) { std::cout << "float: " << f; },
		[](const std::string& s) { std::cout << "string: " << s; }
		},
		intFloatString
	);
}