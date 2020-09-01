#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>  // std::find_if_not

template <typename OutputIterator>
void extractWordViews(std::string const& s, OutputIterator out)
{
	static auto const isSpace = [](char letter) { return letter == ' '; };

	auto lastExaminedPosition = begin(s);
	while (lastExaminedPosition != end(s))
	{
		auto const beginWord = std::find_if_not(lastExaminedPosition, end(s), isSpace);
		auto const endWord = std::find_if(beginWord, end(s), isSpace);
		if (beginWord != endWord)
		{
			*out = std::string_view(&*beginWord, std::distance(beginWord, endWord));
			++out;
		}
		lastExaminedPosition = endWord;
	}
}

auto extractWordsViewVector(std::string const& s)
{
	std::vector<std::string_view> results;
	extractWordViews(s, back_inserter(results));
	return results;
}

bool testsPassed()
{
	bool passed = true;
	passed = passed && (extractWordsViewVector("word1  word2  ") == std::vector<std::string_view>{"word1", "word2"});
	passed = passed && (extractWordsViewVector("  word1  word2  ") == std::vector<std::string_view>{"word1", "word2"});
	passed = passed && (extractWordsViewVector("  word1  word2") == std::vector<std::string_view>{"word1", "word2"});
	passed = passed && (extractWordsViewVector("  ") == std::vector<std::string_view>{});
	passed = passed && (extractWordsViewVector("") == std::vector<std::string_view>{});
	return passed;
}

int main()
{
	std::cout << (testsPassed() ? "TESTS PASSED" : "TESTS FAILED") << '\n';
}
