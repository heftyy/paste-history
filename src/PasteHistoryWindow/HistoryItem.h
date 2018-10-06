#pragma once

#include <string>
#include <vector>

#include <QStandardItem>
#include <QString>

#include <FuzzySearch.h>

struct HistoryItemData
{
	std::string m_Text;
	size_t m_TextHash;
	size_t m_Timestamp;
};

Q_DECLARE_METATYPE(HistoryItemData)

class HistoryItem : public QStandardItem
{
public:
	HistoryItem(HistoryItemData item);

	const HistoryItemData& GetHistoryItemData() const noexcept { return m_HistoryItemData; }

	const FuzzySearch::PatternMatch& GetPatternMatch() const noexcept { return m_PatternMatch; }
	void SetPatternMatch(FuzzySearch::PatternMatch match) noexcept { m_PatternMatch = match; }

private:
	HistoryItemData m_HistoryItemData;
	FuzzySearch::PatternMatch m_PatternMatch;
};