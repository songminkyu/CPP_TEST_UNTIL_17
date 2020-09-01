#include <vector>
#include <list>
#include <iostream>
#include <windows.h>
#include <boost/range/combine.hpp>
template<class... _Elems>
static auto multi_zip_range(_Elems&... conts) -> decltype(boost::make_iterator_range(
	boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
	boost::make_zip_iterator(boost::make_tuple(conts.end()...))
))
{
	return{
		boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
		boost::make_zip_iterator(boost::make_tuple(conts.end()...))
	};
}

int main()
{

	std::vector<std::string> str1;
	std::vector<std::string> str2;
	std::vector<std::string> str3;

	str1.push_back("song"); str2.push_back("min"); str3.push_back("kyu");
	str1.push_back("kim"); str2.push_back("tea"); str3.push_back("hong");
	str1.push_back("good"); str2.push_back("OK"); str3.push_back("YES");

	if (!str1.empty() && !str2.empty() && !str3.empty())
	{
		for (auto && item : multi_zip_range(str1, str2, str3))
		{
			std::cout << item.get<0>().c_str() << "  " << item.get<1>().c_str() << "  " << item.get<2>().c_str() << std::endl;
		}
	}
}