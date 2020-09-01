#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>  // std::find_if_not


template <typename InputIterator, typename OutputIterator, typename UnaryPred>
void extractWords(InputIterator first, InputIterator last, OutputIterator out, UnaryPred pred)
{
	static const auto add{ [&out](auto beginWord, auto endWord) {*out = std::string(beginWord, endWord); ++out; } };

	for (auto lastPos{ first }; (first = std::find_if_not(lastPos, last, pred)) != last; add(first, lastPos = std::find_if(first, last, pred)));
}

auto extractWordsVector(const std::string& s)
{
	std::vector<std::string> results;
	extractWords(std::begin(s), std::end(s), back_inserter(results), [](auto letter) { return letter == ' '; });
	return results;
}


int main()
{

}