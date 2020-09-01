#include "stdafx.h"
#include <iostream>
#include <functional>
/*
��� ���� ĸó
�̴ϼ� �������� ����Ͽ� ��� ������ ĸó �� ���� �ֽ��ϴ�. 
�׷� ���� ��� ������ ���纻�� ĸó�ϰ� �Ŵ޷��ִ� ������ �Ű� ���� �ʽ��ϴ�.

���� ��� �Ʒ��� ���� �ڵ� Ȯ��


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