// I initially based this on
// https://blog.forrestthewoods.com/reverse-engineering-sublime-text-s-fuzzy-match-4cffeed33fdb
// https://medium.com/@Srekel/implementing-a-fuzzy-search-algorithm-for-the-debuginator-cacc349e6c55
// and ended up rewriting pretty much all of it from scrach to improve performance
// and changed match scores to fit my needs

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace FuzzySearch
{

struct SearchResult
{
	std::string m_String;
	int m_Score = 0;
	std::vector<int> m_Matches;
};

enum class MatchMode
{
	E_STRINGS,
	E_FILENAMES,
	E_SOURCE_FILES
};

constexpr size_t max_pattern_length = 256;

int FuzzyMatch(std::string_view pattern, std::string_view str, MatchMode match_mode, std::vector<int>& out_matches);

template <typename Iterator, typename Func>
std::vector<SearchResult> Search(std::string_view pattern, Iterator begin, Iterator end, Func&& get_string_func, MatchMode match_mode)
{
	if (pattern.empty())
	{
		return {};
	}
	else if (pattern.length() >= max_pattern_length)
	{
		pattern = pattern.substr(0, max_pattern_length);
	}

	std::vector<SearchResult> search_results;
	search_results.reserve(std::distance(begin, end));

	std::for_each(begin, end, [pattern, &get_string_func, match_mode, &search_results](const auto& element) 
	{
		std::vector<int> matches;
		matches.reserve(pattern.length());

		const std::string& input_string = get_string_func(element);
		const int score = FuzzyMatch(pattern, input_string, match_mode, matches);

		if (score > 0)
		{
			search_results.push_back({input_string, score, matches});
		}
	});

	std::sort(search_results.begin(), search_results.end(), [](const SearchResult& lhs, const SearchResult& rhs) noexcept 
	{
		if (lhs.m_Score > rhs.m_Score)
		{
			return true;
		}
		else if (lhs.m_Score == rhs.m_Score)
		{
			return lhs.m_String.size() < rhs.m_String.size();
		}
		return false;
	});

	return search_results;
}

} // namespace FuzzySearch