
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

/*

nontype�� ��� ������ �������̴�.
template<double Z>  ��� �Ұ�
template<float Z>  ��� �Ұ�
template<std:string Z> Ŭ������ ���õ� ��üŸ�� ���� �Ұ���. 
������ �Ҽ����� ���ؼ� ��Ȯ�� �����͸� ����Ҽ� ���⶧���̴�.


double m1 = 1.1;
double m2 = 1.2;
double m3 = 1.3;
auto m1 = m1 + m2 + m3;
auto m2 = m3 + m2 + m1;

if ((m1 - m2) == 0) {} // �ٻ�ġ ������ ���� �ʾƼ� 0�� �����ɼ� ����.

*/


//�ٵ� �� ���뿡�� �������� ����Ҽ� ���� ����µ� �Ʒ�ó�� �߸� ���ȴ�....
//������ test �Լ� ���ø� �Ķ���ͷ� ���� �ϴ� �ν��Ͻ��� ���� ������ �ƴ� ���� �����̱⶧���� ����.
//���� ������ ���ٽ� ���� �߻�.

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
const char m[] = "heelo"; //�׳� externŸ���� ���� ��: extern const char m[] = "XXXX"; ������ ������ ������ ����� ����
#else
extern "C" const char m[] = "hello"; // �̷��� �ϸ� extern Ÿ���� �����ϴ�.
#endif
#endif

template<class T>
void test_1(T &it)
{
	typename T::const_iterator pos; //typename ������ ��������.

	for (pos = it.begin(); pos != it.end(); ++pos)
	{
		std::cout << *pos << std::endl;
	} //Items are protected all thanks to const_iterator. 
}


class A {
public:
	A() {
		std::cout << "AŬ���� ȣ��" << std::endl;
	}
};
void A() { std::cout << "A�Լ� ȣ��" << std::endl; }




//���� 4 �̸�ã��
struct AAA
{
	using type = int;
};

struct BBB_1
{
	int type;
};
//���Ͱ��� ��� Ÿ���� 1���� ��� ������
//�Ʒ��Ͱ��� ��� Ÿ�԰����� ��ȣ�ϱ⶧���� �����߻�.
struct BBB_2
{
	//static int type;
	using type = int;
	//�Ϲ� ������ Ÿ�� ������ ���￡���̱�����, ������ �߼�, �� 2Ÿ�� �� �ϳ��� ��� ����. 
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
	// �Ұ����� ������ ��ü�� �����ϰ�� test �Լ��ּ� ������ �߻�
	//std::string m{ "hello" }; 
	test<&m>(); // m�� �����̸� �Ұ���


#else
	test<m>(); // m�� �����ϰ�� ����
#endif

	std::vector<double> vp_double;

	vp_double.emplace_back(1.1);
	vp_double.emplace_back(1.2);
	vp_double.emplace_back(1.3);
	test_1<std::vector<double>>(vp_double);


	A(); //A �� ���� Ÿ��?? A �Ϲ� �Լ��� ȣ��
		
	class A a;  //������ Ŭ���� A�� ȣ�� �ϱ����� �Ʒ��� ���� ������� ��� rear backward ���ñ��� �ϴ���..

	//��� ȣ���� �Ѵ� �ǳ�???? 


	test<BBB_2>();
}
#endif // (MAIN_ACTIVATION)
