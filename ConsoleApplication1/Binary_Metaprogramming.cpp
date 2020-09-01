#include "stdafx.h"
#include <iostream>

// 메타식 재귀 프로그래밍
// 2진수를 10진수화로 변경
template<unsigned long N>
struct binary
{
	static unsigned long const value = binary<N / 10>::value * 2 + N % 10;
};
template<>
struct binary<0UL>
{
	static unsigned long const value = 0;
};

int main()
{
	auto value_1 = binary<1011>::value;
	std::cout << value_1 << std::endl;
}