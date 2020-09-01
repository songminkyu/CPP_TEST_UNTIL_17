#include "stdafx.h"
#include <iostream>
#include <mutex>

class Monitor {
	std::mutex lock;

	struct __locked {
		Monitor * _obj;
		std::scoped_lock<std::mutex> _lock;
		__locked(Monitor * obj, std::mutex & lock) : _obj{ obj }, _lock{ lock } {
			std::cout << "locked \n ";
		}
		~__locked() {
			std::cout << "unlocked" << std::endl;
		}
		Monitor * operator->()
		{
			return _obj;
		}

	};
public:
	void test()
	{
		std::cout << "test" << std::endl;

	}

	__locked operator->()
	{
		return { this,lock };
	}
};

int main()
{
	Monitor obj;
	obj->test();
	obj.test();

}