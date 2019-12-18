
#include "PasteHistoryWindow.h"

#include <chrono>

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QLayout>
#include <QLineEdit>

#include "Clipboard.h"
#include "HistoryView.h"

PasteHistoryWindow::PasteHistoryWindow(QWidget* parent)
    : QDialog(parent)
{
	QLayout* main_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

	m_HistoryView = new HistoryView(this);
	main_layout->addWidget(m_HistoryView);

	m_LineEdit = new QLineEdit(this);
	m_LineEdit->installEventFilter(this);
	main_layout->addWidget(m_LineEdit);
	main_layout->setSizeConstraint(QLayout::SetFixedSize);

	m_HistoryView->setFocusProxy(m_LineEdit);

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	m_Clipboard = new Clipboard(this);
	connect(m_Clipboard, &Clipboard::DataChanged, this, &PasteHistoryWindow::OnClipboardDataChanged);
	connect(m_LineEdit, &QLineEdit::textChanged, m_HistoryView, &HistoryView::UpdateFilterPattern);
	connect(m_HistoryView, &HistoryView::activated, this, &PasteHistoryWindow::OnPasteRequested);
}

void PasteHistoryWindow::Start()
{
	m_HistoryView->AddToHistory("blum1", 1);
	m_HistoryView->AddToHistory("blam2", 2);
	m_HistoryView->AddToHistory("blam5", 5);
	m_HistoryView->AddToHistory("blam78", 78);
	m_HistoryView->AddToHistory("blam3", 3);
	m_HistoryView->AddToHistory("blam105", 105);

	show();
}

bool PasteHistoryWindow::eventFilter(QObject* /*obj*/, QEvent* event)
{
	if (const QKeyEvent* key_event = dynamic_cast<QKeyEvent*>(event))
	{
		if (m_HistoryView->IsShortutKey(key_event))
		{
			qDebug() << "PasteHistoryWindow::eventFilter - pass event to the line edit" << event;
			return true;
		}
		else if (key_event->key() >= Qt::Key_Left && key_event->key() <= Qt::Key_Down || key_event->key() == Qt::Key_Return)
		{
			QApplication::sendEvent(m_HistoryView, event);
			return true;
		}
	}

	return false;
}

void PasteHistoryWindow::OnClipboardDataChanged(const std::string& text)
{
	const auto duration_since_epoch = std::chrono::system_clock::now().time_since_epoch();
	const size_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();

	m_HistoryView->AddToHistory(text, timestamp);
}

void PasteHistoryWindow::OnPasteRequested(std::string_view text)
{
	
}
