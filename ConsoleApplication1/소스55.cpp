#include "stdafx.h"
#include <iostream>
#include <functional>

/*
�� �ڵ�� Baz��ü�� ���� �� ���� ȣ���մϴ� foo(). Ŭ������ ����� ĸó�ϴ� foo()���� 
(�� ���� �� std::function) �� ��ȯ�մϴ� .

�츮�� �ӽ� ��ü�� ����ϱ� ������, �츮�� ����� ȣ�� �� �� ���� ���� �Ͼ �� Ȯ�� �� �� ���� 
f1�ϰ� f2. �̰��� �Ŵ޷��ִ� ���� �����̸� ���ǵ��� ���� ������ �����մϴ�.

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