#pragma once

#include <QDialog>

class PasteHistoryWindow : public QDialog
{
public:
	PasteHistoryWindow(class QWidget* parent);
	void Start();

private:
	class HistoryListView* m_HistoryListView;
	class QLineEdit* m_LineEdit;

	void OnClipboardDataChanged();
};
