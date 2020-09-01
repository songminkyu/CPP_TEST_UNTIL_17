#include "stdafx.h"
#include "Collection.h"

#define MAIN_ACTIVE 1
#define TYPE 1

void Variant_function_in_CRPT_To_String_Return_Atype();
void Variant_function_in_CRPT_To_NonType_Return_Atype();
void Variant_function_in_CRPT_To_NonType_Return_Btype();
void Variant_function_in_CRPT_To_TupleRef_Return();
void Variant_function_in_CPRT_To_double_Return();
void unieq_Test();

struct class_string
{
	using type = std::string;
	std::string string_;
	class_string(std::string a_) : string_(a_) {}
	std::string string_return() { return string_.c_str(); }
};

struct class_string_array
{
	using type = std::vector<std::string>;
	std::vector<std::string> vec_str;

	//class_string_array() = default;
	class_string_array(const std::initializer_list<std::string> & str)
	{
		for (auto && item : str)
		{
			vec_str.push_back(item);
		}
	}

	std::vector<std::string>& output()
	{
		return vec_str;
	}

};
struct class_int
{
	using type = int;
	int int_;
	class_int(int a_) : int_(a_) {}
	int int_return() { return int_; }
};


struct class_double
{
	using type = double;
	double double_;
	class_double(double a_) : double_(a_) {}
	double double_return() { return double_; }
};
struct class_double_
{
	using type1 = double;
	using type2 = std::string;
	using type3 = std::vector<std::string>;

	double a;
	std::string b;
	std::vector<std::string> c;
	class_double_(double a_, std::string b_, std::vector<std::string> c_) : a(a_), b(b_), c(c_) {}

	double& d_return() { return a; }
	std::string& string_return_() { return b; }
	std::vector<std::string>& vs_return() 
	{
		return c; 
	}

};


/******************************************************************************************************************************/
template<class T>   struct class_type
{
	T class_mem_call;
};
template<class T> struct class_type<T*>
{
	class_type() :class_mem_call(new T()) { printf("#log : heap memory struct create\n"); };
	class_type(typename T::type a) : class_mem_call(new T(a)) { printf("##log : heap memory struct create\n"); };
	T* class_mem_call;
	~class_type() {
		printf("log : class_mem_call : heap memory struct delete\n");
		delete class_mem_call;
	}
};

template<class T>   struct class_type_
{
	T class_mem_call;
};
template<class T> struct class_type_<T*>
{
	class_type_() :class_mem_call(new T()) { printf("###log : heap memory struct create\n"); }
	class_type_(typename T::type1 a, typename T::type2 b, typename T::type3 c) : class_mem_call(new T(a, b, c)) { printf("####log : heap memory struct create\n"); }
	T* class_mem_call;
	~class_type_() {
		printf("log : class_mem_call : heap memory struct delete\n");
		delete class_mem_call;
		
	}
};


struct struct_call_mem
{
	struct_call_mem() {
		printf("log : heap memory struct create\n");
	}
	struct_call_mem(char* _name) {
		this->name = _name;
		this->name = new char[strlen(_name) + 1];
		strcpy(this->name, _name);
	}
	struct_call_mem(const struct_call_mem & deep_copy) {
		this->name = deep_copy.name;
		this->name = new char[strlen(deep_copy.name) + 1];
		strcpy(this->name, deep_copy.name);
	}
	~struct_call_mem() {
		printf("#######log : struct_call_mem : heap memory struct delete\n");

		delete[]name;
		name = nullptr;
	}
	template <class O, class D, class R, class...AS1, class...AS2>
	auto auto_arrow_call_mem_func(O&& o, R(D::*f)(AS1...), AS2&&... args)
	{
		return (static_cast<D*>(autoarrow(o))->*f)(std::forward<AS2>(args)...);
	}

	template <class O, class D, class R, class...AS1, class...AS2>
	auto auto_arrow_call_mem_func_(O&& o, R(D::*f)(AS1...), AS2... args)
	{
		return (static_cast<D*>(autoarrow(o))->*f)(std::forward<AS2>(args)...);
	}
	//----------------------------------------------------------------------------//
	template <class T>
	auto autoarrow(T&& a) { return &a; }

	template <class T>
	auto autoarrow(T* a) { return a; }
	//----------------------------------------------------------------------------//
public:

	char * name;
};

/******************************************************************************************************************************/


template<typename D, typename B>
struct Direct_Function {
	static constexpr bool value = false;
};

template<typename D, typename C, typename R, typename... Args>
struct Direct_Function<D, R(C::*)(Args...)> {
	static constexpr bool value = std::is_same<D, C>::value;
};

template <class T>
struct struct_crtp {
public:
	template <class...AS>
	auto print(AS&&... args) {
		using FN = decltype(std::declval<T>().print(std::declval<AS>()...));
		static_assert(!Direct_Function<struct_crtp<T>, FN>::value,
			"error : crtp recursive function call");
		//std::cout<<"************************" << typeid(FN).name() << std::endl;
		return static_cast<T*>(this)->print(std::forward<AS>(args)...);
	}
	
};

