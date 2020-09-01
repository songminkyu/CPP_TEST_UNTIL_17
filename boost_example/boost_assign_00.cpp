#include <iostream>
#include <boost\assign\std\vector.hpp>

using namespace boost::assign;
void main() {
	
	std::vector<int> values;
	values += 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;

	for (auto && item : values) {
		std::cout << item << " ";
	}

}