#pragma once

#include <QDialog>

class PasteHistoryWindow : public QDialog
{
public:
	PasteHistoryWindow(class QWidget* parent);
	void Start();

protected:
	void showEvent(QShowEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	class HistoryView* m_HistoryView;
	class QLineEdit* m_LineEdit;

	void OnClipboardDataChanged();
};
