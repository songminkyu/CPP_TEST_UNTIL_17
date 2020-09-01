#include "stdafx.h"
#include <iostream>
#include <type_traits>


// �̼ҽ��� ������� ����

// CRPT ����
/*

������ �ߴ�:
crtp �� ���𰡸� ����. Image �� ���̽� TiffImage �� �Ļ�Ŭ����
(Image<TiffImage>* pImage = new TiffImage).Draw(); ȣ��
Image ���� Draw �� ���������� �Ļ�Ŭ�������� Draw �� �������� ����.
������ ����(?) -> ������ �ô��� Image<TiffImage>::Draw(){ TiffImage(); } �ε�
��Ӱ����̰� ���� �̸��̱⶧���� TiffImage() ������ ���̽�Ŭ������ Draw() �Լ��� ȣ��.
����� �ݺ� -> segment fault


�����ذ��� ���� ���� 1: ����
��͸� �������� ���̽� Ŭ������ Draw �Լ��� private ���� ����ϰ�
public ���� �� �Լ��� ȣ���ϴ� �Լ��� ����(�����Լ�).
(����ȿ���� ����ǰ�, �Ļ�Ŭ�������� Draw �Լ��� ���ٰ��ص� ���̽�Ŭ������
Draw �Լ��� private �̱⶧���� ���ȣ���� ���� �ذ��Ҽ�����)


�����ذ��� ����2: ���Ƴ� ������
�̰��� Ÿ�԰˻縦 �����μ� �����ع�����. static_assert �� Ÿ�� �˻��ϰ� Draw()�� ȣ���ϴ�
���ο� �Լ��� ����� ����ȣ���� ���θ��� �Լ��� ���ؼ� �������.

���� : ���ο� �Լ��� ����ϸ� Ÿ�԰˻簡 ������ ����� ���忡���� ����ȣ�⵵ ����Ǿ��ֱ⶧����
������ ������ �����Ű���.


���� ��� : ���Ƴ� ������
�׷��� Ÿ�� �˻��ϴ� static_assert �κ��� �⺻Ŭ������ Draw() �Լ��� �־������.
������ �߻��ϴ°� ���ȣ���̱⶧���� ���ȣ���� ������ �����ع�����.

*/


//��� ������ �Ҷ� "C2338 not direct method" ǥ��Ǵ°� ����.
// ��Ȳ��������.


struct A {
	void test() { std::cout << "#1" << std::endl; }
};

struct B :A {
	void test() { std::cout << "#2" << std::endl; }
};

template<typename D, typename B>
struct Direct_Function {
	static constexpr bool value = false;
};

template<typename D, typename C, typename R, typename... Args>
struct Direct_Function<D, R(C::*)(Args...)> {
	static constexpr bool value = std::is_same<D, C>::value;
};

//**************************
template <class Derived>
struct base {
	void foo() {
		static_assert(!Direct_Function<base<Derived>, decltype(&Derived::foo)>::value, "not direct method");
		static_cast<Derived *>(this)->foo();
	};
};

struct my_type : base<my_type> {
	void foo() { std::cout << "my_type" << std::endl; } // required to compile.
};

struct your_type : base<your_type> {
	void foo() { std::cout << "your_type" << std::endl; } // required to compile.
};

template <class T> // T is deduced at compile-time
void bar(base<T> & obj) {
	obj.foo(); // will do static dispatch
}

struct not_derived_from_base { }; // notice, not derived from base

struct not_implement_foo_for_base : base<not_implement_foo_for_base> {
	void foo() { std::cout << "ather type" << std::endl; } // required to compile.
	// ���� foo �Լ��� �ּ� ó�� �Ұ�� ���� 
}; // notice, not derived from base

																		  //**************************
int main() {
	std::cout << Direct_Function<A, decltype(&B::test)>::value << std::endl;
	std::cout << Direct_Function<B, decltype(&B::test)>::value << std::endl;

	my_type my_instance;
	your_type your_instance;

	not_derived_from_base invalid_instance;
	not_implement_foo_for_base invalid_instance2;

	bar(my_instance); // will call my_instance.foo()
	bar(your_instance); // will call your_instance.foo()
						//bar(invalid_instance); // compile error, cannot deduce correct overload
	bar(invalid_instance2); // compile error, cannot deduce correct overload
	return 0;
}

