#pragma once


#include <deque>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <sstream>
#include <chrono>
#include <windows.h>
#include <functional>
#include <list>
#include <string>
#include <utility>
#include <complex>
#include <tuple>
#include <process.h>
#include <conio.h>
#include <stdio.h> 
#include <tchar.h> 
#include <strsafe.h>
#include <cstdlib>
#include <boost/multiprecision/cpp_int.hpp>


using namespace std;

/****************************************************************************************************/
/****************************************************************************************************/

// 튜플 구조체

/****************************************************************************************************/
/****************************************************************************************************/

template <typename T>
void xtuple_size(const T& a)
{
	cout << T::N << endl;
}

class string_set_class
{
private:
	std::string _str;
public:
	string_set_class(std::string str_) : _str(str_) {}
	std::string getter_string_set(std::string str_) { return (_str + str_); }
};


// tuple (primary template)
template <typename ...Types> class xtuple;

// empty tuple (specialization)
template <> class xtuple<> {};

// recursive tuple definition (partial specialization)

template <typename T, typename ... Types>
class xtuple<T, Types...> : private xtuple<Types ...>
{
public:
	xtuple(std::string _str) : first_{ _str } {
	}
	xtuple(const T& a, Types ... args) : first_{ a }, xtuple<Types...>(args...)
	{ }

	enum { N = 1 + sizeof...(Types) };

public:
	T first_;
};

template <size_t N, typename xtuple>
class xtuple_element;

// type of the first element
template <typename T, typename ... Types>
class xtuple_element<0, xtuple<T, Types...>>
{
public:
	xtuple_element() = default;
	~xtuple_element() = default;
public:
	using Type = T;
	using TupleType = xtuple<T, Types...>;
};


template <size_t N, typename ... Types>
inline typename xtuple_element<N, xtuple<Types...>>::Type&
Get(xtuple<Types...>& t)
{
	using TupleType = xtuple_element<N, xtuple<Types...>>::TupleType;
	return ((TupleType&)t).first_;
}


// recursive tuple_element definition
template <size_t N, typename T, typename ... Types>
class xtuple_element<N, xtuple<T, Types...>>
	: public xtuple_element<N - 1, xtuple<Types...>>
{};


template <class T>
class unwrap_refwrapper
{
public:
	using type = T;
};

template <class T>
class unwrap_refwrapper<std::reference_wrapper<T>>
{
public:
	using type = T&;
};

template <class T>
using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;

template <class... Types>
auto song_make_tuple(Types&&... args)
{
	return std::tuple<special_decay_t<Types>...>(std::forward<Types>(args)...);
}

// pretty-print a tuple (from http://stackoverflow.com/a/6245777/273767)
namespace aux {
	template<std::size_t...> struct seq {};

	template<std::size_t N, std::size_t... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

	template<std::size_t... Is>
	struct gen_seq<0, Is...> : seq<Is...> {};


	template<class Ch, class Tr, class Tuple, std::size_t... Is>
	void print_tuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t, seq<Is...>) {
		using swallow = int[];
		(void)swallow {
			0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
		};
	}
}
template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)->std::basic_ostream<Ch, Tr>&
{
	os << "(";
	aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
	return os << ")";
}



/****************************************************************************************************/

// 비동기화 쓰레드

/****************************************************************************************************/

using namespace std;
using namespace boost::multiprecision;

/**************************************************************************************************/

template <typename T, typename N, typename O>
T power(T x, N n, O op)
{
	if (n == 0) return identity_element(op);

	while ((n & 1) == 0) {
		n >>= 1;
		x = op(x, x);
	}

	T result = x;
	n >>= 1;
	while (n != 0) {
		x = op(x, x);
		if ((n & 1) != 0) result = op(result, x);
		n >>= 1;
	}
	return result;
}

/**************************************************************************************************/

template <typename N>
struct multiply_2x2 {
	array<N, 4> operator()(const array<N, 4>& x, const array<N, 4>& y)
	{
		return{ x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3],
			x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
	}
	
};

template <typename N>
array<N, 4> identity_element(const multiply_2x2<N>&) { return{ N(1), N(0), N(0), N(1) }; }

template <typename R, typename N>
R fibonacci(N n) {
	if (n == 0) return R(0);
	return power(array<R, 4>{ 1, 1, 1, 0 }, N(n - 1), multiply_2x2<R>())[0];
}

/**************************************************************************************************/

using lock_t = unique_lock<mutex>;

