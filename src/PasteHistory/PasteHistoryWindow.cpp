#include "PasteHistoryWindow.h"
#include "HistoryView.h"

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>

PasteHistoryWindow::PasteHistoryWindow(QWidget* parent)
    : QDialog(parent)
{
	QLayout* main_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

	m_HistoryView = new HistoryView(this);
	m_HistoryView->setFocusPolicy(Qt::NoFocus);
	main_layout->addWidget(m_HistoryView);

	m_LineEdit = new QLineEdit(this);
	m_LineEdit->setFocusPolicy(Qt::StrongFocus);
	m_LineEdit->installEventFilter(this);
	main_layout->addWidget(m_LineEdit);

	QClipboard* clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::dataChanged, this, &PasteHistoryWindow::OnClipboardDataChanged);
	connect(m_LineEdit, &QLineEdit::textChanged, m_HistoryView, &HistoryView::UpdateFilterPattern);
}

void PasteHistoryWindow::Start()
{
	show();

	m_HistoryView->AddToHistory("blum", 1, 1);
	m_HistoryView->AddToHistory("blam", 2, 2);
}

void PasteHistoryWindow::showEvent(QShowEvent* event)
{
	m_LineEdit->setFocus();
}

bool PasteHistoryWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (QKeyEvent* key_event = dynamic_cast<QKeyEvent*>(event))
	{
		if (m_HistoryView->IsShortutKey(key_event))
		{
			// qDebug() << "PasteHistoryWindow::eventFilter - pass event to the line edit" << event;
			return true;
		}
	}

	return false;
}

void PasteHistoryWindow::OnClipboardDataChanged()
{
	QClipboard* clipboard = QApplication::clipboard();
	QString text = clipboard->text();

	std::string text_to_hash = text.toStdString();
	std::hash<std::string> hasher;
	size_t text_hash = hasher(text_to_hash);
	size_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	m_HistoryView->AddToHistory(text, text_hash, timestamp);
}
