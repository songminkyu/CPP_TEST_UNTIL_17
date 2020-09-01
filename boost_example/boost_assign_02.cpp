
#include <boost\assign\list_inserter.hpp>
#include <string>
#include <deque>
#include <tuple>
#include <iostream>

using namespace boost::assign;
int main() {

	using str_pair = std::pair<std::string, std::string>;
	std::deque<str_pair> str_dqe;

	push_front(str_dqe)
		("foo", "bar")
		("bar", "foo");

	for (auto && item : str_dqe)
	{
		std::cout << item.first << ", " << item.second << std::endl;
	}


	str_dqe.clear();

	push_back(str_dqe)
		("foo", "bar")
		("bar", "foo");

	for (auto && item : str_dqe)
	{
		std::cout << item.first << ", " << item.second << std::endl;
	}

}