class notification_queue {
	deque<function<void()>> _q;
	bool                    _done{ false };
	mutex                   _mutex;
	condition_variable      _ready;

public:
	bool try_pop(function<void()>& x) {
		lock_t lock{ _mutex, try_to_lock };
		if (!lock || _q.empty()) return false;
		x = move(_q.front());
		_q.pop_front();
		return true;
	}

	template<typename F>
	bool try_push(F&& f) {
		{
			lock_t lock{ _mutex, try_to_lock };
			if (!lock) return false;
			_q.emplace_back(forward<F>(f));
		}
		_ready.notify_one();
		return true;
	}

	void done() {
		{
			unique_lock<mutex> lock{ _mutex };
			_done = true;
		}
		_ready.notify_all();
	}

	bool pop(function<void()>& x) {
		lock_t lock{ _mutex };
		while (_q.empty() && !_done) _ready.wait(lock);
		if (_q.empty()) return false;
		x = move(_q.front());
		_q.pop_front();
		return true;
	}

	template<typename F>
	void push(F&& f) {
		{
			lock_t lock{ _mutex };
			_q.emplace_back(forward<F>(f));
		}
		_ready.notify_one();
	}
};

/**************************************************************************************************/

class task_system {
	const unsigned              _count{ thread::hardware_concurrency() };
	vector<thread>              _threads;
	vector<notification_queue>  _q{ _count };
	atomic<unsigned>            _index{ 0 };

	void run(unsigned i) {
		while (true) {
			function<void()> f;

			for (unsigned n = 0; n != _count * 32; ++n) {
				if (_q[(i + n) % _count].try_pop(f)) break;
			}
			if (!f && !_q[i].pop(f)) break;

			f();
		}
	}

public:
	task_system() {
		for (unsigned n = 0; n != _count; ++n) {
			_threads.emplace_back([&, n] { run(n); });
		}
	}

	~task_system() {
		for (auto& e : _q) e.done();
		for (auto& e : _threads) e.join();
	}

	template <typename F>
	void async_(F&& f) {
		auto i = _index++;

		for (unsigned n = 0; n != _count; ++n) {
			if (_q[(i + n) % _count].try_push(forward<F>(f))) return;
		}

		_q[i % _count].push(forward<F>(f));
	}
};

/**************************************************************************************************/

task_system _system;

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

/**************************************************************************************************/

template <typename R>
struct shared_base {
	vector<R> _r; // optional
	mutex _mutex;
	condition_variable _ready;
	vector<function<void()>> _then;

	virtual ~shared_base() { }

	void set(R&& r) {
		vector<function<void()>> then;
		{
			lock_t lock{ _mutex };
			_r.push_back(move(r));
			swap(_then, then);
		}
		_ready.notify_all();
		for (const auto& f : then) _system.async_(move(f));
	}

	template <typename F>
	void then(F&& f) {
		bool resolved{ false };
		{
			lock_t lock{ _mutex };
			if (_r.empty()) _then.push_back(forward<F>(f));
			else resolved = true;
		}
		if (resolved) _system.async_(move(f));
	}

	const R& get() {
		lock_t lock{ _mutex };
		while (_r.empty()) _ready.wait(lock);
		return _r.back();
	}
};

template <typename> struct shared; // not defined

template <typename R, typename... Args>
struct shared<R(Args...)> : shared_base<R> {
	function<R(Args...)> _f;

	template<typename F>
	shared(F&& f) : _f(forward<F>(f)) { }

	template <typename... A>
	void operator()(A&&... args) {
		this->set(_f(forward<A>(args)...));
		_f = nullptr;
	}
};

template <typename> class packaged_task; //not defined
template <typename> class future;

template <typename S, typename F>
auto package(F&& f)->pair<packaged_task<S>, future<result_of_t_<S>>>;

template <typename R>
class future {
	shared_ptr<shared_base<R>> _p;

	template <typename S, typename F>
	friend auto package(F&& f)->pair<packaged_task<S>, future<result_of_t_<S>>>;

	explicit future(shared_ptr<shared_base<R>> p) : _p(move(p)) { }
public:
	future() = default;

	template <typename F>
	auto then(F&& f) {
		auto pack = package<result_of_t<F(R)>()>([p = _p, f = forward<F>(f)](){
			return f(p->_r.back());
		});
		_p->then(move(pack.first));
		return pack.second;
	}

	const R& get() const { return _p->get(); }
};

