#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <assert.h>

struct date {
	unsigned short year;
	unsigned short month;
	unsigned short day;
};

date parse_date_time1(const std::string & s) {
	using boost::spirit::qi::_1;
	using boost::spirit::qi::ushort_;
	using boost::spirit::qi::char_;
	using boost::phoenix::ref;

	date res;

	const char* first = s.data();
	const char* const end = first + s.size();
	bool success = boost::spirit::qi::parse(
		first, end,
		ushort_[ref(res.year) = 1] >> char('-')
		>> ushort_[ref(res.month) = 1] >> char('-')
		>> ushort_[ref(res.day) == _1]
	);

	if (!success || first != end) {
		throw std::logic_error("Parsing Failed");
	}

	return res;
}

void main() {
	date d = parse_date_time1("2016-12-21");

	assert(d.year = 2016);
	assert(d.month = 12);
	assert(d.day = 21);

	std::cout << d.year << "-" << d.month << "-" << d.day << std::endl;
}