struct structA : public struct_crtp<structA> {
	void print() {
		printf("A : don't exist any argument\n");
	}
	//auto == std::string
	auto print(
		class_type<class_string> &cs,
		class_type<class_int> &ci,
		class_type<class_double> &cd)
	{

		cs.class_mem_call.string_ = "###호출 1번";
		std::cout << cd.class_mem_call.double_return() << std::endl;
		std::cout << ci.class_mem_call.int_return() << std::endl;

		return cs.class_mem_call.string_return();
	}
	
	//auto == xtuple<class_type_<class_double_*>*>
	auto&& print(xtuple<class_type_<class_double_*>*> *TU)
	{
		std::string plusStr = "minkyu";
		Get<0>(*TU)->class_mem_call->a = 3.5;
		Get<0>(*TU)->class_mem_call->b = "song" + plusStr;
		Get<0>(*TU)->class_mem_call->c.push_back(plusStr.c_str());
		return *TU;
	}
	//auto == xtuple<class_type<class_string>, class_type<class_int>, class_type<class_double>>& 
	auto&& print(xtuple<class_type<class_string>, class_type<class_int>, class_type<class_double>> &TU)
	{
		Get<0>(TU).class_mem_call.string_ = "songminkyu";
		Get<1>(TU).class_mem_call.int_ = 1988;
		Get<2>(TU).class_mem_call.double_ = 11.25;
		return TU;
	}

};

struct structB : public structA, public struct_crtp<structB> {
	void print() {
		printf("B : don't exist any argument\n");
	}
	
	//auto == int
	auto&& print(class_type<class_int> argc, class_type<class_string_array> argv)
	{
		std::cout << argc.class_mem_call.int_return() << std::endl;
		std::cout << argv.class_mem_call.output().at(0) << std::endl;
		std::cout << argv.class_mem_call.output().at(1) << std::endl;
		std::cout << argv.class_mem_call.output().at(2) << std::endl;
		std::cout << argv.class_mem_call.output().at(3) << std::endl;
		return  argc.class_mem_call.int_return();
	}
	//auto == std::string
	auto&& print(class_type<class_int*> *ci, class_type_<class_double_*> *cd)
	{
		std::cout << ci->class_mem_call->int_return() << std::endl;
		std::cout << cd->class_mem_call->d_return() << std::endl;
		std::cout << cd->class_mem_call->string_return_() << std::endl;
		std::cout << cd->class_mem_call->vs_return().at(0) << std::endl;
		return "ture";
	}
	//auto == std::unique_ptr<class_type_<class_double_*>>
	auto print(std::unique_ptr<class_type_<class_double_*>> & _unique_tpr){
		_unique_tpr->class_mem_call->a = 0.0;
		_unique_tpr->class_mem_call->b = (std::string)("cortex88");
		_unique_tpr->class_mem_call->c.push_back("asdasdcortex88");
		return std::move(_unique_tpr);
	}
};


//----------------------------------------------------------------------------//

class User;
using UserPtr = std::shared_ptr<User>;

class Party {

	using MemberList = std::vector<UserPtr>;
	MemberList m_MemberList;
public:
	Party() = default;
	~Party() = default;
public:
	void AddMeber(const UserPtr & member) {
		m_MemberList.push_back(member);
	}

	void RemoveMember() {

	}
};

using PartyPtr = std::shared_ptr<Party>;
using PartyWeakPtr = std::weak_ptr<Party>;

class User {
	PartyWeakPtr m_Party;
public:

	User() = default;
	~User() = default;

	void SetParty(const PartyPtr & party) {
		m_Party = party;
	}
	void LeaveParty() {
		if (m_Party.expired()) {

			PartyPtr partyPtr = m_Party.lock();
			if (partyPtr) {
				partyPtr->RemoveMember();
			}
		}
	}
};

struct_call_mem gen_call_mem("해제 발동!");
std::shared_ptr<struct_call_mem> shared_call_mem(new struct_call_mem(gen_call_mem));
class_type<class_string_array> type_array_string{ std::initializer_list<std::string> { "ABC","EFF","777","eee" } };
class_type<class_string> type_string{ "\n" };
class_type<class_int>    type_int{ 0 };
class_type<class_double> type_double{ 0.0 };
class_type<class_int*>    *type_int_ = new class_type<class_int*>{ 0 };
xtuple<class_type<class_string>, class_type<class_int>, class_type<class_double>> tuple{ class_type<class_string>{"\n"},
class_type<class_int>{NULL},
class_type<class_double>{NULL} };
class_type_<class_double_*> *type_mix_data = new class_type_<class_double_*>{ 0.0 , "\n", std::vector<std::string>{"\n"} };
xtuple<class_type_<class_double_*>*> *p_x_tuple = new xtuple<class_type_<class_double_*>*>{ type_mix_data };
std::unique_ptr<class_type_<class_double_*>> _type_double{ new class_type_<class_double_*>{ 0.0 , "\n", std::vector<std::string>{"\n"} } };
structB* b_ = new structB();


