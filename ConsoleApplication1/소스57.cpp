#include "stdafx.h"
#include <iostream>
#include <functional>
/*
멤버 변수 캡처
이니셜 라이저를 사용하여 멤버 변수를 캡처 할 수도 있습니다. 
그런 다음 멤버 변수의 복사본을 캡처하고 매달려있는 참조를 신경 쓰지 않습니다.

예를 들어 아래와 같은 코드 확인


*/
struct Baz
{
	std::function<void()> foo()
	{
		return [s = s] { std::cout << s << std::endl; };
	}
	auto auto_foo()
	{
		return [s = s] { std::cout << s << std::endl; };
	}

	std::string s;
};

int main()
{
	auto f1 = Baz{ "ala" }.foo();
	auto f2 = Baz{ "ula" }.foo();
	f1();
	f2();

	auto f3 = Baz{ "pop" }.auto_foo();
	auto f4 = Baz{ "song" }.auto_foo();
	f3();
	f4();

}