#include "stdafx.h"
#include <vector>
#include <functional>//�Լ��� �Ȱ��� Ÿ������ �ٲ��ִ� ���̺귯��
#include <iostream>
#include <memory>//����Ʈ ������
using namespace std;

class TTT
{
private:
//using Ÿ�Ե���
//std::vector<std::function<bool(int, int)>> fns_;
//��Ÿ���� �Ʒ� Ÿ��ó�� ������ ->  using typedef ������� �ٲ�
	using f_type = std::function<bool(int, int)>;
	std::vector<f_type> fns_;
public:

	void append(f_type fn)
	{
		this->fns_.push_back(fn);
	}
	void run()
	{
		std::cout << this->fns_.size() << std::endl;
		for (auto&& item : fns_)
		{
		bool v = item(10, 20);
		}
	}
};

bool _sum(int, int)
{
	return true;
}
class A
{
public:
	bool test(int a, int b) {
	std::cout << "A::test(int,int)" << a<<","<< b << std::endl;
	return false; }
	A()
	{
		cout << "A()" << endl;
	}
	~A()
	{
		cout << "~A()" << endl;
	}

};

class B {
public:
//Ŭ������� �η�⵵ ������ �Լ� ��ü��� �θ���.
	bool operator()(int, int)
	{
		return true;
	}
};

//int main()
//{
//
////���� 1,
//
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//���ٽ�
//
//	using namespace std::placeholders;
//	A a;
//	ttt.append(
//		std::bind(&A::test, &a, _1, _2)
//	);//���ε�� &A::test�� a �� ��� �ϳ��� �Լ��� �߱��Ѵ�.
//
//
//	ttt.run();
//
//   return 0;
//
///*********************************************/
//
////���� 2,
//
////���ε�� ���� ���� ���� ����
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//���ٽ�
//
//	using namespace std::placeholders;
//	A a;
//	auto n = std::bind(&A::test, &a, 0, _3)(0, 0, 1);
//
//	ttt.append(std::bind(&A::test, &a, 0, _1));
//
//
//	ttt.run();
//
//	return 0;
//
///*************************************************/
////���� 3,
////�Լ� ��ü
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//���ٽ�
//	B fn;
//	fn(23, 40);
//	ttt.append(fn);
//
//	ttt.run();
//	return 0;
//
///*************************************************/
////���� 4
//
//	TTT ttt;
//	ttt.append(_sum);
//	ttt.append([](int, int)->auto{return true; });//���ٽ�
//
////������ �� �ı��� ���� ȣ��
//	using namespace std::placeholders;
//	{
//		auto p = std::make_shared<A>();
//		//A a;
//		auto n = std::bind(&A::test, p, 0, _1);
//	}
//
//	ttt.run();
//
//	return 0;
//
//}