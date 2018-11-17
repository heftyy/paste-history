#include "HistoryItem.h"

#include "HistoryViewConstants.h"

HistoryItem::HistoryItem(HistoryItemData item_data)
    : m_HistoryItemData(item_data)
{
	m_DisplayText = m_HistoryItemData.m_Text;
	if (m_HistoryItemData.m_Text.length() > HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		const size_t display_string_length = m_HistoryItemData.m_Text.length() - HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH;
		m_DisplayText.replace(HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH, display_string_length, "...");
	}
}