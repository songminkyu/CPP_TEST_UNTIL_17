#include "stdafx.h"
#include <iostream>
#include <mutex>

template<typename CHILD>
class CRTP_Moniter {
	std::mutex lock;
	struct __locked {
		CHILD* obj;
		std::scoped_lock<std::mutex> lock;
		__locked(CHILD* obj, std::mutex& lock) :obj{ obj }, lock{ lock }{	std::cout << "locked\n"; }
		~__locked() { std::cout << "unlocked\n"; }
		CHILD*  operator->() { return obj; }
	};

public:
	__locked operator->() { return { static_cast<CHILD*>(this), this->lock }; }
};

class A :public CRTP_Moniter<A> {
public:
	void test() {
		std::cout << "test\n";
	}
};

int main() {
	A obj;
	//lock 모드로 실행하기
	obj->test();
	// no lock 모드로 실행하기
	obj.test();
}
