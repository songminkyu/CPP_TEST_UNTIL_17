
#include "stdafx.h"
#include <deque>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;
#define MAIN_ACTIVATION 0
//������ ������ ���� �ٶ�;  Stack<T,CONT>&operator = (Stack <T2,CONT2>) 



#if(MAIN_ACTIVATION)
template<template<typename ...T> UU, typename... As>
void test() {
	UU<AS...> m;
}

template<typename T>
class A {};

int main()
{
	test<std::vector>();
	test<std::map>();
	test<std::unordered_map>();

}
#endif