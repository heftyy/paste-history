#pragma once

#include <QStandardItem>
#include <QString>
#include <QTableView>

#include <string>
#include <vector>

namespace HistoryListViewConstants
{
const int DISPLAY_STRING_MAX_LENGTH = 40;
const int HISTORY_ITEM_DATA_ROLE = Qt::UserRole + 1;
} // namespace HistoryListViewConstants

struct HistoryItemData
{
	std::string m_Text;
	size_t m_TextHash;
	size_t m_Timestamp;
};

class HistoryViewItem : public QStandardItem
{
public:
	HistoryViewItem(HistoryItemData* item_data);

private:
	HistoryItemData* m_HistoryItemData;
};

class HistoryView : public QTableView
{
	Q_OBJECT
public:
	HistoryView(class QWidget* parent);
	void AddToHistory(QString text);

private:
	std::vector<HistoryItemData> m_History;
};