#pragma once

#include <QListView>
#include <QString>

class HistoryView : public QListView
{
	Q_OBJECT
public:
	HistoryView(class QWidget* parent);

	void AddToHistory(const std::string& text, size_t timestamp);
	bool IsShortutKey(const QKeyEvent* key_event);

public Q_SLOTS:
	void UpdateFilterPattern(QString pattern);

private:
	class HistoryItemModel* m_HistoryItemModel;
};
