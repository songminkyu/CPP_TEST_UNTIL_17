#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <typeinfo>

#define TYPE_SELECTION 1

#if(TYPE_SELECTION == 1)

template<typename D, typename B>
struct Direct_Function {
	static constexpr bool value = false;
};

template<typename D, typename C, typename R, typename... Args>
struct Direct_Function<D, R(C::*)(Args...)> {
	static constexpr bool value = std::is_same<D, C>::value;
};

//----------------------------------------------------------------------------//
template <class O, class D, class R, class...AS1, class...AS2>
auto auto_arrow_call_mem_func(O&& o, R(D::*f)(AS1...), AS2&&... args)
{
	return (static_cast<D*>(autoarrow(o))->*f)(std::forward<AS2>(args)...);
}
//----------------------------------------------------------------------------//
template <class T>
auto autoarrow(T&& a) { return &a; }

template <class T>
auto autoarrow(T* a) { return a; }
//----------------------------------------------------------------------------//
template <class T>
struct struct_crtp {
	template <class R, class...AS>
	R print(AS&&... args) {
		using FN = R(AS&&...);
		static_assert(!Direct_Function<struct_crtp<T>, FN>::value,
			"error : crtp recursive function call");
		return static_cast<T*>(this)->print(std::forward<AS>(args)...);
	}
};

struct structA : public struct_crtp<structA> {
	void print() {
		printf("A : don't exist any argument\n");
	}
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("A : process : %d + %d = %d\n", a, b, c);
		return c;
	}
};

struct structB : public structA, public struct_crtp<structB> {
	void print(){
		printf("B : don't exist any argument\n");
	}
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("B : process : %d + %d = %d\n", a, b, c);
		return c;
	}
};
//----------------------------------------------------------------------------//
int main() {
	struct_crtp<structA>* a;
	struct_crtp<structB>* b;
	structA* a_;
	structB* b_;

	int x = 1, y = 2, z = 3;
	auto_arrow_call_mem_func(a, &struct_crtp<structA>::print<int, const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(a, &struct_crtp<structA>::print<void>);

	auto_arrow_call_mem_func(b, &struct_crtp<structB>::print<int, const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b, &struct_crtp<structB>::print<void>);

	auto_arrow_call_mem_func(a_, &struct_crtp<structA>::print<int, const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(a_, &struct_crtp<structA>::print<void>);

	auto_arrow_call_mem_func(b_, &struct_crtp<structA>::print<int, const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b_, &struct_crtp<structA>::print<void>);
	auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<int, const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<void>);
	
	return 0;
}
#else
template<typename D, typename B>
struct Direct_Function {
	static constexpr bool value = false;
};

template<typename D, typename C, typename R, typename... Args>
struct Direct_Function<D, R(C::*)(Args...)> {
	static constexpr bool value = std::is_same<D, C>::value;
};

//----------------------------------------------------------------------------//
template <class O, class D, class R, class...AS1, class...AS2>
auto auto_arrow_call_mem_func(O&& o, R(D::*f)(AS1...), AS2&&... args)
{
	return (static_cast<D*>(autoarrow(o))->*f)(std::forward<AS2>(args)...);
}
//----------------------------------------------------------------------------//
template <class T>
auto autoarrow(T&& a) { return &a; }

template <class T>
auto autoarrow(T* a) { return a; }
//----------------------------------------------------------------------------//
template <class T>
struct struct_crtp {
	template <class...AS>
	auto print(AS&&... args) {
		using FN = decltype(std::declval<T>().print(std::declval<AS>()...));
		static_assert(!Direct_Function<struct_crtp<T>, FN>::value,
			"error : crtp recursive function call");
		return static_cast<T*>(this)->print(std::forward<AS>(args)...);
	}
};

struct structA : public struct_crtp<structA> {
	void print() {
		printf("A : don't exist any argument\n");
	}
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("A : process : %d + %d = %d\n", a, b, c);
		return c;
	}
};

struct structB : public structA, public struct_crtp<structB> {
	void print() {
		printf("B : don't exist any argument\n");
	}
	int print(const int& a, const int& b, int& c) {
		c = a + b;
		printf("B : process : %d + %d = %d\n", a, b, c);
		return c;
	}
};
//----------------------------------------------------------------------------//

int main() {

	struct_crtp<structA>* a;
	struct_crtp<structB>* b;
	structA* a_;
	structB* b_;
	
	int x = 1, y = 2, z = 3;
	auto_arrow_call_mem_func(a, &struct_crtp<structA>::print<const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(a, &struct_crtp<structA>::print<>);

	auto_arrow_call_mem_func(b, &struct_crtp<structB>::print<const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b, &struct_crtp<structB>::print<>);

	auto_arrow_call_mem_func(a_, &struct_crtp<structA>::print<const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(a_, &struct_crtp<structA>::print<>);

	auto_arrow_call_mem_func(b_, &struct_crtp<structA>::print<const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b_, &struct_crtp<structA>::print<>);
	auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<const int&, const int&, int&>, x, y, z);
	auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<>);
	
	return 0;
}
#endif