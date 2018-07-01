#include "FuzzySearch.h"

#include <algorithm>
#include <cctype>

namespace FuzzySearch
{

struct PatternMatch
{
	int m_Score;
	std::vector<int> m_Matches;
};

int CalculateScore(const std::string& pattern, const std::string& str, int pattern_index, int str_index, int filename_start_index, MatchMode match_mode,
                   const std::vector<int>& matches, int matches_length)
{
	const int sequential_bonus = 20;   // bonus for adjacent matches
	const int separator_bonus = 20;    // bonus if match occurs after a separator
	const int camel_bonus = 25;        // bonus if match is uppercase and prev is lower
	const int first_letter_bonus = 25; // bonus if the first letter is matched
	const int filename_bonus = 15;     // bonus if the match is in the filename instead of the path

	const int leading_letter_penalty = -2;   // penalty applied for every letter in str before the first match
	const int unmatched_letter_penalty = -1; // penalty for every letter that doesn't matter

	const int max_leading_letter_penalty = -10;

	int out_score = 0;

	if (match_mode == MatchMode::E_SOURCE_FILES)
	{
		// Prioritize cpp files
		if (str[str.length() - 3] == 'c' && str[str.length() - 2] == 'p' && str[str.length() - 1] == 'p')
		{
			out_score += 3;
		}
	}

	int matches_in_filename = 0;
	int first_match_in_filename = 0;

	// Apply ordering bonuses
	for (int i = 0; i < matches_length; ++i)
	{
		int curr_index = matches[i];

		if (i > 0)
		{
			int prev_index = matches[i - 1];

			// Sequential
			if (curr_index == (prev_index + 1))
			{
				out_score += sequential_bonus;
			}
		}

		// Check for bonuses based on neighbor character value
		if (curr_index > 0)
		{
			char neighbor = str[curr_index - 1];
			char curr = str[curr_index];
			if (match_mode == MatchMode::E_SOURCE_FILES)
			{
				// Camel case
				if ((std::islower(neighbor) || neighbor == '\\' || neighbor == '/') && std::isupper(curr))
				{
					out_score += camel_bonus;
				}
			}

			// Separator
			bool neighbor_separator = neighbor == '_' || neighbor == ' ' || neighbor == '\\' || neighbor == '/';
			if (neighbor_separator)
			{
				out_score += separator_bonus;
			}
		}

		if (curr_index >= filename_start_index)
		{
			if (first_match_in_filename == 0)
			{
				first_match_in_filename = curr_index;
			}

			// Bonus for matching the filename
			out_score += filename_bonus;
			if (curr_index == filename_start_index)
			{
				// First letter
				out_score += first_letter_bonus;
			}
			++matches_in_filename;
		}
	}

	// Apply leading letter penalty
	int calculated_leading_letter_penalty = std::min(leading_letter_penalty * (first_match_in_filename - filename_start_index), 0);
	out_score += std::max(calculated_leading_letter_penalty, max_leading_letter_penalty);

	// Apply unmatched penalty
	int unmatched = (str.length() - filename_start_index - matches_in_filename) / 3;
	out_score += std::min(unmatched_letter_penalty * unmatched, 0);

	return out_score;
}

int FuzzyMatch(const std::string& pattern, const std::string& str, int filename_start_index, MatchMode match_mode, std::vector<int>& out_matches)
{
	std::string pattern_lower = pattern;
	std::string str_lower = str;
	std::transform(pattern_lower.begin(), pattern_lower.end(), pattern_lower.begin(), ::tolower);
	std::transform(str_lower.begin(), str_lower.end(), str_lower.begin(), ::tolower);

	std::vector<PatternMatch> pattern_scores(pattern.length());
	std::vector<int> matches(pattern.length());

	int str_start = 0;

	// Loop through pattern and str looking for a match
	for (int pattern_index = 0; pattern_index < pattern.length(); ++pattern_index)
	{
		pattern_scores[pattern_index].m_Score = 0;

		int pattern_start = pattern_index;
		int match_length = 0;

		// When pattern contains a space, start a search from the beginning of str
		// again
		if (pattern[pattern_index] == ' ')
		{
			str_start = 0;
			continue;
		}

		for (int str_index = str_start; str_index < str.length(); ++str_index)
		{
			int search_pattern_index = pattern_index;
			int search_str_index = str_index;

			for (int i = 0; pattern_lower[search_pattern_index] == str_lower[search_str_index]; ++i)
			{
				matches[i] = search_str_index;

				search_pattern_index = pattern_index + i + 1;
				search_str_index = str_index + i + 1;

				if (search_pattern_index >= pattern.length() || search_str_index >= str.length())
				{
					break;
				}
			}

			if (search_pattern_index - pattern_index > 0)
			{
				int match_score =
				    CalculateScore(pattern, str, pattern_index, str_index, filename_start_index, match_mode, matches, search_pattern_index - pattern_index);
				if (match_score > pattern_scores[pattern_index].m_Score)
				{
					match_length = search_pattern_index - pattern_index;

					PatternMatch& match = pattern_scores[pattern_index];

					match.m_Score = match_score;
					match.m_Matches.resize(match_length);
					std::copy(matches.begin(), matches.begin() + match_length, match.m_Matches.begin());

					str_index += match_length;
					str_start = str_index;
				}
				// Search for better match for the pattern in the string
				pattern_index = pattern_start;
			}
		}

		if (pattern_scores[pattern_index].m_Score > 0)
		{
			pattern_index += match_length - 1;
		}
	}

	int out_score = 0;

	for (const PatternMatch& match : pattern_scores)
	{
		if (match.m_Score > 0)
		{
			out_score += match.m_Score;
			out_matches.insert(out_matches.end(), match.m_Matches.begin(), match.m_Matches.end());
		}
	}

	return out_score;
}

std::vector<SearchResult> Search(const std::string& expression, const std::vector<std::string>& input_strings, MatchMode match_mode)
{
	if (expression.empty())
	{
		return {};
	}

	std::vector<SearchResult> result;

	for (const std::string& input_string : input_strings)
	{
		int last_slash_index = match_mode == MatchMode::E_SOURCE_FILES || match_mode == MatchMode::E_FILENAMES ? input_string.find_last_of("\\/") : 0;

		std::vector<int> matches;
		int score = FuzzyMatch(expression, input_string, last_slash_index, match_mode, matches);

		if (score > 0)
		{
			result.push_back({input_string, score, matches});
		}
	}

	std::sort(result.begin(), result.end(), [](const SearchResult& lhs, const SearchResult& rhs) {
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

	return result;
}

} // namespace FuzzySearch