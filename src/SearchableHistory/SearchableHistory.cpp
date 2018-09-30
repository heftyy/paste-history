#include "SearchableHistory.h"

#include <fstream>

void SearchableHistory::AddText(const CopiedText& copied_text)
{
	m_CopyHistory.mutable_history()->Add()->CopyFrom(copied_text);
}

CopiedText& SearchableHistory::GetText(int index)
{
	return *m_CopyHistory.mutable_history(index);
}

const CopiedText& SearchableHistory::GetText(int index) const
{
	return m_CopyHistory.history()[index];
}

std::vector<FuzzySearch::SearchResult> SearchableHistory::Search(std::string_view pattern)
{
	if (pattern.empty())
	{
		return {};
	}

	std::vector<FuzzySearch::SearchResult> result;
	result.reserve(m_CopyHistory.history_size());

	for (const CopiedText& copied_text : m_CopyHistory.history())
	{
		std::vector<int> matches;
		std::string input_string = copied_text.text();
		matches.reserve(pattern.length());
		const int score = FuzzySearch::FuzzyMatch(pattern, input_string, FuzzySearch::MatchMode::E_FILENAMES, matches);

		if (score > 0)
		{
			result.push_back({input_string, score, matches});
		}
	}

	std::sort(result.begin(), result.end(), [](const FuzzySearch::SearchResult& lhs, const FuzzySearch::SearchResult& rhs) noexcept {
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

bool SearchableHistory::SaveToFile(std::string_view file_path)
{
	std::fstream output(file_path.data(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (!m_CopyHistory.SerializeToOstream(&output))
	{
		return false;
	}
	return true;
}

bool SearchableHistory::ReadFromFile(std::string_view file_path)
{
	std::fstream input(file_path.data(), std::ios::in | std::ios::binary);
	if (!input)
	{
		return false;
	}
	else if (!m_CopyHistory.ParseFromIstream(&input))
	{
		return false;
	}
	return true;
}
