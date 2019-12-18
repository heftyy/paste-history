#pragma once

#include <string>
#include <vector>

#include <QMetaType>
#include <QString>

#include <FuzzySearch.h>

struct HistoryItemData
{
	std::string m_Text;
	size_t m_TextHash{0};
	size_t m_Timestamp{0};
};

Q_DECLARE_METATYPE(HistoryItemData)

struct HistoryItem
{
	HistoryItem(HistoryItemData item_data);

	HistoryItemData m_HistoryItemData;
	QString m_DisplayText;
	FuzzySearch::PatternMatch m_Match;
};