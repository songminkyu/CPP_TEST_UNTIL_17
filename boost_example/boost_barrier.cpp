
#include <thread>
#include <boost/thread/barrier.hpp>
#include <sstream>
std::size_t const count{ 10 };
boost::barrier barrier{ count + 1 };

void work(std::size_t idx) {
	{
		std::stringstream ss;
		ss << "1 action:" << (idx + 1) << std::endl;
		std::cout << ss.str();
		
	}
	barrier.wait();//##1
	{
		std::stringstream ss;
		ss << "2 action:" << (idx + 1) << std::endl;
		std::cout << ss.str();
	
	}
	barrier.wait();//##2
}

int main() {

	for (std::size_t i = 0; i < count; ++i) {
		std::thread(work, i).detach();
	}
	barrier.wait(); //##1
	barrier.wait(); //##2
}