int main() {


	gen_call_mem.auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<>);


	Variant_function_in_CRPT_To_String_Return_Atype();
	Variant_function_in_CRPT_To_NonType_Return_Atype();
	Variant_function_in_CRPT_To_NonType_Return_Btype();
	Variant_function_in_CPRT_To_double_Return();
	Variant_function_in_CRPT_To_TupleRef_Return();
	unieq_Test();


	PartyPtr party(new Party);
	for (int i = 0; i < 5; i++) {
		UserPtr user(new User);
		party->AddMeber(user);
		user->SetParty(party);
	}
	party.reset();


	std::weak_ptr<struct_call_mem> weak_ptr = shared_call_mem;
	std::shared_ptr<struct_call_mem> shared_call_mem2 = weak_ptr.lock();
	
	std::cout << "shared 스마트 포인터 사용 포인터 : " << shared_call_mem.use_count() << std::endl;
	std::cout << "weak_ptr 스마트 포인터 사용 포인터 : " << weak_ptr.use_count() << std::endl;

	shared_call_mem.reset();
	
	std::cout << "shared 스마트 포인터 사용 포인터 : " << shared_call_mem.use_count() << std::endl;
	std::cout << "weak_ptr 스마트 포인터 사용 포인터 : " << weak_ptr.use_count() << std::endl;

	if (weak_ptr.expired()) {
		std::cout << "shared_ptr managed object deleted." << std::endl;
	}

	gen_call_mem.~struct_call_mem();
	p_x_tuple->first_->~class_type_();

	delete b_;
	type_int_ = nullptr;
	b_ = nullptr;
	
	return 0;
}

void unieq_Test() {

	auto unique_ptr_returnvalue = shared_call_mem->auto_arrow_call_mem_func_(b_, &struct_crtp<structB>::print<
		std::unique_ptr<class_type_<class_double_*>>>,
		std::move(_type_double));

		std::cout << unique_ptr_returnvalue->class_mem_call->vs_return().at(1) << std::endl;
}
void Variant_function_in_CRPT_To_String_Return_Atype()
{
	auto returnStr = shared_call_mem->auto_arrow_call_mem_func(b_,
		&struct_crtp<structA>::print<class_type<class_string>&, class_type<class_int>&, class_type<class_double>&>, 
		type_string, type_int, type_double);
	std::cout << returnStr.c_str() << std::endl;
}
void Variant_function_in_CRPT_To_NonType_Return_Atype()
{
	type_int.class_mem_call.int_ = 1;
	shared_call_mem->auto_arrow_call_mem_func(b_, &struct_crtp<structB>::print<
		class_type<class_int>&,class_type<class_string_array>&>,
		type_int,type_array_string);
}
void Variant_function_in_CRPT_To_NonType_Return_Btype()
{
	type_int_->class_mem_call->int_ = 100;
	auto returnStr = shared_call_mem->auto_arrow_call_mem_func_(b_, &struct_crtp<structB>::print<
		class_type<class_int*>*,class_type_<class_double_*>*>,
		type_int_,type_mix_data);
	std::cout << returnStr << std::endl;
}

void Variant_function_in_CRPT_To_TupleRef_Return()
{
	auto tupleret = shared_call_mem->auto_arrow_call_mem_func(b_, &struct_crtp<structA>::print<
		xtuple<class_type<class_string>, class_type<class_int>, class_type<class_double>>&>,
		::tuple);

	std::cout << "콜바이 레퍼런스로부터 리턴 받은 값" << std::endl;
	std::cout << Get<0>(tupleret).class_mem_call.string_return() << std::endl;
	std::cout << Get<1>(tupleret).class_mem_call.int_return() << std::endl;
	std::cout << Get<2>(tupleret).class_mem_call.double_return() << std::endl;

	std::cout << "콜바이 레퍼런스로부터 받은 값" << std::endl;
	std::cout << Get<0>(::tuple).class_mem_call.string_return() << std::endl;
	std::cout << Get<1>(::tuple).class_mem_call.int_return() << std::endl;
	std::cout << Get<2>(::tuple).class_mem_call.double_return() << std::endl;

}

void Variant_function_in_CPRT_To_double_Return()
{
	auto double_return = shared_call_mem->auto_arrow_call_mem_func_(b_, &struct_crtp<structA>::print<
		xtuple<class_type_<class_double_*>*>*>,
		p_x_tuple);

	std::cout << Get<0>(double_return)->class_mem_call->d_return() << std::endl;
	std::cout << Get<0>(double_return)->class_mem_call->string_return_() << std::endl;
	std::cout << Get<0>(double_return)->class_mem_call->vs_return().at(1) << std::endl;
}
