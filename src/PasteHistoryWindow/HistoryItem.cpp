#include "HistoryItem.h"

#include "HistoryViewConstants.h"

HistoryItem::HistoryItem(HistoryItemData item_data)
    : m_HistoryItemData(item_data)
{
	QString text = QString::fromStdString(item_data.m_Text);
	QString display_string = text.simplified();
	if (display_string.length() > HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		const int display_string_length = display_string.length() - HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH;
		display_string.replace(HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH, display_string_length, "...");
	}

	setData(display_string, Qt::DisplayRole);

	setEditable(false);
}