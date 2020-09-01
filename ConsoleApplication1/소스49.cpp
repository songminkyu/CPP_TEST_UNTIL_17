#include "stdafx.h"
#include <iostream>
#include <utility>
#include <array>

int whatIndex() { return 1; }
int * whatArray(int *a, int *b) { return b; }

int main() {
	int a[] = { 1,2,3,4 };
	
	for (int i = 0; i < 4; i++) {
		std::cout << i[a] << std::endl;
	}
	std::cout << 0[a] << std::endl;
	std::cout << 1[a] << std::endl;
	std::cout << 2[a] << std::endl;
	std::cout << 3[a] << std::endl;

	int b[] = { 10,20,30,40 };
	std::cout << whatIndex()[whatArray(a, b)] << std::endl;
	std::cout << whatArray(a, b)[whatIndex()] << std::endl;
	

}