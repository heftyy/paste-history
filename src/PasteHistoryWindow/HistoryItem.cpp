#include "HistoryItem.h"

#include "HistoryViewConstants.h"

HistoryItem::HistoryItem(HistoryItemData item_data)
    : m_HistoryItemData(item_data)
{
	m_PatternMatch.m_Score = 0;
	m_DisplayText = item_data.m_Text;
	if (item_data.m_Text.length() > HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		const size_t display_string_length = item_data.m_Text.length() - HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH;
		m_DisplayText.replace(HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH, display_string_length, "...");
	}
}