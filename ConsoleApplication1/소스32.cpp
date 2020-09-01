#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <string>
#include <vector>

int main()
{
	using v_tuple_t = std::tuple<std::string, int>;

	std::vector< v_tuple_t > v_tuple;


	v_tuple.push_back({ "song",11 });
	v_tuple.push_back({ "min",21 });
	v_tuple.push_back({ "kyu",34 });

	for (auto &&[first, second] : v_tuple)
	{
		std::cout << "first : " << first << " ," << "second : " << second << std::endl;
	}

}

