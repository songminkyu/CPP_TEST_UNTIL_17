
#include "stdafx.h"
#include <iostream>
/*

unnamed nested union는 모두 메모리를 공유하는 것을 아실려나 ㅋㅋ
아래 스샷 두개는 같은 구조~
마지막 스샷은 바로 이 원리를 이용한 std::any의 내부 데이터

*/
struct A1 {
	union {
		int _head;
		struct A2 {
			union 
			{
				float _head;
			};
			int _key;

		}_tail;
	};
};

struct B1 {
	union {
		float _head;
	};
	int _key;
};

struct B2 {
	union {
		int _head;
		B1 _tail;
	};
};
void print(A1 & a) {
	std::cout << "key : " << a._tail._key << "  _head : " << a._head << std::endl;
}

void print(B2 & a) {
	std::cout << "key : " << a._tail._key << "  _head : " << a._head << std::endl;
}

int main()
{
	A1 a;
	a._tail._key = 100;
	a._head = 10;			print(a);
	a._tail._head = 20;	    print(a);

	B2 b;
	b._tail._key = 100;
	b._head = 10;			print(b);
	b._tail._head = 20;	    print(b);
}