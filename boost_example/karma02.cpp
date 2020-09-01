
#include <boost/config/warning_disable.hpp>

#include <iostream>
#include <string>
#include <iterator>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

namespace client
{
	template<typename OutputIterator>
	bool classify_character(OutputIterator sink, char c) {
		
		namespace ascii = boost::spirit::ascii;
		namespace sprite = boost::spirit;
		namespace karma = sprite::karma;
		
		using karma::generate;

		using ascii::char_;//����
		using ascii::digit;//����
		using ascii::xdigit;//16������ ���Ǵ� 0x0A ~ 0x0F
		using ascii::alpha;//���ĺ�
		using ascii::punct;//������
		using ascii::space;//�����̽�
		using ascii::cntrl;//��Ʈ�� ����

		if (!sprite::char_encoding::ascii::isascii_(c))
		{
			return false;
		}

		return generate(
			sink,
			(
				"The character'" << char_ << "'is"
				<<
				(
					&digit << "a digit" // ���ڸ� ���
					| &xdigit << "a xdigit" // A~F �ٻ� �� ���
					| &alpha << "a alpha" // ���ĺ��ΰ�?
					| &punct << "a punct" // �������ΰ�?
					| &space << "a space" // �����̽��ΰ�
					| &cntrl << "a cntrl" //��Ʈ�� ����
					|"of unknown type" // �̵����� �ƴ� Ÿ��
				)
				<< "\n"
			),
			c, c
		);
	}
}

int main()
{
	std::string str;
	while (getline(std::cin, str)) {
		if (str.empty()) break;
		std::string gen;
		std::back_insert_iterator<std::string> sink(gen);

		for (auto & chr : str) {
			client::classify_character(sink, chr);
		}

		std::cout << gen << std::endl;
	}
	return 0;
}