#ifndef REDI_INDEX_SEQ_H
#define REDI_INDEX_SEQ_H

// Copyright Jonathan Wakely 2012-2015
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus < 201402L
# include <cstddef>
#else
# include <utility>
#endif

#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>

#include <algorithm>
#include <iterator>
#include <utility>
#include <type_traits>
#include <cstddef>

#include <vector>
#include <string>
#include <iostream>
namespace redi
{
#if __cplusplus < 201402L

	/// A type that represents a parameter pack of zero or more integers.
	template<std::size_t... Indices>
	struct index_sequence
	{
		/// Generate an index_sequence with an additional element.
		template<std::size_t N>
		using append = index_sequence<Indices..., N>;
	};

	/// Unary metafunction that generates an index_sequence containing [0, Size)
	template<std::size_t Size>
	struct make_index_sequence
	{
		using type
			= typename make_index_sequence<Size - 1>::type::template append<Size - 1>;
	};

	// Terminal case of the recursive metafunction.
	template<>
	struct make_index_sequence<0u>
	{
		typedef index_sequence<> type;
	};

	template<typename... Types>
	using index_sequence_for
		= typename make_index_sequence<sizeof...(Types)>::type;

#else
	using std::index_sequence;
	using std::make_index_sequence;
	using std::index_sequence_for;
#endif

}  // namespace redi

#endif  // REDI_INDEX_SEQ_H 

#ifndef REDI_ZIP_H
#define REDI_ZIP_H

// Copyright Jonathan Wakely 2012-2015
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { template<typename Iter> class iterator_range; }

namespace redi
{
	template<typename... Container>
	struct make_iterator_tuple
	{
		template<typename Cont>
		using iterator = decltype(std::begin(std::declval<Cont&>()));

		typedef boost::tuple<iterator<Container>...> type;
	};

	template<typename T, typename Iter = decltype(std::begin(std::declval<T&>()))>
	struct is_derived_from_boost_iterator_range
		: std::is_base_of<boost::iterator_range<Iter>, T>
	{ };

	template<typename T,
		bool = is_derived_from_boost_iterator_range<T>::value>
		class has_size_impl
	{
		template<typename U, typename = decltype(std::declval<U&>().size())>
		static std::true_type test(U*);

		static std::false_type test(...);

	public:
		using type = decltype(test(std::declval<T*>()));
	};

	// Workaround for https://svn.boost.org/trac/boost/ticket/8011
	template<typename T>
	struct has_size_impl<T, true> : std::false_type
	{ };

	template<typename T>
	struct has_size : has_size_impl<T>::type
	{ };

	template<typename... Container>
	class zipper
	{
		typedef boost::tuple<Container...> container_tuple;

		typedef index_sequence_for<Container...> indices;

		typedef typename make_iterator_tuple<Container...>::type iterator_tuple;

	public:
		typedef boost::zip_iterator<iterator_tuple> iterator;

		static_assert(!std::is_same<typename iterator::iterator_category,
			std::input_iterator_tag>::value,
			"Cannot zip input iterators");

		explicit
			zipper(Container&&... c)
			: conts(std::forward<Container>(c)...), minmax_size(get_sizes(indices()))
		{ }

		zipper& operator=(const zipper&) = delete;

		iterator begin()
		{
			return boost::make_zip_iterator(get_begin_tuple(indices()));
		}

		iterator end()
		{
			if (minmax_size.first == minmax_size.second)
				return boost::make_zip_iterator(get_end_tuple(indices()));
			auto iter = begin();
			std::advance(iter, minmax_size.first);
			return iter;
		}

		std::size_t size() const noexcept { return minmax_size.first; }

		bool empty() const noexcept { return minmax_size.first == 0; }

	private:
		container_tuple                           conts;
		std::pair<std::size_t, std::size_t>       minmax_size;

		template<std::size_t... Indices>
		auto
			get_begin_tuple(index_sequence<Indices...>)
			-> decltype(boost::make_tuple(std::begin(boost::get<Indices>(conts))...))
		{
			return boost::make_tuple(std::begin(boost::get<Indices>(conts))...);
		}

		template<std::size_t... Indices>
		auto
			get_end_tuple(index_sequence<Indices...>)
			-> decltype(boost::make_tuple(std::end(boost::get<Indices>(conts))...))
		{
			return boost::make_tuple(std::end(boost::get<Indices>(conts))...);
		}

		template<std::size_t... Indices>
		std::pair<std::size_t, std::size_t>
			get_sizes(index_sequence<Indices...>)
		{
			return std::minmax({ get_size(boost::get<Indices>(conts))... });
		}

		template<typename T>
		static typename std::enable_if<has_size<T>::value, std::size_t>::type
			get_size(T& t)
		{
			return t.size();
		}

		template<typename T>
		static typename std::enable_if<!has_size<T>::value, std::size_t>::type
			get_size(T& t)
		{
			return std::distance(std::begin(t), std::end(t));
		}

		template<typename T, std::size_t N>
		static std::size_t
			get_size(T(&)[N]) noexcept
		{
			return N;
		}
	};

	/// Object generator to allow boost::zip_iterator to be used with range-for
	/**
	 * std::vector<int> vi{ 0, 2, 4 };
	 * std::vector<std::string> vs{ "1", "3", "5" };
	 * for (auto i : redi::zip(vi, vs))
	 *   std::cout << i.get<0>() << ' ' << i.get<1>() << ' ';
	 */
	template<typename... Containers>
	zipper<Containers...>
		zip(Containers&&... conts)
	{
		return zipper<Containers...>(std::forward<Containers>(conts)...);
	}

}  // namespace redi

#endif  // REDI_ZIP_H

// vi: set ft=cpp sw=2: 

int main() {

	std::vector<int> vi{ 0, 2, 4 ,6};
	std::vector<std::string> vs{ "1", "3", "5", "7" };
	for (auto i : redi::zip(vi, vs))
	{		
		//std::cout << i.get<0>() << ' ' << i.get<1>() << ' ';
		std::cout << i.get<0>() << std::endl;
		std::cout << i.get<1>() << std::endl;
	}

	return 0;
}