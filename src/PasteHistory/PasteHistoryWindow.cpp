#include "PasteHistoryWindow.h"

#include <QApplication>
#include <QClipboard>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>

#include "HistoryListView.h"

PasteHistoryWindow::PasteHistoryWindow(QWidget* parent)
    : QDialog(parent)
{
	QLayout* main_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

	m_HistoryListView = new HistoryListView(this);
	main_layout->addWidget(m_HistoryListView);

	m_LineEdit = new QLineEdit(this);
	main_layout->addWidget(m_LineEdit);

	QClipboard* clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::dataChanged, this, &PasteHistoryWindow::OnClipboardDataChanged);
}

void PasteHistoryWindow::Start()
{
	show();
}

void PasteHistoryWindow::OnClipboardDataChanged()
{
	QClipboard* clipboard = QApplication::clipboard();
	QString text = clipboard->text();

	m_HistoryListView->AddToHistory(text);
}
