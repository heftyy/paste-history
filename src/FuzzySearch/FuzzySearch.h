// I initially based this on
// https://blog.forrestthewoods.com/reverse-engineering-sublime-text-s-fuzzy-match-4cffeed33fdb
// https://medium.com/@Srekel/implementing-a-fuzzy-search-algorithm-for-the-debuginator-cacc349e6c55
// and ended up rewriting pretty much all of it from scrach to improve performance
// and changed match scores to fit my needs

#pragma once

#include <string>
#include <vector>

namespace FuzzySearch
{

struct SearchResult
{
	std::string m_String;
	int m_Score;
	std::vector<int> m_Matches;
};

enum class MatchMode
{
	E_STRINGS,
	E_FILENAMES,
	E_SOURCE_FILES
};

int FuzzyMatch(const std::string& pattern, const std::string& str, int filename_start_index, MatchMode match_mode, std::vector<int>& out_matches);

std::vector<SearchResult> Search(const std::string& expression, const std::vector<std::string>& input_strings, MatchMode match_mode);

} // namespace FuzzySearch