#include <iostream>
#include <vector>
#include <algorithm>

#include <boost\accumulators\accumulators.hpp>
#include <boost\accumulators\statistics\stats.hpp>
#include <boost\accumulators\statistics\min.hpp>
#include <boost\accumulators\statistics\mean.hpp>
#include <boost\accumulators\statistics\sum.hpp>
#include <boost\accumulators\statistics\moment.hpp>

namespace accs = boost::accumulators;
int main() {
	accs::accumulator_set<double, 
						  accs::stats<accs::tag::min, 
						  accs::tag::mean(accs::immediate), 
						  accs::tag::sum, accs::tag::moment<2>>> ac;
	//memont<X> = ((1.2 ^ X)  + ( 2.3 ^ X) + ( 3.4 ^ X) + ( 4.5 ^ X)) / 4
	// X = ÀÚ½Â(Á¦°ö)
	std::vector<double> datas = { 1.2 , 2.3 , 3.4 , 4.5 };
	ac = std::for_each(datas.begin(), datas.end(), ac);

	std::cout << "min : " << accs::min(ac) << std::endl;
	std::cout << "mean : " << accs::mean(ac) << std::endl;
	std::cout << "count : " << accs::count(ac) << std::endl;
	std::cout << "sum : " << accs::sum(ac) << std::endl;
	std::cout << "moment : " << accs::moment<2>(ac) << std::endl;
}