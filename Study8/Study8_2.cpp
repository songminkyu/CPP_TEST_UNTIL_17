// Study8.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <condition_variable>
#include <mutex>
#include <deque>
#include <functional>
#include <atomic>
#include <vector>
#include <functional>//�Լ��� �Ȱ��� Ÿ������ �ٲ��ִ� ���̺귯��
#include <iostream>
#include <memory>//����Ʈ ������
#include <windows.h>
using namespace std;



//���ø� Ư��ȭ, �̷��� ���ø��� ����ϱ� ���ؼ� �� �����̸Ӹ� ���ø��� �־���Ѵ�.
//�����ͷ� ������ ���������� �Ϲ� �ν��Ͻ����� ������ �Ұ����ϴ�.


using lock_t = std::unique_lock<std::mutex>;
//notification_queue
template<class ...Args>
class notification_queue
{
	std::deque<std::function<void(Args...)>>		_q;
	std::mutex									_mutex;
	std::condition_variable						_ready;
	std::atomic_bool							_end = false;
public:
	void done() {
		_end = true; // ���� ���·� �ٲ��� ��� ���� ������ �˶�.
		_ready.notify_all();
	}

	bool pop(std::function< void(Args...) > & x) {// ���� ������ ����Ǵ� �Լ�.
		lock_t lock{ _mutex };
		_ready.wait(lock, [=] { return !_q.empty() || _end; });//!! ���� �ñ׳��� �ְų�, �� ���°� �ƴ��̸� ���.
		if (_end && _q.empty()) return false; // ���� �ñ׳ΰ� ť�� ����ִٸ� false ��ȯ

		x = std::move(_q.front());
		_q.pop_front();
		return true;
	}

	bool try_pop(std::function< void(Args...) > & x) {
		lock_t lock{ _mutex , std::try_to_lock };
		if (!lock || _q.empty()) return false;

		x = std::move(_q.front());
		_q.pop_front();
		return true;
	}

	template<typename F>
	void push(F&& f) {
		{
			lock_t lock{ _mutex }; //
			_q.emplace_back(std::forward<F>(f));
		}
		_ready.notify_one();
	}

	template<typename F>
	bool try_push(F&& f) {
		{
			lock_t lock{ _mutex, std::try_to_lock };
			if (!lock)   return   false;
			_q.emplace_back(std::forward<F>(f));
		}
		_ready.notify_one();
		return true;
	}
};

template<class ...Args>
class task_system
{
	const unsigned										_count{ std::thread::hardware_concurrency() };
	std::vector<std::thread>							_threads;
	std::vector<notification_queue<Args...>>			_q{ _count };
	std::atomic<unsigned>								_index{ 0 };
	double _x_value;
	double _y_value;
	double _z_value;
	double _data_selection;

	void run(unsigned i) {
		while (true) {
			std::function<void(Args...)> f;
			for (unsigned n = 0; n != _count; ++n) {
				if (_q[(i + n) % _count].try_pop(f)) break;
			}
			if (!f && !_q[i].pop(f)) break;
			//f();
			f(_x_value, _y_value, _z_value, _data_selection);
		}
	}
public:

	task_system() {
		for (unsigned n = 0; n != _count; ++n) {
			_threads.emplace_back([&, n] { run(n); });
		}
	}

	~task_system() {
		for (auto & e : _q)        e.done();
		for (auto & e : _threads) e.join();
	}


	template<typename F>
	auto async_(F&& f, double x, double y, double z, double s) {

		_x_value = x;
		_y_value = y;
		_z_value = z;
		_data_selection = s;
	
		auto i = _index++;
		for (unsigned n = 0; n != _count; ++n)
		{
			if (_q[(i + n) % _count].try_push(std::forward<F>(f)))
			{
				return;
			}
		}
		_q[i % _count].push(std::forward<F>(f));

	}
};


#include <iostream>
#include <sstream>
#include <chrono>
class TimeCheck {
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
public:
	TimeCheck() {
		start_ = std::chrono::high_resolution_clock::now();
	}
	~TimeCheck() {
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "running time:" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count() << std::endl;
	}
};

/******************************************************************************************************************/
//���� 2
template<typename T>//�����̸Ӹ�(�⺻) ���ø�
struct impl_function;//impl_function<T> ���� ���� �ϳ��� ������ ����.

