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

protected:
	QSize sizeHint() const override;
	
	bool viewportEvent(QEvent* event) override;
	void showEvent(class QShowEvent* event) override;

private:
	void SelectTopRow();

	class HistoryItemModel* m_HistoryItemModel;
	class HistoryItemDelegate* m_HistoryItemDelegate;
	class HistoryToolTip* m_ToolTip;
};
