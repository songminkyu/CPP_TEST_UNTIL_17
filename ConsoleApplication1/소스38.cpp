#include "stdafx.h"
#include <iostream>

void operator "" _print(const wchar_t * data, std::size_t s)
{
	std::wcout << data << " ," << s <<std::endl;
}

int main()
{
			LR"(
		asdasdasd
		hhhhhhhas
		213123123
		)"_print;

			"test1"_print
				L"test2"
				"test3"
				R"(
		asdasdasd
		rtrtret
		ttt
		)";

			std::cout << 10.45E2 << std::endl;
			std::cout << 0xAF.FBP2 << std::endl;

}