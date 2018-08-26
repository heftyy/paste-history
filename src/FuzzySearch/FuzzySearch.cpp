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

constexpr int sequential_bonus = 20;   // bonus for adjacent matches
constexpr int separator_bonus = 20;    // bonus if match occurs after a separator
constexpr int camel_bonus = 25;        // bonus if match is uppercase and prev is lower
constexpr int first_letter_bonus = 25; // bonus if the first letter is matched
constexpr int filename_bonus = 15;     // bonus if the match is in the filename instead of the path

constexpr int leading_letter_penalty = -2;   // penalty applied for every letter in str before the first match
constexpr int unmatched_letter_penalty = -1; // penalty for every letter that doesn't matter

constexpr int max_leading_letter_penalty = -10;

constexpr int max_unmatched_characters_from_pattern = 1;

int CalculateScore(const std::string& str, size_t filename_start_index, MatchMode match_mode, const std::vector<int>& matches, size_t matches_length)
{
	int out_score = 25;

	if (match_mode == MatchMode::E_SOURCE_FILES)
	{
		// Prioritize cpp files
		if (str[str.length() - 3] == 'c' && str[str.length() - 2] == 'p' && str[str.length() - 1] == 'p')
		{
			out_score += 3;
		}
	}

	size_t matches_in_filename = 0;
	int first_match_in_filename = std::numeric_limits<int>::min();

	// Apply ordering bonuses
	for (size_t i = 0; i < matches_length; ++i)
	{
		size_t curr_index = matches[i];

		if (i > 0)
		{
			size_t prev_index = matches[i - 1];

			// Sequential
			if (curr_index == (prev_index + 1))
			{
				out_score += sequential_bonus;
			}
		}

		// Check for bonuses based on neighbor character value
		if (curr_index > 0 && (match_mode == MatchMode::E_FILENAMES || match_mode == MatchMode::E_SOURCE_FILES))
		{
			char neighbor = str[curr_index - 1];
			char curr = str[curr_index];

			// Camel case
			if ((std::islower(neighbor) || neighbor == '\\' || neighbor == '/') && std::isupper(curr))
			{
				out_score += camel_bonus;
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
			// Save the first match in the filename
			if (first_match_in_filename == std::numeric_limits<int>::min())
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
	int calculated_leading_letter_penalty = std::min(leading_letter_penalty * (first_match_in_filename - static_cast<int>(filename_start_index)), 0);
	out_score += std::max(calculated_leading_letter_penalty, max_leading_letter_penalty);

	// Apply unmatched penalty
	int unmatched = static_cast<int>(str.length() - filename_start_index - matches_in_filename) / 3;
	out_score += std::min(unmatched_letter_penalty * unmatched, 0);

	return out_score;
}

int FuzzyMatch(const std::string& pattern, const std::string& str, size_t filename_start_index, MatchMode match_mode, std::vector<int>& out_matches)
{
	std::string pattern_lower = pattern;
	std::string str_lower = str;
	std::transform(pattern_lower.begin(), pattern_lower.end(), pattern_lower.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
	std::transform(str_lower.begin(), str_lower.end(), str_lower.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });

	std::vector<PatternMatch> pattern_scores(pattern.length());
	std::vector<int> matches(pattern.length());

	int str_start = 0;

	// Loop through pattern and str looking for a match
	for (size_t pattern_index = 0; pattern_index < pattern.length(); ++pattern_index)
	{
		pattern_scores[pattern_index].m_Score = 0;

		int pattern_start = pattern_index;
		int match_length = 0;

		// When pattern contains a space, start a search from the beginning of str
		// again to allow out of order matches from the pattern
		if (pattern[pattern_index] == ' ')
		{
			str_start = 0;
			continue;
		}

		for (size_t str_index = str_start; str_index < str.length(); ++str_index)
		{
			size_t search_pattern_index = pattern_index;
			size_t search_str_index = str_index;

			for (size_t i = 0; pattern_lower[search_pattern_index] == str_lower[search_str_index]; ++i)
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
				int match_score = CalculateScore(str, filename_start_index, match_mode, matches, search_pattern_index - pattern_index);
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
	int unmatched_characters_from_pattern = 0;

	for (size_t pattern_index = 0; pattern_index < pattern_scores.size(); ++pattern_index)
	{
		const PatternMatch& match = pattern_scores[pattern_index];
		if (match.m_Score > 0)
		{
			out_score += pattern_scores[pattern_index].m_Score;
			out_matches.insert(out_matches.end(), match.m_Matches.begin(), match.m_Matches.end());

			// Advance the pattern_index by the match length, m_Score is only set for the first character of a match
			pattern_index += static_cast<int>(match.m_Matches.size() - 1);
		}
		else if (pattern[pattern_index] != ' ')
		{
			++unmatched_characters_from_pattern;
		}
	}

	// Allow some unmatched characters (typos etc...)
	if (unmatched_characters_from_pattern >= max_unmatched_characters_from_pattern)
	{
		return 0;
	}

	return out_score;
}

std::vector<SearchResult> Search(const std::string& pattern, const std::vector<std::string>& input_strings, MatchMode match_mode)
{
	if (pattern.empty())
	{
		return {};
	}

	std::vector<SearchResult> result;

	for (const std::string& input_string : input_strings)
	{
		size_t last_path_separator_index = 0;
		if (match_mode == MatchMode::E_SOURCE_FILES || match_mode == MatchMode::E_FILENAMES)
		{
			last_path_separator_index = input_string.find_last_of("\\/");
		}

		std::vector<int> matches;
		int score = FuzzyMatch(pattern, input_string, last_path_separator_index, match_mode, matches);

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