template<typename R, typename...Args>
struct impl_function <R(Args...)>
{
	//R value or L Value
	virtual R operator()(Args&&... args) = 0 //int, float, int
	{
		//�̷��� �����ϴ°���, ���� ���̽����� �ޱ� ���ؼ�
		return R{};
	}
};


//impl_function ��� -> impl_sub_function
template<class T>
struct impl_sub_function;

template<class R, class... Args>
struct impl_sub_function<R(Args...)> : impl_function<R(Args...)>
{
	//���� private �ȿ� �־���� �Ʒ� 3�� ������ 
	using type = R(*)(Args...);//�Լ� ������ Ÿ��
	type fn_;
	
	impl_sub_function(type fn) : fn_(fn) {}
	
	//R value or L Value
	R operator()(Args&&... args) override
	{
		return (*fn_)(std::forward<Args>(args)...);
	}
};

//Ŭ���� �� �Լ� ȣ��....
template<class C ,class R, class... Args>
struct impl_sub_function< R(C::*)(Args...) > : impl_function< R(Args...) >
{
	// < R(C::*)(Args...) > �� Ŭ������ ������ �ִ� �Լ� ������
	//���� private �ȿ� �־���� �Ʒ� 3�� ������
	using type = R(C::*)(Args...);//�Լ� ������ Ÿ��
	type fn_;

	//C& obj_;
	C obj_; //������ �����ؼ� ���� �ִ°ɷ�...

	impl_sub_function(type fn, C& obj) : fn_(fn), obj_(obj)
	{

	}

	//R value or L Value
	R operator()(Args&&... args) override
	{
		//.* �ɹ����� ������. 
		return ( /*Ÿ��*/ obj_.*fn_ /*��Į��(�ɼ�)*/ )(std::forward<Args>(args)...);
	}
};

//���� �ڵ� : ���α׷��Ӱ� �ƴ� �������� �߷��Ѵ�.
template<class R, class... Args>
auto make_class_member_function_sugar_type_1(R(*fn)(Args...))
{
	//impl_function <R(Args...)> ��Ÿ�Կ� ���� ����Ʈ����Ʈ�� �����ؾ���.
	using type = impl_function <R(Args...)>;

	//type ���ϵǾ�� ��.
	using sm_type = std::shared_ptr<type>;

	// = ��ȣ �ϸ� �Ҵ�
	//sm_type rt =  new impl_sub_function<R(Args...)>{ fn };
	// �Ʒ� Ÿ������ �ٲ�� �����ϵ�.
	sm_type rt{ new impl_sub_function< R(Args...) > { fn } };

	return rt;
}

//Ŭ���� ��� �Լ� ����
template<class C ,class R, class... Args>
auto make_class_member_function_sugar_type_2(R(C::*fn)(Args...), C& a)
{
	//impl_function <R(Args...)> ��Ÿ�Կ� ���� ����Ʈ����Ʈ�� �����ؾ���.
	using type = impl_function <R(Args...)>;

	//type ���ϵǾ�� ��.
	using sm_type = std::shared_ptr<type>;

	// = ��ȣ �ϸ� �Ҵ�
	//sm_type rt =  new impl_sub_function<R(Args...)>{ fn };
	// �Ʒ� Ÿ������ �ٲ�� �����ϵ�.
	sm_type rt{ new impl_sub_function< R(C::*)(Args...) > { fn , a } }; //���ε� ���� ����
	return rt;
}


template<class R, class... Args>
using func_type = std::shared_ptr < impl_function< R(Args...)> >;

//�Լ� ��ü ���� type 1
template<class C >
auto make_function_obj_sugar_type_1(C& a)
{
	return make_class_member_function_sugar_type_2(&C::operator(), a);
}

//�Լ� ��ü ���� type 2
template<class C >
auto make_function_obj_sugar_type_2(C& a)
{
	using CR = std::remove_reference_t< C >;
	return make_class_member_function_sugar_type_2(&CR::operator(), a);
}


task_system<double,double,double,double> _task;

