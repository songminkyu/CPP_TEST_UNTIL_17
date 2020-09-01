#include "stdafx.h"
#include <iostream>
#include <vector>

template<class _Ty, class _Alloc>
struct VectorEqualSum {
	using VEC = std::vector<_Ty, _Alloc>;
	using ITERATOR_TYPE = typename VEC::iterator;
private:
	VEC& ref_;
	_Ty value_;

	struct __Detail { VectorEqualSum& __obj; _Ty sum; };
public:
	VectorEqualSum(VEC& ref, _Ty value) :ref_{ ref }, value_{ value }
	{

	}

	ITERATOR_TYPE begin() {
		return ref_.begin();
	}
	__Detail end() { return { *this,_Ty{} }; }

	friend bool operator!=(ITERATOR_TYPE & iter, __Detail & p){
		bool bret = false;
		do {
			if (iter == p.__obj.ref_.end()) break;
			bret = !(p.sum >= p.__obj.value_);
			p.sum += *iter;
		} while (false);

		return bret;
	}



};

void main()
{
	std::vector<double> datas{ 1.1, 2.2 ,3.3, 4.4, 5.8, 6.8, 7.8, 7.0 };

	for (auto &p : VectorEqualSum(datas, 10.5))
	{
		std::cout << p << std::endl;
	}
}