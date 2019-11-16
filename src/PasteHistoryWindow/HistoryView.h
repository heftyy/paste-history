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

	bool viewportEvent(QEvent* event) override;

public Q_SLOTS:
	void UpdateFilterPattern(QString pattern);

protected:
	QSize sizeHint() const override;

private:
	class HistoryItemModel* m_HistoryItemModel;
	class HistoryItemDelegate* m_HistoryItemDelegate;
	class HistoryToolTip* m_ToolTip;
};
