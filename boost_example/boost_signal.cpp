#include <iostream>
#include <vector>
#include <iterator>
#include <boost\signals2\signal.hpp>

float product(float x, float y) { return x * y; };
float quotient(float x, float y) { return x / y; };
float sum(float x, float y) { return x + y; };
float diffenrce(float x, float y) { return x - y; };

struct vec_values {
	using result_type = std::vector<float>;

	template<typename InputIterator>
	result_type operator() (InputIterator first, InputIterator last) const
	{
		return result_type(first, last);
	}
};

void main() {
	boost::signals2::signal<float(float, float), vec_values> sig;
	sig.connect(std::move(product));
	sig.connect(std::move(quotient));
	sig.connect(std::move(sum));
	sig.connect(std::move(diffenrce));

	std::vector<float> result = sig(5, 3);
	
	for (auto && item : result) {
		std::cout << item << std::endl;
	}
}