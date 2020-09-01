// templete_test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

//#define MAIN_ACITVE

template<typename T>
struct TY {
public:
	template<typename... AS>
	void test(AS&&... as) {
		T* obj = static_cast<T*>(this);
		obj->rawTest(std::forward<AS>(as)...);
	}
};
struct CD :TY<CD> {
	void rawTest(int) { printf("#3"); }
	void rawTest(int, double, std::string) { printf("#4"); }
};

struct AB :TY<AB> {
	void rawTest() { printf("#1"); }
	void rawTest(int, std::string, double) { printf("#2"); }
};

#ifdef MAIN_ACTIVE
int main()
{
	TY<CD> a;
	a.test(1);
	a.test(1, 1, "abcd");
	return 0;
}
#endif

