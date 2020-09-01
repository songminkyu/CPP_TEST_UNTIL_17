
#include "stdafx.h"
#include <iostream>
/*

unnamed nested union�� ��� �޸𸮸� �����ϴ� ���� �ƽǷ��� ����
�Ʒ� ���� �ΰ��� ���� ����~
������ ������ �ٷ� �� ������ �̿��� std::any�� ���� ������

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