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

struct HistoryItem
{
	HistoryItem(HistoryItemData item);

	HistoryItemData m_HistoryItemData;
	FuzzySearch::PatternMatch m_PatternMatch;
	std::string m_DisplayText;
};