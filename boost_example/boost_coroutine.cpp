#include <iostream>
#include <boost/system/system_error.hpp>
#include <boost/coroutine2/all.hpp>

int main()
{
	namespace CRT2 = boost::coroutines2;

	auto fn =
		[&](CRT2::coroutine< int >::push_type & sink) {
		int first = 1, second = 1;
		sink(first);
		sink(second);
		for (int i = 0; i < 8; ++i) {
			int third = first + second;
			first = second;
			second = third;
			sink(third);
		}
	};

	CRT2::coroutine< int >::pull_type source(fn);

	for (auto i : source)
		std::cout << i << " ";

	std::cout << "\nDone" << std::endl;
	return EXIT_SUCCESS;
}
