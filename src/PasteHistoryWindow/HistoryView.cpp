#include "HistoryView.h"

#include <QKeyEvent>
#include <QStandardItemModel>

#include "HistoryItemModel.h"

#include <QAbstractItemModelTester>

HistoryView::HistoryView(QWidget* parent)
    : QListView(parent)
{
	m_HistoryItemModel = new HistoryItemModel(this);
#ifndef NDEBUG
	new QAbstractItemModelTester(m_HistoryItemModel, QAbstractItemModelTester::FailureReportingMode::Fatal, nullptr);
#endif
	setModel(m_HistoryItemModel);

	installEventFilter(this);
}

void HistoryView::AddToHistory(const std::string& text, size_t timestamp)
{
	std::hash<std::string> hasher;
	const size_t text_hash = hasher(text);
	m_HistoryItemModel->AddToHistory({text, text_hash, timestamp});
}

bool HistoryView::IsShortutKey(const QKeyEvent* key_event)
{
	if (key_event && (key_event->type() == QEvent::KeyPress || key_event->type() == QEvent::KeyRelease))
	{
		const bool control_pressed = key_event->modifiers() & Qt::CTRL;
		const bool number_pressed = key_event->key() >= Qt::Key_0 && key_event->key() <= Qt::Key_9;

		return control_pressed && number_pressed;
	}

	return false;
}

void HistoryView::UpdateFilterPattern(QString pattern)
{
	if (m_HistoryItemModel->UpdateFilterPattern(pattern))
	{
		update();
	}
}
