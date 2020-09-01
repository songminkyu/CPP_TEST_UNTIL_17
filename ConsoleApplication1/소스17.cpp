#include "stdafx.h"
#include <iostream>
#include <locale>
#include <iomanip>

struct my_tf : std::numpunct<char>
{
private:
	std::string do_truename()   const override { return "참 입니다."; }
	std::string do_falsename() const override { return "거짓 입니다."; }
	~my_tf()
	{
		std::cout << "~my_tf() called!!";
	}
};
bool test()
{
	return true;
}
int main()
{

	using namespace std;
	{
		cout << std::boolalpha;
		auto loc = std::locale(cout.getloc(), new my_tf);
		auto pre_loc = cout.imbue(loc);
		
		cout << true << std::endl;
		cout << false << std::endl;
		(void)cout.imbue(pre_loc);
	}
}