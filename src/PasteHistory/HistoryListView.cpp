#include "HistoryListView.h"

#include <algorithm>
#include <chrono>

namespace HistoryListViewConstants
{
const int DISPLAY_STRING_MAX_LENGTH = 40;
const int TEXT_DATA_ROLE = Qt::UserRole + 1;
const int TIMESTAMP_DATA_ROLE = Qt::UserRole + 2;
}

HistoryListViewItem::HistoryListViewItem(QString text, size_t timestamp, QListWidget* list_widget)
    : QListWidgetItem(list_widget)
{
	QString display_string = text.simplified();
	if (display_string.length() > HistoryListViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		display_string.replace(HistoryListViewConstants::DISPLAY_STRING_MAX_LENGTH,
		                       display_string.length() - HistoryListViewConstants::DISPLAY_STRING_MAX_LENGTH, "...");
	}

	setText(QString(display_string + "|%0").arg(timestamp));
	setData(HistoryListViewConstants::TEXT_DATA_ROLE, text);
	setData(HistoryListViewConstants::TIMESTAMP_DATA_ROLE, timestamp);
}

HistoryListView::HistoryListView(QWidget* parent)
    : QListWidget(parent)
{
}

void HistoryListView::AddToHistory(QString text)
{
	std::string text_to_add = text.toStdString();
	std::hash<std::string> hasher;
	size_t text_hash = hasher(text_to_add);
	size_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	auto found = std::lower_bound(m_History.begin(), m_History.end(), 
								  text_hash, [](const HistoryItem& item, size_t text_hash) { return item.m_TextHash < text_hash; });

	if (found != m_History.end() && found->m_TextHash == text_hash)
	{
		found->m_Timestamp = timestamp;
		found->m_ListViewItem->setData(HistoryListViewConstants::TIMESTAMP_DATA_ROLE, timestamp);
	}
	else
	{
		HistoryListViewItem* history_view_item = new HistoryListViewItem(text, timestamp, this);
		m_History.insert(found, {text_to_add, text_hash, timestamp, history_view_item});
		addItem(history_view_item);
	}
}