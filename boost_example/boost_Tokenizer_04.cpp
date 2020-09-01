#include <iostream>
#include <string>
#include <boost\tokenizer.hpp>

template<typename _Elem, typename _ChangeElem >
auto TokenizerS(_Elem str, _ChangeElem *separator, int TokPos) -> _Elem
{
	using Tok = boost::tokenizer<boost::char_separator<_ChangeElem>, typename _Elem::const_iterator, _Elem>;
	int i = 0;
	boost::char_separator<_ChangeElem> sep(separator); // list of separator characters
	_Elem _str;
	Tok tok(str, sep);

	for (typename Tok::iterator beg = tok.begin(); beg != tok.end(); ++beg, ++i) {
		if (TokPos == i)
			_str = *beg;
	}

	return _str;
}

int main()
{

	std::string str = "song;min;";

	std::cout << TokenizerS<std::string, char>(str, ";",0) << std::endl; // 0 ¿ÞÂÊ
	std::cout << TokenizerS<std::string, char>(str, ";", 1) << std::endl; // 1 ¿À¸¥ÂÊ


}