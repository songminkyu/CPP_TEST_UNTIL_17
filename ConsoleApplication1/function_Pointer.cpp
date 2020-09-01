#include "stdafx.h"
#include <iostream>
#include <array>
#include <string>

struct S {
	int x;
	struct Foo {
		int i;
		int j;
		int a[3];
	}b;
};


void del_arg(int argc, char **argv, int index)
{
	int i;
	for (i = index; i < argc - 1; ++i) argv[i] = argv[i + 1];
	argv[i] = 0;
}

int find_int_arg(int argc, char **argv, char *arg, int def)
{	
	int i;
	for (i = 0; i < argc - 1; ++i) {

		if (!argv[i]) continue;
		if (0 == strcmp(argv[i], arg)) {
			def = atoi(argv[i + 1]);
			del_arg(argc, argv, i);
			del_arg(argc, argv, i);
			break;
		}
	}
	return def;
}

void test1() { std::cout << "test_1" << std::endl; }
void test2() { std::cout << "test_2" << std::endl; }

struct CollectFoo {
	void test() const { std::cout << "void test() const " << std::endl; }
	void test1() const& { std::cout << "void test()& const " << std::endl; }
	void test1() const&& { std::cout << "void test()&& const " << std::endl; }
};

void two_dimension() 
{		
	char *argv[100];
	
	argv[0] = "song.exe";
	argv[1] = "detector";
	argv[2] = "demo";
	argv[3] = "cfgcocodata";
	argv[4] = "-c";
	argv[5] = "100";

	int cam_index = find_int_arg(7, argv, "-c", 0);
	std::cout << "cam_index = "<<  cam_index << std::endl;

}

struct concepts {
	void(*AA)(int, int) noexcept;
	void(*BB)(const char*, const char *) noexcept;
};

void AA(int a, int b) noexcept {
	std::cout << a << "  " << b << std::endl;
}
void BB(const char*a, const char* b) noexcept {
	std::cout << a << std::endl;
	std::cout << b << std::endl;
}
int main()
{
	concepts _vtable{AA,BB};
	_vtable.AA(1, 2);
	_vtable.BB("song", "min");

	two_dimension();
	void(*FNS[2])() = {
		&test1,
		&test2
	};

	for (auto & item : FNS) {
		item();
	}

	CollectFoo cf;
	void (CollectFoo::* fn_test)() const = &CollectFoo::test;
	(cf.*fn_test)();

	void (CollectFoo::* fn_test2)() const& = &CollectFoo::test1;
	(cf.*fn_test2)();

	void (CollectFoo::* fn_test3)() const&& = &CollectFoo::test1;
	(std::move(cf).*fn_test3)();

	S s1 = { 1,{2,3,{4,5,6}} };
	S s2 = { 1,2,3,4,5,6 };
	S s3{ 1,{2,3,{4,5,6}} };
	S s4{ 1,2,3,4,5,6 };

	for (const auto & g : s4.b.a)
	{
		std::cout << g << std::endl;
	}
	std::string args[] = { std::string("one"),
						 "two",
						 {'t','h','r','e','e'} };


	for (const auto & s : args)
	{
		std::cout << s.c_str() << std::endl;
	}
}
