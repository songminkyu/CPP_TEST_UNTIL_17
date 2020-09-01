// ConsoleApplication8.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

class endable
{
public:
	explicit endable() = default;
	~endable() = default;

public:


	template<class T>
	static typename std::enable_if<std::is_same<std::wstring, T>::value, T>::type
		compair(const std::vector<std::wstring> & r, T & l)
	{
		std::wstring str;

		for (const auto & cit : r)
		{
			if (_wcsicmp(cit.c_str(), l.c_str()) == 0)
			{
				str = l;
				break;
			}
		}
		return str;
	}


	template<class T>
	static typename std::enable_if<std::is_same<std::string, T>::value, T>::type
		compair(const std::vector<std::string> & r, T & l)
	{
		std::string str;
		for (const auto & cit : r)
		{
			if (_stricmp(cit.c_str(), l.c_str()) == 0)
			{
				str = l;
				break;
			}
		}
		return str;
	}

	template<class T>
	static typename std::enable_if<std::is_same<std::wstring, T>::value, T>::type
		compair(const std::list<std::wstring> & r, T & l)
	{
		std::wstring str;

		for (const auto & cit : r)
		{
			if (_wcsicmp(cit.c_str(), l.c_str()) == 0)
			{
				str = l;
				break;
			}
		}
		return str;
	}


	template<class T>
	static typename std::enable_if<std::is_same<std::string, T>::value, T>::type
		compair(const std::list<std::string> & r, T & l)
	{
		std::string str;
		for (const auto & cit : r)
		{
			if (_stricmp(cit.c_str(), l.c_str()) == 0)
			{
				str = l;
				break;
			}
		}
		return str;
	}


	template<typename _Ty>
	static _Ty ifind_item(const std::vector<_Ty> & r, _Ty & l)
	{
		return compair(r, l);
	}

	template<typename _Ty>
	static _Ty ifind_item(const std::list<_Ty> & r, _Ty & l)
	{
		return compair(r, l);
	}
};


int main()
{


	std::list<std::wstring> str;
	std::wstring _str = L"ddD";
	std::wstring getstr = _str.c_str();
	str.push_back(L"AAA");
	str.push_back(L"BBB");
	str.push_back(L"CCC");
	str.push_back(L"DDD");
	str.push_back(L"EEE");

	std::wcout << endable::ifind_item(str, getstr) << std::endl;

	return 0;
}

