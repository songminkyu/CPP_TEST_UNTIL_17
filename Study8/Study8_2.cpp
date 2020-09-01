// Study8.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <condition_variable>
#include <mutex>
#include <deque>
#include <functional>
#include <atomic>
#include <vector>
#include <functional>//함수를 똑같은 타입으로 바꿔주는 라이브러리
#include <iostream>
#include <memory>//스마트 포인터
#include <windows.h>
using namespace std;



//템플릿 특수화, 이러한 템플릿을 사용하기 위해선 꼭 프라이머리 템플릿이 있어야한다.
//포인터로 참조가 가능하지만 일반 인스턴스에선 참조가 불가능하다.


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
		_end = true; // 종료 상태로 바꾼후 모든 상태 변수에 알람.
		_ready.notify_all();
	}

	bool pop(std::function< void(Args...) > & x) {// 상태 변수에 제어되는 함수.
		lock_t lock{ _mutex };
		_ready.wait(lock, [=] { return !_q.empty() || _end; });//!! 종료 시그널이 있거나, 빈 상태가 아님이면 깨어남.
		if (_end && _q.empty()) return false; // 종료 시그널과 큐가 비어있다면 false 반환

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
//스텝 2
template<typename T>//프라이머리(기본) 템플릿
struct impl_function;//impl_function<T> 오직 인자 하나만 받을수 있음.

template<typename R, typename...Args>
struct impl_function <R(Args...)>
{
	//R value or L Value
	virtual R operator()(Args&&... args) = 0 //int, float, int
	{
		//이렇게 구성하는것은, 여러 케이스들을 받기 위해서
		return R{};
	}
};


//impl_function 상속 -> impl_sub_function
template<class T>
struct impl_sub_function;

template<class R, class... Args>
struct impl_sub_function<R(Args...)> : impl_function<R(Args...)>
{
	//원래 private 안에 있어야함 아래 3줄 ㅋㅋㅋ 
	using type = R(*)(Args...);//함수 포인터 타입
	type fn_;
	
	impl_sub_function(type fn) : fn_(fn) {}
	
	//R value or L Value
	R operator()(Args&&... args) override
	{
		return (*fn_)(std::forward<Args>(args)...);
	}
};

//클래스 내 함수 호출....
template<class C ,class R, class... Args>
struct impl_sub_function< R(C::*)(Args...) > : impl_function< R(Args...) >
{
	// < R(C::*)(Args...) > 는 클래스가 가지고 있는 함수 포인터
	//원래 private 안에 있어야함 아래 3줄 ㅋㅋㅋ
	using type = R(C::*)(Args...);//함수 포인터 타입
	type fn_;

	//C& obj_;
	C obj_; //무조건 복사해서 갖고 있는걸로...

	impl_sub_function(type fn, C& obj) : fn_(fn), obj_(obj)
	{

	}

	//R value or L Value
	R operator()(Args&&... args) override
	{
		//.* 맴버접근 연산자. 
		return ( /*타겟*/ obj_.*fn_ /*스칼라(옵셋)*/ )(std::forward<Args>(args)...);
	}
};

//설탕 코드 : 프로그래머가 아닌 컴파일이 추론한다.
template<class R, class... Args>
auto make_class_member_function_sugar_type_1(R(*fn)(Args...))
{
	//impl_function <R(Args...)> 이타입에 대한 스마트포인트로 리턴해야함.
	using type = impl_function <R(Args...)>;

	//type 리턴되어야 함.
	using sm_type = std::shared_ptr<type>;

	// = 등호 하면 할당
	//sm_type rt =  new impl_sub_function<R(Args...)>{ fn };
	// 아래 타입으로 바꿔야 컴파일됨.
	sm_type rt{ new impl_sub_function< R(Args...) > { fn } };

	return rt;
}

//클래스 멤버 함수 슈가
template<class C ,class R, class... Args>
auto make_class_member_function_sugar_type_2(R(C::*fn)(Args...), C& a)
{
	//impl_function <R(Args...)> 이타입에 대한 스마트포인트로 리턴해야함.
	using type = impl_function <R(Args...)>;

	//type 리턴되어야 함.
	using sm_type = std::shared_ptr<type>;

	// = 등호 하면 할당
	//sm_type rt =  new impl_sub_function<R(Args...)>{ fn };
	// 아래 타입으로 바꿔야 컴파일됨.
	sm_type rt{ new impl_sub_function< R(C::*)(Args...) > { fn , a } }; //바인드 같은 개념
	return rt;
}


template<class R, class... Args>
using func_type = std::shared_ptr < impl_function< R(Args...)> >;

//함수 객체 슈가 type 1
template<class C >
auto make_function_obj_sugar_type_1(C& a)
{
	return make_class_member_function_sugar_type_2(&C::operator(), a);
}

//함수 객체 슈가 type 2
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
	cout << "1 번방 호출" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_2(double A, double B, double C, double S)
{
	cout << "2 번방 호출" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_3(double A, double B, double C, double S)
{
	cout << "3 번방 호출" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_4(double A, double B, double C, double S)
{
	cout << "4 번방 호출" << endl;
	void(*ptr_function_arrya[])(ty...) = { add_is_line };
	_task.async_(ptr_function_arrya[0], A, B, C, S);
	return true;
}


/**************백터 타입으로 변경******************/
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
//스템 3 
//template<class... Args, class V2>
//Auto operator+=(std::vector<Args...>&c, V2 v)
//{
//	c.push_back(c);
//	return _inserter<std::vector<Args...>>(c);
//	//http://cafe.naver.com/multism/6580 참조 필요;  
//    ps-> Boost.Range 소스를 참고해서 만들어봤습니다
//}

///스템 4 http://cafe.naver.com/multism/6581 -> for-range 구문에서 index 사용 가능하도록 만들어봤습니다.
//
//[출처] for-range 구문에서 index 사용 가능하도록 만들어봤습니다. (멀티즘연합-디렉터(Director)) |작성자 윤훈남

