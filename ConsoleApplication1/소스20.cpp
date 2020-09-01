#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <array>
void hellow_world()
{
	std::ofstream f("test.txt");
	f << "hellow world !! \n";
	f << "songminkyu !! \n";
}

template <class InputStream>
struct IStreamContainer {
	using stream_type   = InputStream;
	using char_type     = typename stream_type::char_type;
	using traits_type   = typename stream_type::traits_type;
	using iterator_type = std::istreambuf_iterator<char_type, traits_type>;

	stream_type & f;
	IStreamContainer(stream_type & f) : f(f){}

	iterator_type begin() const { return { f }; }
	iterator_type end() const { return { }; }

};


int main()
{
	hellow_world();

	std::ifstream f("test.txt");
	std::string str;
	std::array<std::string,255> str_array;
	int i = 0;
	for (auto && c : IStreamContainer(f))
	{
		if (c != '\n')			
		{
			str += c;			
		}
		else
		{				
			str_array[i] = str.c_str();
			str.clear();
			i++;
		}
	}

	
	for (const auto & i : str_array)
	{	
		if (i.empty())
			break;
		else
			std::cout << i << std::endl;
	}

	f.close();
}