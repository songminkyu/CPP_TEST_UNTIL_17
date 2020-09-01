#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
struct A
{
	std::mutex lock_a, lock_b;

	void run1()
	{
		for (int i = 0; i < 1000; i++)
		{
			std::scoped_lock m(lock_a, lock_b);
			std::cout << "run1\n" << i;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	void run2()
	{
		for (int i = 0; i < 1000; i++)
		{
			std::scoped_lock m(lock_b, lock_a);
			std::cout << "run2\n" << i;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

};

void main()
{
	A a;

	std::thread m1(&A::run1, std::ref(a));
	std::thread m2(&A::run2, std::ref(a));

	m1.join(); m2.join();
}