void add_is_line(double x, double y, double z, double s)
{
	cout<<" add_is_line  : "<< "index " << s  << ": "<< x <<"," << y  << "," << z << endl;
}
template<class ...ty>
bool total_function_control_1(double A, double B, double C , double S)
{
	//int index = D;
	/*std::function<void(ty...)> f_display_1 = std::bind(add_is_line_1, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	_task.async_(f_display_1, A, B, C);*/
	
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}
template<class ...ty>
bool total_function_control_2(double A, double B, double C, double S)
{
	//int index = D;
	/*std::function<void(ty...)> f_display_1 = std::bind(add_is_line_1, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	_task.async_(f_display_1, A, B, C);*/
	
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}
template<class ...ty>
bool total_function_control_3(double A, double B, double C, double S)
{
	//int index = D;
	/*std::function<void(ty...)> f_display_1 = std::bind(add_is_line_1, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	_task.async_(f_display_1, A, B, C);*/
	
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	
	return true;
}
template<class ...ty>
bool total_function_control_4(double A, double B, double C, double S)
{
	//int index = D;
	/*std::function<void(ty...)> f_display_1 = std::bind(add_is_line_1, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	_task.async_(f_display_1, A, B, C);*/
	
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}


class Args_class 
{

public:

	bool sum(double A, double B, double C, double D)
	{
		cout << "A::Sum : " << (A + B + C) << endl;
		return true;
	}

	template<class ...ty>
	bool total_function_control_1(double A, double B, double C, double S);
	template<class ...ty>
	bool total_function_control_2(double A, double B, double C, double S);
	template<class ...ty>
	bool total_function_control_3(double A, double B, double C, double S);
	template<class ...ty>
	bool total_function_control_4(double A, double B, double C, double S);

	bool operator()(double A, double B, double C, double D)
	{
		cout << "A::operator() : " << (A + B + C) << endl;
		return true;
	}
};

template<class ...ty>
bool Args_class::total_function_control_1(double A, double B, double C, double S)
{
	cout << "1 ���� ȣ��" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_2(double A, double B, double C, double S)
{
	cout << "2 ���� ȣ��" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_3(double A, double B, double C, double S)
{
	cout << "3 ���� ȣ��" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_4(double A, double B, double C, double S)
{
	cout << "4 ���� ȣ��" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}


/**************���� Ÿ������ ����******************/
int main()
{
	TimeCheck _TImeCheck;
	int i = 0;
	using vectype = func_type<bool, double, double, double, double>;
	std::vector<vectype> datas;
	/*datas.push_back(make_class_member_function_sugar_type_1(&total_function_control_1<double, double, double, double>));
	datas.push_back(make_class_member_function_sugar_type_1(&total_function_control_2<double, double, double, double>));
	datas.push_back(make_class_member_function_sugar_type_1(&total_function_control_3<double, double, double, double>));
	datas.push_back(make_class_member_function_sugar_type_1(&total_function_control_4<double, double, double, double>));*/

	Args_class a;
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_1<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_2<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_3<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_4<double, double, double, double>, a));
	datas.push_back(make_function_obj_sugar_type_2(a));

	datas.push_back(
		make_function_obj_sugar_type_2([](double x, double y, double z, double d) mutable {
		cout << " make_function_obj_sugar_type_2 : " << x << y << z << endl;
		std::cout << "lamda-Call" << endl;
		return true;
	}
			)
		);

	
	while (1)
	{
		for (auto&& item : datas)
		{
			//Sleep(1);
			if ((i % 4) == 0)
			{
				(*item)(10.0, 20.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 1)
			{
				(*item)(20.0, 40.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 2)
			{
				(*item)(70.0, 20.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 3)
			{
				(*item)(80.1, 20.2, 90.3, (double)i);
				i++;
			}
			
		}
		break;
	}
	
	
	return 0;


}
//���� 3 
//template<class... Args, class V2>
//Auto operator+=(std::vector<Args...>&c, V2 v)
//{
//	c.push_back(c);
//	return _inserter<std::vector<Args...>>(c);
//	//http://cafe.naver.com/multism/6580 ���� �ʿ�;  
//    ps-> Boost.Range �ҽ��� �����ؼ� �����ý��ϴ�
//}

///���� 4 http://cafe.naver.com/multism/6581 -> for-range �������� index ��� �����ϵ��� �����ý��ϴ�.
//
//[��ó] for-range �������� index ��� �����ϵ��� �����ý��ϴ�. (��Ƽ����-����(Director)) |�ۼ��� ���Ƴ�

