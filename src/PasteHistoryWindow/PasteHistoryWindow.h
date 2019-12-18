
#pragma once

#include <QDialog>
#include <QClipboard>

class PasteHistoryWindow : public QDialog
{
public:
	PasteHistoryWindow(class QWidget* parent);
	void Start();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	void OnClipboardDataChanged(const std::string& text);
	void OnPasteRequested(std::string_view text);

	class HistoryView* m_HistoryView;
	class QLineEdit* m_LineEdit;
	class Clipboard* m_Clipboard;
};
