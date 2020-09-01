#include "stdafx.h"
#include <iostream>
#include <string>
#include <variant>



struct Circle { void Draw() const { std::cout << "Circle" << std::endl; } };
struct Square { void Draw() const { std::cout << "Square" << std::endl; } };
struct Triangle { void Draw() const { std::cout << "Triangle" << std::endl; } };

int main() {
	std::variant<Circle, Square, Triangle> shape;
	shape = Triangle{};
	auto callDraw = [](auto& sh) { sh.Draw(); };
	std::visit(callDraw, shape);
}

