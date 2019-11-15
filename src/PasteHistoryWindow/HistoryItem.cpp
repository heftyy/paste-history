#include "HistoryItem.h"

#include "HistoryViewConstants.h"

HistoryItem::HistoryItem(HistoryItemData item_data)
    : m_HistoryItemData(item_data)
{
	m_DisplayText = QString::fromStdString(m_HistoryItemData.m_Text).trimmed();
	if (m_HistoryItemData.m_Text.length() > HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		const int new_line_index = m_DisplayText.indexOf('\n');
		int replace_from_index = HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH;
		if (new_line_index > 0)
		{
			replace_from_index = new_line_index;
		}
		const int removed_characters_count = static_cast<int>(m_HistoryItemData.m_Text.length()) - replace_from_index;
		m_DisplayText.replace(replace_from_index, removed_characters_count, "...");
	}
}