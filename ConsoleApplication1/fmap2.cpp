#include "stdafx.h"
#include <iostream>
#include <map>
#include <typeindex>
#include <memory>
#include <utility>
#define SELECT_TYPE 3
struct sub_function_base
{
	virtual ~sub_function_base() {}
};

template< typename _Ret, typename ..._Arg >
struct sub_function : sub_function_base
{
	typedef _Ret(*_Func)(const _Arg&...);

	_Func m_f;

	sub_function(_Func f) : m_f(f) {}
	virtual ~sub_function() {}

	_Ret operator()(const _Arg& ...arg)
	{
		return std::move((*m_f)(std::forward<const _Arg>(arg)...));
	}
};

int f(const int& _i) { return _i; }
double f(const double& _d) { return _d; }
std::string f(const std::string &_s) { return _s; }

int main(int argc, char* argv[])
{
#if SELECT_TYPE == 1
	std::map< std::type_index, std::shared_ptr< sub_function_base > > test;
	std::string strstr = "song";

	test[typeid(int)] = std::shared_ptr< sub_function_base >(new sub_function< int, int >(f));
	test[typeid(double)] = std::shared_ptr< sub_function_base >(new sub_function< double, double >(f));
	test[typeid(std::string)] = std::shared_ptr< sub_function_base >(new sub_function< std::string, std::string >(f));
	try
	{
		std::cout << "int : ";
		std::cout << dynamic_cast< sub_function< int, int >* >(test[typeid(int)].get())->operator()(50) << std::endl;
		std::cout << "double : ";
		std::cout << dynamic_cast< sub_function< double, double >* >(test[typeid(double)].get())->operator()(50.1) << std::endl;
		std::cout << "string : ";
		std::cout << dynamic_cast< sub_function< std::string, std::string >* >(test[typeid(std::string)].get())->operator()(strstr.c_str()) << std::endl;
	}
	catch (std::bad_cast e)
	{
		std::cout << e.what() << std::endl;
	}
#elif SELECT_TYPE == 2
	std::map< std::string, std::shared_ptr< sub_function_base > > test;
	std::string strstr = "song";

	test["INT_TYPE"] = std::shared_ptr< sub_function_base >(new sub_function< int, int >(f));
	test["DOUBLE_TYPE"] = std::shared_ptr< sub_function_base >(new sub_function< double, double >(f));
	test["STRING_TYPE"] = std::shared_ptr< sub_function_base >(new sub_function< std::string, std::string >(f));
	try
	{
		std::cout << "int : ";
		std::cout << dynamic_cast< sub_function< int, int >* >(test["INT_TYPE"].get())->operator()(50) << std::endl;
		std::cout << "double : ";
		std::cout << dynamic_cast< sub_function< double, double >* >(test["DOUBLE_TYPE"].get())->operator()(50.1) << std::endl;
		std::cout << "string : ";
		std::cout << dynamic_cast< sub_function< std::string, std::string >* >(test["STRING_TYPE"].get())->operator()(strstr.c_str()) << std::endl;
	}
	catch (std::bad_cast e)
	{
		std::cout << e.what() << std::endl;
	}
#elif SELECT_TYPE == 3

	std::map< std::string, std::unique_ptr< sub_function_base > > test;
	std::string strstr = "song";

	test["INT_TYPE"] = std::unique_ptr< sub_function_base >(new sub_function< int, int >(f));
	test["DOUBLE_TYPE"] = std::unique_ptr< sub_function_base >(new sub_function< double, double >(f));
	test["STRING_TYPE"] = std::unique_ptr< sub_function_base >(new sub_function< std::string, std::string >(f));
	try
	{
		std::cout << "int : ";
		std::cout << dynamic_cast< sub_function< int, int >* >(test["INT_TYPE"].get())->operator()(50) << std::endl;
		std::cout << "double : ";
		std::cout << dynamic_cast< sub_function< double, double >* >(test["DOUBLE_TYPE"].get())->operator()(50.1) << std::endl;
		std::cout << "string : ";
		std::cout << dynamic_cast< sub_function< std::string, std::string >* >(test["STRING_TYPE"].get())->operator()(strstr.c_str()) << std::endl;
	}
	catch (std::bad_cast e)
	{
		std::cout << e.what() << std::endl;
	}

#endif
	return 0;
}