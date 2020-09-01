#include <cppitertools/zip_longest.hpp>
#include <cppitertools/zip.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <boost/optional.hpp>

// prints Just VALUE or Nothing
template <typename T>
std::ostream & operator<<(std::ostream & out, const boost::optional<T>& opt) {
	if (opt) {
		out << "Just " << *opt;
	}
	else {
		out << "Nothing";
	}
	return out;
}

int main() {

	std::vector<int> ivec = { 1, 4, 9, 16, 25, 36 };
	std::vector<std::string> svec = { "hello", "good day", "goodbye" };
	std::array<double, 10> darray = { 1.1,1.3,1.4,1.5,1.6 };
	std::cout << "zipping a vector of strings with a vector of ints:\n";
	for (auto&&[i, s,d] : iter::zip_longest(ivec, svec, darray)) {
		std::cout << '(' << i << ", " << s << ", " << d << ")\n";
	}

	// zip terminates on the shortest sequence, and is variadic
	std::cout << "zipping a vector of ints and a vector of strings\n";
	for (auto&&[i, s] : iter::zip(ivec, svec)) {
		std::cout << '(' << i << ", " << s << ")\n";
	}

}