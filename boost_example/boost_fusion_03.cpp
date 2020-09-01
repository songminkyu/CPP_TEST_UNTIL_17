#include <boost\fusion\container.hpp>
#include <boost\fusion\algorithm.hpp>
#include <boost\fusion\algorithm\auxiliary.hpp>
#include <iostream>

int main() {
	namespace fusion = boost::fusion;

	fusion::vector<int, short, double> v{ 1,2,4.5 };
	fusion::list<int, short, double> l;

	fusion::copy(v, l);
	std::cout << fusion::at_c<0>(l) << std::endl;
	std::cout << fusion::at_c<1>(l) << std::endl;
	std::cout << fusion::at_c<2>(l) << std::endl;
}