#include "stdafx.h"
#include <vector>
#include <functional>//함수를 똑같은 타입으로 바꿔주는 라이브러리
#include <iostream>
#include <memory>//스마트 포인터
using namespace std;

class TTT
{
private:
//using 타입디에프
//std::vector<std::function<bool(int, int)>> fns_;
//위타입을 아래 타입처럼 변형함 ->  using typedef 방식으로 바꿈
	using f_type = std::function<bool(int, int)>;
	std::vector<f_type> fns_;
public:

	void append(f_type fn)
	{
		this->fns_.push_back(fn);
	}
	void run()
	{
		std::cout << this->fns_.size() << std::endl;
		for (auto&& item : fns_)
		{
		bool v = item(10, 20);
		}
	}
};

bool _sum(int, int)
{
	return true;
}
class A
{
public:
	bool test(int a, int b) {
	std::cout << "A::test(int,int)" << a<<","<< b << std::endl;
	return false; }
	A()
	{
		cout << "A()" << endl;
	}
	~A()
	{
		cout << "~A()" << endl;
	}

};

class B {
public:
//클래스라고 부루기도 하지만 함수 객체라고도 부른다.
	bool operator()(int, int)
	{
		return true;
	}
};

//int main()
//{
//
////스텝 1,
//
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//람다식
//
//	using namespace std::placeholders;
//	A a;
//	ttt.append(
//		std::bind(&A::test, &a, _1, _2)
//	);//바인드는 &A::test와 a 을 묶어서 하나의 함수를 발굴한다.
//
//
//	ttt.run();
//
//   return 0;
//
///*********************************************/
//
////스텝 2,
//
////바인드로 통해 스왑 구조 구현
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//람다식
//
//	using namespace std::placeholders;
//	A a;
//	auto n = std::bind(&A::test, &a, 0, _3)(0, 0, 1);
//
//	ttt.append(std::bind(&A::test, &a, 0, _1));
//
//
//	ttt.run();
//
//	return 0;
//
///*************************************************/
////스텝 3,
////함수 객체
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//람다식
//	B fn;
//	fn(23, 40);
//	ttt.append(fn);
//
//	ttt.run();
//	return 0;
//
///*************************************************/
////스텝 4
//
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//람다식
//
////생성자 및 파괴자 동시 호출
//	using namespace std::placeholders;
//	{
//		auto p = std::make_shared<A>();
//		//A a;
//		auto n = std::bind(&A::test, p, 0, _1);
//	}
//
//	ttt.run();
//
//	return 0;
//
//}