template<typename R, typename ...Args >
class packaged_task<R(Args...)> {
	weak_ptr<shared<R(Args...)>> _p;

	template <typename S, typename F>
	friend auto package(F&& f)->pair<packaged_task<S>, future<result_of_t_<S>>>;

	explicit packaged_task(weak_ptr<shared<R(Args...)>> p) : _p(move(p)) { }

public:
	packaged_task() = default;

	template <typename... A>
	void operator()(A&&... args) const {
		auto p = _p.lock();
		if (p) (*p)(forward<A>(args)...);
	}
};

template <typename S, typename F>
auto package(F&& f) -> pair<packaged_task<S>, future<result_of_t_<S>>> {
	auto p = make_shared<shared<S>>(forward<F>(f));
	return make_pair(packaged_task<S>(p), future<result_of_t_<S>>(p));
}

/**************************************************************************************************/

template <typename Function, typename ...Args>
auto async(Function&& f, Args&&... args)
{
	using result_type = result_of_t<Function(Args...)>;
	using packaged_type = packaged_task<result_type()>;

	auto pack = package<result_type()>(bind(forward<Function>(f), forward<Args>(args)...));

	_system.async_(move(get<0>(pack)));
	return get<1>(pack);
}

/**************************************************************************************************/

class TimeCheck {
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
public:
	TimeCheck() {
		start_ = std::chrono::high_resolution_clock::now();
	}
	~TimeCheck() {
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "running time:" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_).count() << std::endl;
	}
};

int iter2;
template <typename R, typename N , typename T>
R test_make_function(N n1, N n2, N n3, T type) {
	
	cout << "test_make_function" << iter2++ << endl;

	if(is_same<T, std::string>::value)	std::cout << type << std::endl;
	else                                std::cout << type << std::endl;
	
	if ((n1 == 0) || (n2 == 0) || (n3 == 0)) return R(0);
	return (n1 * n2) * n3;
}

auto multply_than_make_function(double x)
{
	return double(x * 2);
}

auto divide_than_make_function(double y)
{
	return double(y / 15.0);
}

class Cdouble
{
	double _d;
public:
	Cdouble(double d)  : _d(d) {}
};
auto customer__fibonacci_make_function_start(double x, double y, double z, std::string &str)
{
	double x_ = 0;
	for (int i = 0; i < 5000; i++)
	{
		for (int j = 0; j < 5000000; j++) {
			x_ = x_ + (i + j);
		}
	}

	std::cout << "끝" << x_ <<std::endl;
	return x_;
	//return test_make_function<double>(x, y, z, str);
}
auto customer__fibonacci_make_function_2(double x, double y, double z, std::string &str)
{
	Sleep(5000);
	return test_make_function<double>(x, y, z, str);
}
auto customer__fibonacci_make_function_3(double x, double y, double z, std::string &str)
{
	Sleep(5000);
	return test_make_function<double>(x, y, z, str);
}
auto customer__fibonacci_make_function_4(double x, double y, double z, std::string &str)
{
	Sleep(5000);
	return test_make_function<double>(x, y, z, str);
}
auto customer__fibonacci_make_function_5(double x, double y, double z, std::string &str)
{
	Sleep(5000);
	return test_make_function<double>(x, y, z, str);
}
auto customer__fibonacci_make_function_end(double x, double y, double z, std::string &str)
{
	return test_make_function<double>(x, y, z, str);
}

/****************************************************************************************************/
/****************************************************************************************************/

// 컴포넌트

/****************************************************************************************************/
/****************************************************************************************************/

#include <unordered_map>

struct null_t {};

template <class _component_t>
struct is_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;

	enum
	{
		value = std::_If<identifier_t::is_unique, std::true_type, is_unique_component<parent_t>>::type::value
	};
};
template <>
struct is_unique_component<null_t>
{
	enum { value = false };
};

template <class _component_t>
struct get_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;

	typedef typename std::_If<identifier_t::is_unique, _component_t, typename get_unique_component<parent_t>::type>::type type;
};
template <>
struct get_unique_component<null_t>
{
	typedef null_t type;
};


template <class _component_t>
struct get_component
{
	typedef typename std::_If<
		is_unique_component<_component_t>::value,
		typename get_unique_component<_component_t>::type,
		_component_t>::type type;
};

template <class _component_t, class _parent_component_t, bool _is_unique>
struct _component_identifier
{
	typedef _component_t current_component_t;
	typedef _parent_component_t parent_component_t;

	enum { is_unique = _is_unique };
};

