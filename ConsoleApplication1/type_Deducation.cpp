#include "stdafx.h"
#include <iostream>
#include <string>

struct _Tag;


void call(std::string _Tag::*) {
	std::cout << "#1 \n";
}

void call(int _Tag::*) {
	std::cout << "#2 \n";
}

int main()
{
	call((std::string _Tag::*)(nullptr));
	call((int _Tag::*)(nullptr));

}