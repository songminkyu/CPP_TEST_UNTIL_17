#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <map>

int main()
{
	std::map<std::string, int> _map;

	_map.insert({ "song", 123 });
	_map.insert({ "min", 11 });
	_map.insert({ "kyu", 88 });

	std::for_each(std::begin(_map), std::end(_map),
		[](auto& entry) {
			std::cout << entry.first << " = " << entry.second << '\n';
		}
	);

}