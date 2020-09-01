#include "stdafx.h"
#include <iostream>
#include <map>


class AnyBase
{
public:
	virtual ~AnyBase() = default;
};

template<class T>
class Any : public AnyBase
{
public:
	typedef T Type;
	explicit Any(const Type& data) : data(data) {}
	Any() {}
	Type data;
};

int main()
{
	std::map<std::string, std::unique_ptr<AnyBase>> anymap;
	anymap["number"].reset(new Any<int>(5));
	anymap["text"].reset(new Any<std::string>("5"));

	// throws std::bad_cast if not really Any<int>
	int value = dynamic_cast<Any<int>&>(*anymap["number"]).data;

	std::cout << value << std::endl;
}
