#include <iostream>
#include <vector>
#include <string>

template <typename Iterator>
void advance_all(Iterator & iterator) {
	++iterator;
}
template <typename Iterator, typename ... Iterators>
void advance_all(Iterator & iterator, Iterators& ... iterators) {
	++iterator;
	advance_all(iterators...);
}
template <typename Function, typename Iterator, typename ... Iterators>
Function zip(Function func, Iterator begin,
	Iterator end,
	Iterators ... iterators)
{
	for (; begin != end; ++begin, advance_all(iterators...))
		func(*begin, *(iterators)...);
	//could also make this a tuple
	return func;
}

int main() {
	std::vector<int> v1{ 1,2,3 };
	std::vector<int> v2{ 3,2,1 };
	std::vector<double> v3{ 1.2,2.4,9.0 };
	std::vector<double> v4{ 1.2,2.4,9.0 };
	zip(
		[](int i, int j, double k, double l) {
		std::cout << i << " " << j << " " << k << " " << l << std::endl;
	},
		v1.begin(), v1.end(), v2.begin(), v3.begin(), v4.begin());
}