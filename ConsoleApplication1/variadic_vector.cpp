#include "stdafx.h"
#include <vector>
#include <list>
#include <iostream>

// Nice syntax to allow in-order expansion of parameter packs.


struct any {
	enum type { Int, Float, String };
	any(int   e) { m_data.INT = e; m_type = Int; }
	any(float e) { m_data.FLOAT = e; m_type = Float; }
	any(char* e) { m_data.STRING = e; m_type = String; }
	type get_type() const { return m_type; }
	int get_int() const { return m_data.INT; }
	float get_float() const { return m_data.FLOAT; }
	char* get_string() const { return m_data.STRING; }
private:
	type m_type;
	union {
		int   INT;
		float FLOAT;
		char *STRING;
	} m_data;
};

template <class ...Args>
void foo_imp(const Args&... args)
{
	std::vector<any> vec = { args... };
	for (unsigned i = 0; i < vec.size(); ++i) {
		switch (vec[i].get_type()) {
		case any::Int: std::cout << vec[i].get_int() << '\n'; break;
		case any::Float: std::cout << vec[i].get_float() << '\n'; break;
		case any::String: std::cout << vec[i].get_string() << '\n'; break;
		}
	}
}

template <class ...Args>
void foo(Args... args)
{
	foo_imp(any(args)...);  //pass each arg to any constructor, and call foo_imp with resulting any objects
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/

struct do_init_order {
	template<typename T> do_init_order(std::initializer_list<T>&&) { }
	do_init_order() = default;
	~do_init_order() = default;
};
namespace details {
	template<typename V> void container_zip_helper(V& l, const V& r) {
		l.insert(l.end(), r.begin(), r.end());
	}
	template<typename V> void container_zip_helper(V& l, V&& r) {
		l.insert(l.end(), std::make_move_iterator(r.begin()),
			std::make_move_iterator(r.end()));
	}
} // namespace details

template<typename _Elem, typename... _Elems>
std::vector<_Elem> container_multi_zip_pack(std::vector<_Elem> v1, _Elems&&... vr) {
	std::size_t s = v1.size();
	do_init_order{ s += vr.size()... };
	v1.reserve(s);
	do_init_order{ (details::container_zip_helper(v1, std::forward<_Elems>(vr)), 0)... };
	return std::move(v1);   // rvo blocked
}

template<typename _Elem, typename... _Elems>
std::list<_Elem> container_multi_zip_pack(std::list<_Elem> l1, _Elems&&... lr) {
	do_init_order{ (details::container_zip_helper(l1, std::forward<_Elems>(lr)), 0)... };
	return std::move(l1);   // rvo blocked
}


int main() {

	char s[] = "Hello";
	foo(1, s, 3.4f);
	
	/**********************************************************/

	const std::vector<int> v1{ 1,2 };
	const std::vector<int> v2{ 4 };
	const std::vector<int> v3{ 5, 6 };
	for (auto&& i : container_multi_zip_pack(v1, v2, v3))
		std::cerr << " " << i;	std::cerr << "\n";          // gcc output is:  1 2 3 5 6 4	
}

