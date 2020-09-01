#include "stdafx.h"
#include <locale>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

template<class _Elem, class _Traits>
std::basic_istream<_Elem ,_Traits>&
operator >> (std::basic_istream<_Elem, _Traits> & lhs, struct tm& t) {
	using value_type = std::basic_istream<_Elem, _Traits>;
	using char_type = typename value_type::char_type;
	
	auto loc = lhs.getloc();
	std::ios_base::iostate err = std::ios_base::goodbit;

	auto & time_get_facet = std::use_facet<std::time_get<char_type>>(loc);
	time_get_facet.get_time({ lhs }, {}, lhs, err, &t);
	lhs.setstate(err);

	return lhs;
}

int main()
{
	using namespace std;

	string s("21:30:11");
	istringstream istr(s);
	std::ios_base::iostate err = std::ios_base::goodbit;

	auto loc = istr.getloc();
	auto & time_get_facet = std::use_facet<std::time_get<char>>(loc);

	std::tm t;
	auto ret = time_get_facet.get_time({ istr }, {}, istr, err, &t);
	istr.setstate(err);
	if (istr) 
	
	{
		cout << "Houser : " << t.tm_hour << ' '
			<< "Minutes : " << t.tm_min << ' '
			<< "Seconds : " << t.tm_sec << std::endl;		
	}
	else
	{
		cout << "Parse failed . Unparsed string : ";
		copy(ret, {},std::ostreambuf_iterator<char>(cout));
		cout << "\n";
	}
	
	std::wstring input = L"2011-Februar-18 23:12:34";
	std::wistringstream ss(input);
	ss.imbue(std::locale("de_DE"));
	ss >> std::get_time(&t, L"%Y-%b-%d %H:%M:%S"); // uses std::time_get<wchar_t>
	std::cout << std::asctime(&t);

	wstring ws(L"22:30:34");
	wistringstream wstr(ws);

	wstr >> t;

	if(wstr){
		cout << "Houser : " << t.tm_hour << ' '
			<< "Minutes : " << t.tm_min << ' '
			<< "Seconds : " << t.tm_sec << std::endl;
	}

}