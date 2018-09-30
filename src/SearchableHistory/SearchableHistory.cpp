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

static const std::string& GetStringFunc(const CopiedText& copied_text)
{
	return copied_text.text();
}

std::vector<FuzzySearch::SearchResult> SearchableHistory::Search(std::string_view pattern)
{
	const auto& history = m_CopyHistory.history();
	return FuzzySearch::Search(pattern, history.begin(), history.end(), GetStringFunc, FuzzySearch::MatchMode::E_FILENAMES);
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
