#include <iostream>
#include <string>
#include <boost\tokenizer.hpp>
#include <vector>

template<typename _Elem, typename _ChangeElem, typename _Container>
auto Tokenizer_v_l(_Elem str, _ChangeElem *separator) -> _Container
{
	using Tok = boost::tokenizer<boost::char_separator<_ChangeElem>, typename _Elem::const_iterator, _Elem>;
	_Container getstrPath;
	boost::char_separator<_ChangeElem> sep(separator);
	Tok tok(str, sep);
	for (typename Tok::const_iterator beg = tok.begin(); beg != tok.end(); ++beg) {
		getstrPath.push_back(*beg);
	}
	return getstrPath;
}

int main()
{

	std::string str = "song;min;kyu";
	
	for (auto && item : Tokenizer_v_l<std::string, char,std::vector<std::string>>(str, ";"))
	{
		std::cout << item << std::endl;
	}

	return 0;
}