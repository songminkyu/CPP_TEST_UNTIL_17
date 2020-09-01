
// 이 코드는 Windows OS, Linux (우분투) 18.04 테스트가 정상적으로 됫음을 알립니다.
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>

namespace detail
{
	template <typename Iterator>
	void advance_all(Iterator & iterator) {
		++iterator;
	}
	template <typename Iterator, typename ... Iterators>
	void advance_all(Iterator & iterator, Iterators& ... iterators) {
		++iterator;
		advance_all(iterators...);
	}
	template <typename Function, typename Iterator, typename ... Iterators>
	Function zip(Function func, Iterator begin,
		Iterator end,
		Iterators ... iterators)
	{
		for (; begin != end; ++begin, advance_all(iterators...))
			func(*begin, *(iterators)...);
		//could also make this a tuple
		return func;
	}

	template <typename Iter, typename... Iters>
	class zip_iterator {
	public:
		using value_type = std::tuple<const typename Iter::value_type&,
			const typename Iters::value_type&...>;

		zip_iterator() = default;
		zip_iterator(const Iter &head, const Iters&... tail)
			: head_(head), tail_(tail...) { }

		virtual ~zip_iterator() = default;

		value_type operator*() const {
			return std::tuple_cat(std::tuple<const typename Iter::value_type&>(*head_), *tail_);
		}

		zip_iterator& operator++() {
			++head_; ++tail_;
			return *this;
		}

		bool operator==(const zip_iterator &rhs) const {
			return head_ == rhs.head_ && tail_ == rhs.tail_;
		}

		bool operator!=(const zip_iterator &rhs) const {
			return !(*this == rhs);
		}

	private:
		Iter head_;
		zip_iterator<Iters...> tail_;
	};

	template <typename Iter>
	class zip_iterator<Iter> {
	public:
		using value_type = std::tuple<const typename Iter::value_type&>;

		zip_iterator() = default;
		zip_iterator(const Iter &head) : head_(head) { }

		virtual ~zip_iterator() = default;

		value_type operator*() const {
			return value_type(*head_);
		}

		zip_iterator& operator++() { ++head_; return *this; }

		bool operator==(const zip_iterator &rhs) const { return head_ == rhs.head_; }

		bool operator!=(const zip_iterator &rhs) const { return !(*this == rhs); }

	private:
		Iter head_;
	};

	template <typename Iter>
	class seq {
	public:
		using iterator = Iter;

		seq() = default;
		seq(const Iter &begin, const Iter &end) : begin_(begin), end_(end) { }
		virtual ~seq() = default;
		iterator begin() const { return begin_; }
		iterator end() const { return end_; }
	private:
		Iter begin_, end_;
	};

	template <typename... Seqs>
	seq<zip_iterator<typename Seqs::const_iterator...>> zip(const Seqs&... seqs)
	{
		return seq<zip_iterator<typename Seqs::const_iterator...>>(
			zip_iterator<typename Seqs::const_iterator...>(std::begin(seqs)...),
			zip_iterator<typename Seqs::const_iterator...>(std::end(seqs)...));
	}

}

void main()
{
	std::vector<int> vi{ 1,2,3 };
	std::list<std::string> vs{ "song","min" };

	// 컨테이너 사이즈는 모두 다 동일해야 함....
	for (const auto & ci : detail::zip(vi, vs))
	{
		/*std::cout << std::get<0>(ci) << std::endl;
		std::cout << std::get<1>(ci).c_str() << std::endl;*/
	}
	if (vi.size() != vs.size())
	{
		std::cout << "size 다릅니다" << std::endl;
	}
	else
	{
		

		std::cout << "~~~~~ for_each ~~~~~~~" << std::endl;

		/*detail::zip([](int & i, std::string &str)
		{
			std::cout << i << std::endl;
			std::cout << str << std::endl;
		}, vi.begin(), vi.end(), vs.begin());*/
	}
	

}