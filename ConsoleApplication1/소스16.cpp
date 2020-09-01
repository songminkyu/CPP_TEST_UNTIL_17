#include "stdafx.h"
#include <iostream>
#include <locale>
#include <iomanip>

struct my_tf : std::numpunct<char>
{
private:
	char do_thousands_sep()   const override { return '\''; }
	std::string do_grouping() const override { return "1\2\3\5"; }
};

int main()
{

	using namespace std;
	{
		auto loc = std::locale(cout.getloc(), new my_tf);
		auto pre_loc = cout.imbue(loc);

		cout << std::numeric_limits<unsigned long long>::max();
		(void)cout.imbue(pre_loc);
	}
}