#include "stdafx.h"
#include <iostream>
#include <functional>

/*
이 코드는 Baz객체를 선언 한 다음 호출합니다 foo(). 클래스의 멤버를 캡처하는 foo()람다 
(에 저장 됨 std::function) 를 반환합니다 .

우리가 임시 개체를 사용하기 때문에, 우리는 당신이 호출 할 때 무슨 일이 일어날 지 확신 할 수 없다 
f1하고 f2. 이것은 매달려있는 참조 문제이며 정의되지 않은 동작을 생성합니다.

*/
struct Baz
{
	std::function<void()> foo()
	{
		return [=] { std::cout << s << std::endl; };
	}

	std::string s;
};

int main()
{
	auto f1 = Baz{ "ala" }.foo();
	auto f2 = Baz{ "ula" }.foo();
	f1();
	f2();
}