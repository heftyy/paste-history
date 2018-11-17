#include "FuzzySearch.h"

#include <gsl/gsl>
#include <set>

// #define USE_STD_LOWER
#if defined(USE_STD_LOWER)
#include <cctype>
#endif

namespace FuzzySearch
{

constexpr int sequential_bonus = 20;         // bonus for adjacent matches
constexpr int separator_bonus = 20;          // bonus if match occurs after a separator
constexpr int camel_bonus = 25;              // bonus if match is uppercase and prev is lower
constexpr int first_letter_bonus = 25;       // bonus if the first letter is matched
constexpr int filename_bonus = 15;           // bonus if the match is in the filename instead of the path
constexpr int leading_letter_penalty = -2;   // penalty applied for every letter in str before the first match
constexpr int unmatched_letter_penalty = -1; // penalty for every letter that doesn't matter

constexpr int max_leading_letter_penalty = -10;
constexpr int max_unmatched_characters_from_pattern = 3;

constexpr bool IsLower(char c) noexcept
{
#if defined(USE_STD_LOWER)
	return std::islower(c);
#else
	return (c & 0x20) != 0;
#endif
}

constexpr bool IsUpper(char c) noexcept
{
#if defined(USE_STD_LOWER)
	return std::isupper(c);
#else
	return (c & 0x20) == 0;
#endif
}

constexpr int ToLower(int c) noexcept
{
#if defined(USE_STD_LOWER)
	return std::tolower(c);
#else
	return c | 0x20;
#endif
}

constexpr int ToUpper(int c) noexcept
{
#if defined(USE_STD_LOWER)
	return std::toupper(c);
#else
	return c & ~0x20;
#endif
}

const std::set<std::string>& GetSourceExtensions()
{
	static std::set<std::string> source_extensions = {".cpp", ".cs", ".c", ".java", ".py"};
	return source_extensions;
}

bool IsSourceFile(std::string_view str)
{
	const size_t ext_len = str.rfind(".");
	if (ext_len != std::string::npos)
	{
		std::string ext(str, ext_len, str.length() - ext_len);
		const std::set<std::string>& source_extensions = GetSourceExtensions();
		return source_extensions.find(ext) != source_extensions.end();
	}
	return false;
}

int CalculateSequentialMatchScore(std::string_view str, int filename_start_index, MatchMode match_mode, const gsl::span<int>& matches)
{
	int out_score = 5;
	const int str_length = gsl::narrow_cast<int>(str.length());

	int matches_in_filename = 0;
	int first_match_in_filename = -1;

	// Apply ordering bonuses
	for (const int curr_index : matches)
	{
		// Check for bonuses based on neighbor character value
		if (curr_index > 0 && (match_mode == MatchMode::E_FILENAMES || match_mode == MatchMode::E_SOURCE_FILES))
		{
			const char neighbor = str.at(curr_index - 1);
			const char curr = str.at(curr_index);

			// Camel case
			if ((IsLower(neighbor) || neighbor == '\\' || neighbor == '/') && IsUpper(curr))
			{
				out_score += camel_bonus;
			}

			// Separator
			if (neighbor == '_' || neighbor == ' ' || neighbor == '\\' || neighbor == '/')
			{
				out_score += separator_bonus;
			}
		}

		if (curr_index >= filename_start_index)
		{
			// Save the first match in the filename
			if (first_match_in_filename == max_pattern_length + 1)
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

	if (match_mode == MatchMode::E_SOURCE_FILES && IsSourceFile(str))
	{
		out_score += 2;
	}

	// Apply leading letter penalty
	const int calculated_leading_letter_penalty = std::min(leading_letter_penalty * (first_match_in_filename - filename_start_index), 0);
	out_score += std::max(calculated_leading_letter_penalty, max_leading_letter_penalty);

	// Apply unmatched penalty
	const int unmatched = (str_length - filename_start_index - matches_in_filename) / 3;
	out_score += std::min(unmatched_letter_penalty * unmatched, 0);

	// Apply sequential match bonus
	out_score += sequential_bonus * (gsl::narrow_cast<int>(matches.size()) - 1);

	return out_score;
}

inline int FindSequentialMatch(std::string_view pattern, int pattern_index, std::string_view str, int str_index) noexcept
{
	// The pattern characters usually mismatch str characters so this early out just helps optizmier/cpu
	if (ToLower(pattern.at(pattern_index)) != ToLower(str.at(str_index)))
	{
		return 0;
	}

	const int pattern_length = gsl::narrow_cast<int>(pattern.length());
	const int str_length = gsl::narrow_cast<int>(str.length());

	int matched_chars = 0;
	while (ToLower(pattern.at(pattern_index + matched_chars)) == ToLower(str.at(str_index + matched_chars)))
	{
		++matched_chars;
		if (pattern_index + matched_chars >= pattern_length || str_index + matched_chars >= str_length)
		{
			return matched_chars;
		}
	}

	return matched_chars;
}

PatternMatch CalculatePatternScore(std::string_view pattern, const gsl::span<PatternMatch>& in_matches)
{
	PatternMatch out_match;
	out_match.m_Matches.reserve(pattern.length());
	int unmatched_characters_from_pattern = 0;

	const int pattern_length = gsl::narrow_cast<int>(pattern.length());

	for (int pattern_index = 0; pattern_index < pattern_length; ++pattern_index)
	{
		const PatternMatch& match = in_matches[pattern_index];
		if (match.m_Score > 0)
		{
			out_match.m_Score += match.m_Score;
			out_match.m_Matches.insert(out_match.m_Matches.end(), match.m_Matches.begin(), match.m_Matches.end());

			// Advance the pattern_index by the match length, m_Score is only set for the first character of a match
			pattern_index += gsl::narrow_cast<int>(match.m_Matches.size() - 1);
		}
		else if (pattern.at(pattern_index) != ' ')
		{
			++unmatched_characters_from_pattern;
		}
	}

	// Allow some unmatched characters (typos etc...)
	if (unmatched_characters_from_pattern >= max_unmatched_characters_from_pattern)
	{
		return {0};
	}

	return out_match;
}

PatternMatch FuzzyMatch(std::string_view pattern, std::string_view str, MatchMode match_mode)
{
	const int pattern_length = gsl::narrow_cast<int>(pattern.length());
	const int str_length = gsl::narrow_cast<int>(str.length());

	int str_start = 0;

	std::vector<PatternMatch> pattern_matches(pattern_length);
	std::vector<int> match_indexes(pattern_length);

	int last_path_separator_index = 0;
	if (match_mode == MatchMode::E_SOURCE_FILES || match_mode == MatchMode::E_FILENAMES)
	{
		last_path_separator_index = gsl::narrow_cast<int>(str.find_last_of("\\/"));
	}

	// Loop through pattern and str looking for a match
	for (int pattern_index = 0; pattern_index < pattern_length; ++pattern_index)
	{
		pattern_matches.at(pattern_index).m_Score = 0;

		const int pattern_start = pattern_index;
		int best_match_length = 0;

		// When pattern contains a space, start a search from the beginning of str
		// again to allow out of order matches from the pattern
		if (pattern.at(pattern_index) == ' ')
		{
			str_start = 0;
			continue;
		}

		for (int str_index = str_start; str_index < str_length; ++str_index)
		{
			const int match_length = FindSequentialMatch(pattern, pattern_index, str, str_index);
			if (match_length > 0)
			{
				// We know that the sequential match started at str_index so fill match_indexes
				std::iota(match_indexes.begin(), match_indexes.begin() + match_length, str_index);

				const auto match_span = gsl::span<int>(match_indexes.data(), match_length);
				const int match_score = CalculateSequentialMatchScore(str, last_path_separator_index, match_mode, match_span);

				if (match_score > pattern_matches.at(pattern_index).m_Score)
				{
					best_match_length = match_length;

					PatternMatch& match = pattern_matches.at(pattern_index);
					match.m_Score = match_score;
					match.m_Matches.assign(match_indexes.begin(), match_indexes.begin() + best_match_length);

					// Skip searching for matches in str that we already used in our currect best match, doing this to improve performance
					str_index += best_match_length;
					str_start = str_index;
				}
				// Search for better match for the pattern in the string
				pattern_index = pattern_start;
			}
		}

		if (pattern_matches.at(pattern_index).m_Score > 0)
		{
			pattern_index += best_match_length - 1;
		}
	}

	return CalculatePatternScore(pattern, gsl::span<PatternMatch>(pattern_matches.data(), pattern_length));
}

} // namespace FuzzySearch
