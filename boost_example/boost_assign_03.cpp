#include <iostream>
#include <map>
#include <string>
#include <boost\assign\list_of.hpp>

using namespace boost::assign;
void main() {

	std::map<std::string, int> m = map_list_of("Red", 1)("Orange", 2)("Yellow", 3)("Green", 4)
											  ("Blue", 5)("Navy", 6)("Purple", 7)("Hellow", 8);

	for (auto && item : m) {
		std::cout << item.first << ", " << item.second << std::endl;
	}
}
