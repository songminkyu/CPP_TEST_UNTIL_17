
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

/*

nontype은 사용 범위가 제한적이다.
template<double Z>  사용 불가
template<float Z>  사용 불가
template<std:string Z> 클래스와 관련된 객체타입 전부 불가능. 
이유는 소수점에 의해서 정확한 데이터를 취득할수 없기때문이다.


double m1 = 1.1;
double m2 = 1.2;
double m3 = 1.3;
auto m1 = m1 + m2 + m3;
auto m2 = m3 + m2 + m1;

if ((m1 - m2) == 0) {} // 근사치 오차가 같지 않아서 0이 성립될수 없다.

*/


//근데 위 내용에선 ㅋㅋㅋㅋ 사용할수 없다 들엇는데 아래처럼 잘만 사용된다....
//이유는 test 함수 템플릿 파라미터로 접근 하는 인스턴스가 지역 변수가 아닌 전역 변수이기때문에 가능.
//지역 변수로 접근시 오류 발생.

#define MAIN_ACTIVATION 0

#define PRAMETER_ACTIVATION 1
#define PRAMETER_ACTIVATION_SUB 0

#if(PRAMETER_ACTIVATION)
template<std::string *m>
void test()
{
	std::cout << *m << std::endl;
}

std::string m{ "hello" };
#else
template<const char * m>
void test()
{
	std::cout << m << std::endl;
}

#if(PRAMETER_ACTIVATION_SUB)
const char m[] = "heelo"; //그냥 extern타입은 오류 예: extern const char m[] = "XXXX"; 하지만 다음줄 과같은 방식은 가능
#else
extern "C" const char m[] = "hello"; // 이렇게 하면 extern 타입이 가능하다.
#endif
#endif

template<class T>
void test_1(T &it)
{
	typename T::const_iterator pos; //typename 빠지면 정적변수.

	for (pos = it.begin(); pos != it.end(); ++pos)
	{
		std::cout << *pos << std::endl;
	} //Items are protected all thanks to const_iterator. 
}


class A {
public:
	A() {
		std::cout << "A클래스 호출" << std::endl;
	}
};
void A() { std::cout << "A함수 호출" << std::endl; }




//스텝 4 이름찾기
struct AAA
{
	using type = int;
};

struct BBB_1
{
	int type;
};
//위와같은 경우 타입이 1개라 상관 없지만
//아래와같은 경우 타입결정이 모호하기때문에 에러발생.
struct BBB_2
{
	//static int type;
	using type = int;
	//일반 변수가 타입 변수와 경쟁에서이기지만, 오류가 발셍, 즉 2타입 중 하나만 사용 가능. 
};

template<class T>
void test()
{
	typename T::type pos;
}

#if (MAIN_ACTIVATION)

int main()
{
#if(PRAMETER_ACTIVATION)
	// 불가능한 조건은 객체가 지역일경우 test 함수애서 오류가 발생
	//std::string m{ "hello" }; 
	test<&m>(); // m이 지역이면 불가능


#else
	test<m>(); // m이 전역일경우 가능
#endif

	std::vector<double> vp_double;

	vp_double.emplace_back(1.1);
	vp_double.emplace_back(1.2);
	vp_double.emplace_back(1.3);
	test_1<std::vector<double>>(vp_double);


	A(); //A 는 무슨 타입?? A 일반 함수가 호출
		
	class A a;  //하지만 클래스 A를 호출 하기위해 아래와 같은 방법으로 사용 rear backward 뭐시기라고 하던데..

	//결론 호출이 둘다 되네???? 


	test<BBB_2>();
}
#endif // (MAIN_ACTIVATION)
