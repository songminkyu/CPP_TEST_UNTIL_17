#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>  // std::find_if_not

template < typename OutputIterator >
void extractWords(std::string const& s, OutputIterator out)
{
	static auto const isSpace = [](char letter) { return letter == ' '; };
	for (auto lastExaminedPosition = begin(s); ;)
	{
		auto const beginWord = std::find_if_not(lastExaminedPosition, end(s), isSpace);
		if (beginWord == end(s))
			break;
		auto const endWord = std::find_if(beginWord, end(s), isSpace);
		*out = std::string(beginWord, endWord);
		++out;
		lastExaminedPosition = endWord;
	}
}
auto extractWordsVector(std::string const& s)
{
	std::vector<std::string> results;
	extractWords(s, back_inserter(results));
	return results;
}


bool testsPassed()
{
	bool passed = true;
	passed = passed && (extractWordsVector("word1  word2  ") == std::vector<std::string>{"word1", "word2"});
	passed = passed && (extractWordsVector("  word1  word2  ") == std::vector<std::string>{"word1", "word2"});
	passed = passed && (extractWordsVector("  word1  word2") == std::vector<std::string>{"word1", "word2"});
	passed = passed && (extractWordsVector("  ") == std::vector<std::string>{});
	passed = passed && (extractWordsVector("") == std::vector<std::string>{});
	return passed;
}

int main()
{
	std::cout << (testsPassed() ? "TESTS PASSED" : "TESTS FAILED") << '\n';
}
