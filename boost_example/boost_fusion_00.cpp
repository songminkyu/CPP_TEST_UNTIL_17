
//MPL containers work on types.
//Fusion containers work on both types and values.
//STL�� �� ������� �۵��ϴ� �����̳�.
//MPL�� Ÿ�� ������� �۵��ϴ� �����̳�.
//Fusion�� ���� Ÿ�� ������� �۵��ϴ� �����̳�.
//[��ó] Boost::fusion ���� 1 (��Ƽ���� - ����(Director)) | �ۼ��� ���Ƴ�

#include <iostream>
#include <string>

//Vector
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>

//XML
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>

namespace fusion = boost::fusion;

struct print_xml {
	template<class T>
	void operator()(T const & x) const {
		std::cout << "<" << typeid(x).name() << ">" << x << "</"
			<< typeid(x).name() << ">";
	}
};

void main() {
	fusion::vector<int, char, std::string> stuff(1, 'x', "hellow");
	/*
	int i = fusion::at_c<0>(stuff);
	int char = fusion::at_c<1>(stuff);
	std::string = fusion::at_c<2>(stuff);
	*/

	fusion::for_each(stuff, print_xml());
	std::cout << std::endl;
}