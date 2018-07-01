#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <QString>

#include <string>
#include <vector>

class HistoryListViewItem : public QListWidgetItem
{
public:
	HistoryListViewItem(QString text, size_t timestamp, QListWidget* list_widget);

private:
	struct HistoryItem* m_HistoryItem;
};

class HistoryListView : public QListWidget
{
	Q_OBJECT
public:
	HistoryListView(class QWidget* parent);
	void AddToHistory(QString text);

private:
	struct HistoryItem
	{
		std::string m_Text;
		size_t m_TextHash;
		size_t m_Timestamp;
		HistoryListViewItem* m_ListViewItem;
	};

	std::vector<HistoryItem> m_History;
};