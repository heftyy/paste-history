#pragma once

#include <string_view>
#include <vector>

#include <FuzzySearch.h>
#include <history_types.pb.h>

class SearchableHistory
{
public:
	void AddText(const CopiedText& copied_text);

	CopiedText& GetText(int index);
	const CopiedText& GetText(int index) const;

	std::vector<FuzzySearch::SearchResult> Search(std::string_view pattern);

	bool SaveToFile(std::string_view file_path);
	bool ReadFromFile(std::string_view file_path);

private:
	CopyHistory m_CopyHistory;
};