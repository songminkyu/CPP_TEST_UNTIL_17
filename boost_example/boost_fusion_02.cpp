#include <iostream>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>

//fusion::vector�� ���Ե� int Ÿ�Ե��� ���� fold�� ���ϴ� ���.

struct add_ints_only
{
	template <typename State, typename T>
	State operator()(State const& state, T const&) const {
		return state;
	}

	int operator()(int state, int x) const {
		return x + state;
	}
};

int main() {
	namespace fusion = boost::fusion;
	using vector_type = fusion::vector<int, char, int, double>;
	vector_type v(12345, 'x', 678910, 3.36);

	int result = fusion::fold(v, 0, add_ints_only());
	std::cout << result << "==" << (12345 + 678910) << std::endl;
	return 0;
}