//RTTI 
#define DECLARE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef _component_identifier<component_name, parent_component_name, unique_component> component_identifier_t; \
		static size_t getComponentId(void); \
		virtual const char* getComponentName_(void);\
		static const char* getComponentName(void); \
	private: \
		static component_identifier_t identifier; \

//RTTI 선언부
#define DEFINE_COMPONENT(component_name) \
	 size_t component_name::getComponentId(void) { return reinterpret_cast<size_t>(&identifier); }\
	 const char* component_name::getComponentName_(void){ return #component_name; } \
	 const char* component_name::getComponentName(void) { return #component_name; } \
	 component_name::component_identifier_t component_name::identifier;\


class CBaseComponent
{
public:
	DECLARE_COMPONENT(CBaseComponent, null_t, false);
};

class CRenderComponent : public CBaseComponent
{
public:
	DECLARE_COMPONENT(CRenderComponent, CBaseComponent, true);
};

class CImageRenderComponent : public CRenderComponent
{
public:
	DECLARE_COMPONENT(CImageRenderComponent, CRenderComponent, false);
};

class CPositionComponent : public CBaseComponent
{
public:
	DECLARE_COMPONENT(CPositionComponent, CBaseComponent, true);
};

class CRelativePositionComponent : public CPositionComponent
{
public:
	DECLARE_COMPONENT(CRelativePositionComponent, CPositionComponent, false);
};

class CAbsolutePositionComponent : public CPositionComponent
{
public:
	DECLARE_COMPONENT(CAbsolutePositionComponent, CPositionComponent, false);
};

DEFINE_COMPONENT(CBaseComponent);
DEFINE_COMPONENT(CRenderComponent);
DEFINE_COMPONENT(CImageRenderComponent);
DEFINE_COMPONENT(CPositionComponent);
DEFINE_COMPONENT(CRelativePositionComponent);
DEFINE_COMPONENT(CAbsolutePositionComponent);

class CGameObject
{
public:
	static CGameObject* create(void) { return new CGameObject; }

public:
	CGameObject(void) { }
	~CGameObject(void) { }

public:
	template <class component_t>
	void insertComponent(component_t* component)
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// 컴포넌트를 컨테이너에 삽입
		//--------------------------------------------------------
		this->components[componentId] = component;
	}

	template <class component_t>
	component_t* getComponent(void)
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// 컴포넌트가 컨테이너에 존재하지 않다면 nullptr 리턴
		//--------------------------------------------------------
		if (this->components.find(componentId) == this->components.end())
			return nullptr;

		//--------------------------------------------------------
		// 컴포넌트가 존재한다면 컴포넌트 리턴
		//--------------------------------------------------------
		return reinterpret_cast<component_t*>(this->components[componentId]);
	}

private:
	std::unordered_map<size_t, CBaseComponent*> components;
};


/****************************************************************************************************/
/****************************************************************************************************/

/*가변인자 템플릿 */

/****************************************************************************************************/
/****************************************************************************************************/


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
template<class C, class R, class... Args>
struct impl_sub_function< R(C::*)(Args...) > : impl_function< R(Args...) >
{
	// < R(C::*)(Args...) > 는 클래스가 가지고 있는 함수 포인터
	//원래 private 안에 있어야함 아래 3줄 ㅋㅋㅋ
	using type = R(C::*)(Args...);//함수 포인터 타입
	type fn_;

	//C& obj_;
	C obj_; //무조건 복사해서 갖고 있는걸로...

	impl_sub_function(type fn, C& obj) : fn_(fn), obj_(obj) {}

