// I initially based this on
// https://blog.forrestthewoods.com/reverse-engineering-sublime-text-s-fuzzy-match-4cffeed33fdb
// https://medium.com/@Srekel/implementing-a-fuzzy-search-algorithm-for-the-debuginator-cacc349e6c55
// and ended up rewriting pretty much all of it from scrach to improve performance
// and changed match scores to fit my needs

#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace FuzzySearch
{

struct PatternMatch
{
	int m_Score = 0;
	std::vector<int> m_Matches;
};

struct SearchResult
{
	std::string_view m_String;
	PatternMatch m_PatternMatch;
};

enum class MatchMode
{
	E_STRINGS,
	E_FILENAMES,
	E_SOURCE_FILES
};

constexpr size_t max_pattern_length = 256;

PatternMatch FuzzyMatch(std::string_view pattern, std::string_view str, MatchMode match_mode);

template <typename Iterator, typename Func>
std::vector<SearchResult> Search(std::string_view pattern, Iterator begin, Iterator end, Func&& get_string_func, MatchMode match_mode)
{
	static_assert(std::is_function<std::remove_pointer<Func>::type>::value, "get_string_func needs to be a pointer to function");
	static_assert(std::is_same<std::result_of<Func(typename std::iterator_traits<Iterator>::value_type)>::type, std::string_view>::value,
	              "get_string_func must return std::string_view");

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
		SearchResult search_result;
		search_result.m_String = get_string_func(element);
		search_result.m_PatternMatch = FuzzyMatch(pattern, search_result.m_String, match_mode);

		if (search_result.m_PatternMatch.m_Score > 0)
		{
			search_results.push_back(std::move(search_result));
		}
	});

	std::sort(search_results.begin(), search_results.end(), [](const SearchResult& lhs, const SearchResult& rhs) noexcept 
	{
		if (lhs.m_PatternMatch.m_Score > rhs.m_PatternMatch.m_Score)
		{
			return true;
		}
		else if (lhs.m_PatternMatch.m_Score == rhs.m_PatternMatch.m_Score)
		{
			return lhs.m_String.size() < rhs.m_String.size();
		}
		return false;
	});

	return search_results;
}

} // namespace FuzzySearch