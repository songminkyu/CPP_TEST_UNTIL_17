#include "stdafx.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>


//���������� �ݺ����� operator*, operator++�� �����ؾ� �Ѵ�.
// �ݺ��ڸ� �����Ϸ��� �����̳��� begin,end �� �����ؾ��Ѵ�.

template<class>
struct indexed_iterator;

template<
	template<class...>class Container,
	class... Args
>
struct indexed_iterator<Container<Args...>> : Container<Args...>::iterator
{
	using value_type = typename Container<Args...>::value_type;
	using iterator = typename Container<Args...>::iterator;

	struct ReElem {
		value_type & value_;
		std::size_t& index_;
	};

	std::size_t idx_{ 0 };
	indexed_iterator(iterator itr) : iterator(itr) {  };

	auto operator++()
	{
		++idx_;
		return this->iterator::operator++();
	}
	auto operator*()
	{
		return ReElem{ this->iterator::operator*(),this->idx_ };
	}
};
//2. �����̳� ����
template<class>
struct indexed_container;

template<
	template<typename...>class Container,
	typename... Args
>
struct indexed_container<Container<Args...>>
{
	using wrap_iterator = indexed_iterator<Container<Args...>>;

	Container<Args...>& c_;

	indexed_container(Container<Args...>& c) : c_{ c } { }

	wrap_iterator begin()
	{
		using std::begin;
		return begin(c_);
	}
	wrap_iterator end()
	{
		using std::end;
		return end(c_);
	}
};

//2. �����ڵ�
template<
	typename... Args,
	template<class...>class Container
>
auto make_indexed(Container<Args...>& container) {
	return indexed_container<Container<Args...>>(container);
}

int main()
{
	std::vector<int> input = { 10,20,30,40,50,60,70,80,90,100 };
	for (const auto& element : make_indexed(input))
	{
		std::cout << "Element = " << element.value_ << " "
			<< "Index = " << element.index_
			<< std::endl;
	}
}