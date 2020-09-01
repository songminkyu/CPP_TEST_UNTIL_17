#include "stdafx.h"
#include <map>
#include <iostream>


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

template<typename VALUE, typename KEY, typename...MORE_KEYS>
struct VariadicMap {
	typedef VariadicMap<VALUE, MORE_KEYS...> inner_map_type;
	typedef std::map<KEY, inner_map_type>    map_type;
	VALUE& get_ref(const KEY& k, const MORE_KEYS&...more_keys) {
		return map[k].get_ref(more_keys...);
	}
	VALUE* find(const KEY& k, const MORE_KEYS&...more_keys) {
		auto it = map.find(k);
		if (it == map.end()) { return nullptr; }
		return it->second.find(more_keys...);
	}
private:
	map_type map;
};

template <typename VALUE, typename KEY>
struct VariadicMap<VALUE, KEY> {
	VALUE& get_ref(const KEY& k) {
		return map[k];
	}
	VALUE* find(const KEY& k) {
		auto it = map.find(k);
		if (it == map.end()) { return nullptr; }
		return &(it->second);
	}
private:
	std::map<KEY, VALUE> map;
};
template< typename T>
void foo(const T & ty)
{
	if (std::is_same<const char*, T>::value)
	{
		std::cout << ty << std::endl;
	}
	else if (std::is_same<int, T>::value)
	{
		std::cout << ty << std::endl;
	}
	else
	{

	}
}
int main() {

	std::string strTest = "asdasdasdccdsdvefgfeg";
	VariadicMap< std::unique_ptr<AnyBase>, int, std::string, int> vm;
	vm.get_ref(1, "kkk", 3).reset(new Any<std::string>(strTest));
	vm.get_ref(1, "song", 3).reset(new Any<int>(5));
	

	//std::cout << "x = " << vm.get_ref(1, "min", 3) << "\n";

	auto findstrintype = vm.find(1, "kkk", 3);
	auto findInttype = vm.find(1, "song", 3);

	foo(dynamic_cast<Any<std::string>&>(*findstrintype->get()).data.c_str());
	foo(dynamic_cast<Any<int>&>(*findInttype->get()).data);

	return 10;
}