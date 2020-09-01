#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <typeinfo>

#define MAIN_ACTIVATION 0

template<typename E, std::size_t N>
void test(E(&arr)[N]) {
	for (auto && m : arr)
	{
		std::cout << m << std::endl;
	}
}
const char m[6] = "hello";
int sum(int a, int b) { return a + b; } //sum을 빼고 타입을 만들어라


//#if(TYPE_SELECTION)
//template<typename R, typename...AS>
//R fn_call(R(&fn)(AS...), AS... args)
//{
//	return fn(args...);
//}
//#else
//										// 위처럼 해도되고 아래처럼 해도되고 똑같은 의미로 볼수 있다.
//template<typename R, typename...AS>
//R fn_call(R(&fn)(AS...), AS... args)
//{
//	return fn(args...);
//}
//#endif


class BB {
public:

	BB() = default;
	~BB() = default;

	std::string sum_a(std::string _a) { return _a; }
	std::string sum_b(std::string _b) { return _b; }
	std::string sum_c(std::string _c) { return _c; }

	std::string sum(std::string a, std::string b, std::string c)
	{
		return this->sum_a(a.c_str()) + this->sum_b(b.c_str()) + this->sum_c(c.c_str());
	}

	std::string sum_sub(std::string a, std::string b, std::string c, std::string d)
	{
		return this->sum_a(a.c_str()) + this->sum_b(b.c_str()) + this->sum_c(c.c_str()) + this->sum_a(d.c_str());
	}

	template<typename R, typename B, typename...AS>
	R call_mem_fn_1(R(B::*fn)(AS...), B& obj, AS&&...args)
	{
		return (obj.*fn)(args...);
	}

	template<typename R, typename B, typename...AS>
	R call_mem_fn_1(R(B::*fn)(AS...), B* obj, AS&&...args)
	{
		return (obj->*fn)(args...);
	}


	template<typename R, typename B, typename...AS1, typename... AS2>
	R call_mem_fn_2(R(B::*fn)(AS1...), B* obj, AS2&&...args)
	{
		return (obj->*fn)(std::forward<AS2>(args)...);
	}
	template<typename R, typename B, typename...AS1, typename... AS2>
	R call_mem_fn_2(R(std::remove_reference<B>::type::*fn)(AS1...), B&& obj, AS2&&...args)
	{
		return (obj.*fn)(std::forward<AS2>(args)...);
	}

	template<typename R, typename B1, typename B2, typename...AS1, typename... AS2>
	R call_mem_fn_3(R(B1::*fn)(AS1...), B2* obj, AS2&&...args)
	{
		return (obj->*fn)(std::forward<AS2>(args)...);
	}

	template<typename R, typename B1, typename B2, typename...AS1, typename... AS2>
	R call_mem_fn_3(R(B1::*fn)(AS1...), B2&& obj, AS2&&...args)
	{
		return (obj.*fn)(std::forward<AS2>(args)...);
	}
};

class CC
{
public:
	CC() = default;
	~CC() = default;

	int sum_a(int _a) { return _a; }
	int sum_b(int _b) { return _b; }
	int sum_c(int _c) { return _c; }

	int sum(int a, int b, int c) { return this->sum_a(a) + this->sum_b(b) + this->sum_c(c); }

};

template<typename T>
struct id_type {
	using type = T;
};


template<typename R, typename B, typename...AS>
R call_mem_fn_1(R(B::*fn)(AS...), B& obj, AS...args)
{
	return (obj.*fn)(args...);
}

template<typename R, typename B, typename...AS>
R call_mem_fn_1(R(B::*fn)(AS...), B* obj, AS...args)
{
	return (obj->*fn)(args...);
}

template<typename R, typename B, typename...AS1, typename... AS2>
R call_mem_fn_2(R(B::*fn)(AS1...), B* obj, AS2&&...args)
{
	return (obj->*fn)(std::forward<AS2>(args)...);
}

template<typename R, typename B, typename...AS1, typename... AS2>
R call_mem_fn_2(R(std::remove_reference<B>::type::*fn)(AS1...), B&& obj, AS2&&...args)
{
	return (obj.*fn)(std::forward<AS2>(args)...);
}

template<typename R, typename B1, typename B2, typename...AS1, typename... AS2>
R call_mem_fn_3(R(B1::*fn)(AS1...), B2* obj, AS2&&...args)
{
	return (obj->*fn)(std::forward<AS2>(args)...);
}

template<typename R, typename B1, typename B2, typename...AS1, typename... AS2>
R call_mem_fn_3(R(B1::*fn)(AS1...), B2&& obj, AS2&&...args)
{
	return (obj.*fn)(std::forward<AS2>(args)...);
}

#if(MAIN_ACTIVATION)
int main()
{
	std::string song = "song";
	std::string min = "min";
	std::string kyu = "kyu";
	std::string sub = "sub";
	std::string good[200];

	//test(m);
	//std::cout << fn_call(sum, 10, 40) << std::endl;
	//std::cout << typeid(&BB::sum).name() << std::endl;

	BB* pbb = new BB;
	BB  rbb;

	good[0] = "song";
	good[1] = "min";
	good[2] = "kyu";

	std::vector<std::string> *pBBvec = new std::vector<std::string>;
	pBBvec->emplace_back(call_mem_fn_1(&BB::sum_sub, pbb, song, min, kyu, sub));
	pBBvec->emplace_back(call_mem_fn_1(&BB::sum, rbb, good[0], good[1], good[2]));
	pBBvec->emplace_back(call_mem_fn_2(&BB::sum_sub, pbb, song, min, kyu, sub));
	pBBvec->emplace_back(call_mem_fn_2(&BB::sum, rbb, good[0], good[1], good[2]));
	pBBvec->emplace_back(call_mem_fn_3(&BB::sum_sub, pbb, song, min, kyu, sub));
	pBBvec->emplace_back(call_mem_fn_3(&BB::sum, rbb, song, min, kyu));

	for (auto && mem_function : *pBBvec)	std::cout << mem_function << std::endl;


	CC* pcc = new CC;
	CC  rcc;

	std::vector<int> *pCCvec = new std::vector<int>;
	pCCvec->emplace_back(pbb->call_mem_fn_1(&CC::sum, pcc, 100, 200, 300 ));
	pCCvec->emplace_back(pbb->call_mem_fn_1(&CC::sum, rcc, 400, 500, 600 ));
	pCCvec->emplace_back(pbb->call_mem_fn_2(&CC::sum, pcc, 700, 800, 900 ));
	pCCvec->emplace_back(pbb->call_mem_fn_2(&CC::sum, rcc, 1000, 1100,1200 ));
	pCCvec->emplace_back(pbb->call_mem_fn_3(&CC::sum, pcc, 1300, 1400,1500 ));
	pCCvec->emplace_back(pbb->call_mem_fn_3(&CC::sum, rcc, 1600, 1700,1800 ));

	for (auto && mem_function : *pCCvec)	std::cout << mem_function << std::endl;

}
#endif
