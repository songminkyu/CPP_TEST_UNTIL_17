#include "stdafx.h"
#include <iostream>
#include <vector>

template<class Container>
class _Inserter
{
	Container& c_;
public:
	_Inserter(Container & c) : c_{ c } {}

	template<class V2>
	auto& operator, (V2 v)
	{
		c_.push_back(v);
		return *this;
	}
};

template< class... Args, class V2>
auto operator+=(std::vector<Args...>& c, V2 v)
{
	c.push_back(v);
	return _Inserter<std::vector<Args...>>(c);
}

int main()
{


	std::vector<int> input;
	input += 1, 1, 1, 2, 2, 3, 4, 4, 5, 5, 5;
	for (auto & item : input) {
		std::cout << item << std::endl;
	}

	std::vector<const char*> input_cc;
	input_cc += "song", "min", "kyu";
	for (auto & item : input_cc) {
		std::cout << item;
	}
}
