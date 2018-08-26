#pragma once

#include <QListView>
#include <QStandardItem>
#include <QString>

#include <memory>
#include <string>
#include <vector>

namespace HistoryViewConstants
{
const int DISPLAY_STRING_MAX_LENGTH = 40;
const int HISTORY_ITEM_DATA_ROLE = Qt::UserRole + 1;
} // namespace HistoryViewConstants

struct HistoryItemData
{
	std::string m_Text;
	size_t m_TextHash;
	size_t m_Timestamp;

	int m_MatchScore;
	std::vector<int> m_Matches;

	HistoryItemData(const std::string text, size_t text_hash, size_t timestamp)
	    : m_Text(text)
	    , m_TextHash(text_hash)
	    , m_Timestamp(timestamp)
	    , m_MatchScore(0)
	{
	}
};

using HistoryItemDataPtr = std::unique_ptr<HistoryItemData>;

Q_DECLARE_METATYPE(HistoryItemData*)

class HistoryViewItem : public QStandardItem
{
public:
	HistoryViewItem(HistoryItemData& item);
	virtual ~HistoryViewItem();

	inline HistoryItemData& GetHistoryItemData() { return m_HistoryItemData; }

private:
	HistoryItemData& m_HistoryItemData;
};

class HistoryView : public QListView
{
	Q_OBJECT
public:
	HistoryView(class QWidget* parent);
	virtual ~HistoryView();

	void AddToHistory(QString text, size_t text_hash, size_t timestamp);
	bool IsShortutKey(QKeyEvent* key_event);

public Q_SLOTS:
	void UpdateFilterPattern(QString pattern);

private:
	std::vector<HistoryItemDataPtr> m_History;
	class QStandardItemModel* m_SourceModel;
	class HistoryModel* m_HistoryModel;
};