	//R value or L Value
	R operator()(Args&&... args) override
	{
		//.* 맴버접근 연산자. 
		return ( /*타겟*/ obj_.*fn_ /*스칼라(옵셋)*/)(std::forward<Args>(args)...);
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
	sm_type rt{ new impl_sub_function< R(Args...) >{ fn } };

	return rt;
}

//클래스 멤버 함수 슈가
template<class C, class R, class... Args>
auto make_class_member_function_sugar_type_2(R(C::*fn)(Args...), C& a)
{
	//impl_function <R(Args...)> 이타입에 대한 스마트포인트로 리턴해야함.
	using type = impl_function <R(Args...)>;

	//type 리턴되어야 함.
	using sm_type = std::shared_ptr<type>;

	// = 등호 하면 할당
	//sm_type rt =  new impl_sub_function<R(Args...)>{ fn };
	// 아래 타입으로 바꿔야 컴파일됨.
	sm_type rt{ new impl_sub_function< R(C::*)(Args...) >{ fn , a } }; //바인드 같은 개념
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

class Args_class
{

public:

	double section_X(double _X) { return _X; }
	double section_y(double _Y) { return _Y; }
	double section_Z(double _Z) { return _Z; }
	double section_S(double _S) { return _S; }

	void add_is_line(double x, double y, double z, double s)
	{
		cout << " add_is_line  : " << "index " << this->section_S(s) << ": " << this->section_X(x) 
									<< "," << this->section_y(y) << "," << this->section_Z(z) << endl;
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
	void(Args_class::*ptr_function_arrya[])(ty...) = { &Args_class::add_is_line };
	Args_class Ags;
	(Ags.*ptr_function_arrya[0])(A, B, C, S);	
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_2(double A, double B, double C, double S)
{
	cout << "2 번방 호출" << endl;
	void(Args_class::*ptr_function_arrya[])(ty...) = { &Args_class::add_is_line };
	Args_class Ags;
	(Ags.*ptr_function_arrya[0])(A, B, C, S);
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_3(double A, double B, double C, double S)
{
	cout << "3 번방 호출" << endl;
	void(Args_class::*ptr_function_arrya[])(ty...) = { &Args_class::add_is_line };
	Args_class Ags;
	(Ags.*ptr_function_arrya[0])(A, B, C, S);
	
	return true;
}

template<class ...ty>
bool Args_class::total_function_control_4(double A, double B, double C, double S)
{
	cout << "4 번방 호출" << endl;
	void(Args_class::*ptr_function_arrya[])(ty...) = { &Args_class::add_is_line };
	Args_class Ags;
	(Ags.*ptr_function_arrya[0])(A, B, C, S);
	
	return true;
}

/******************************************************************************************************************/

struct PARAMS
{
	int i;
	char* msg;
};

typedef struct MyData {
	int val1;
	int val2;
} MYDATA, *PMYDATA;

class MyWrapper
{
private:
	struct PARAMS
	{
		int i;
		char* msg;
	};

	static DWORD WINAPI myThread(void* parameter)
	{
		PARAMS* params = (PARAMS*)parameter;
		printf("Received parameters: i = %d, msg = '%s'\n", params->i, params->msg);
		delete params;
		return 0;
	}

public:
	HANDLE createThread(int i, char* msg)
	{
		PARAMS* params = new PARAMS;
		params->i = i;
		params->msg = msg;

		return CreateThread(NULL, 0, MyWrapper::myThread, params, 0, NULL);
	}
};


using nano_t = std::chrono::nanoseconds;
using micro_t = std::chrono::microseconds;
using milli_t = std::chrono::milliseconds;
using sec_t = std::chrono::seconds;
using min_t = std::chrono::minutes;
using hour_t = std::chrono::hours;

class timer {
public:
	timer() { base_time = std::chrono::steady_clock::now(); }

	void reset() { base_time = std::chrono::steady_clock::now(); }

	void record(std::string msg = "record: ")
	{
		auto record_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = record_time - base_time;
		std::cout << msg << diff.count() << " sec" << std::endl;
	}

	template <class T>
	void record(std::string msg = "record: ")
	{
		auto record_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = record_time - base_time;
		std::cout << msg << timer::cast<T>(diff) << timer::name<T>() << std::endl;
	}

	auto get()
	{
		auto record_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = record_time - base_time;
		return diff;
	}

	template <class C, class P>
	static auto cast(P p)
	{
		return std::chrono::duration_cast<C>(std::forward<P>(p)).count();
	}

	template <class>
	static char* name();

private:
	std::chrono::time_point<std::chrono::steady_clock> base_time;
};

#define TIMER_STRING(TYPE,STRING) \
template <> char* timer::name<TYPE>(){ return (char*)STRING; };

TIMER_STRING(nano_t, "nano")
TIMER_STRING(micro_t, "micro")
TIMER_STRING(milli_t, "milli")
TIMER_STRING(sec_t, "sec")
TIMER_STRING(min_t, "min")
TIMER_STRING(hour_t, "hour")

#define SHOW_TIME_CONVERT(TIME,FROM_TYPE,TO_TYPE) \
	std::cout<< TIME <<"  "<< timer::name<FROM_TYPE>() <<"  "<< timer::cast<TO_TYPE>(FROM_TYPE(TIME))<< "  "<< timer::name<TO_TYPE>() << "  "<< std::endl;