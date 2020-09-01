#include "stdafx.h"
#include <memory>
#include <iostream>

/*

factory 객체과 std::shared_ptr
- 생성된 모든 객체의 REF는 같다.
- 생성된 모든 객체의 pointer 대상은 다르다.	
*/
struct A : std::enable_shared_from_this<A> {
	struct TIME1 {
		std::shared_ptr<A> ref_;
		TIME1(const std::shared_ptr<A>& ref) : ref_{ ref } { std::cout << "TIME1 \n"; }
		~TIME1() { std::cout << "~TIME1\n"; }
		void print() { std::cout << "OK1 \n"; }
	};

	struct TIME2 {
		std::shared_ptr<A> ref_;
		TIME2(const std::shared_ptr<A>& ref) : ref_{ ref } { std::cout << "TIME2 \n"; }
		~TIME2() { std::cout << "~TIME2\n"; }
		void print() { std::cout << "OK2 \n"; }
	};
public:
	A() = default;
	~A() {
		std::cout << "~A \n";
	}
	auto Item1() { return std::make_shared<TIME1>(this->shared_from_this()); }
	auto Item2() { return std::make_shared<TIME2>(this->shared_from_this()); }

};

int main()
{
	std::shared_ptr<A> manager = std::make_shared<A>();

	auto i1 = manager->Item1();
	auto i2 = manager->Item2();
	manager.reset();

	i1->print();
	i2->print();
}