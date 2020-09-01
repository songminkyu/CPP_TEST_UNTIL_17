#include <iostream>
#include <boost\accumulators\accumulators.hpp>
#include <boost\accumulators\statistics\stats.hpp>
#include <boost\accumulators\statistics\mean.hpp>
#include <boost\accumulators\statistics\moment.hpp>

namespace accs = boost::accumulators;

void main() {

	accs::accumulator_set<double, accs::stats<accs::tag::mean, accs::tag::moment<2> > > ac;

	ac(1.2);
	ac(2.3);
	ac(3.4);
	ac(4.5);

	std::cout << "Mean : " << accs::mean(ac) << std::endl;
	std::cout << "Moment : " << accs::moment<2>(ac) << std::endl;
}