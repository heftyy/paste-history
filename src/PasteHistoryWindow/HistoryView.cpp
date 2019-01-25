#include "HistoryView.h"

#include <QKeyEvent>
#include <QStandardItemModel>

#include "HistoryItemDelegate.h"
#include "HistoryItemModel.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
#include <QAbstractItemModelTester>
#endif

HistoryView::HistoryView(QWidget* parent)
    : QListView(parent)
{
	m_HistoryItemModel = new HistoryItemModel(this);
#ifndef NDEBUG
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
	new QAbstractItemModelTester(m_HistoryItemModel, QAbstractItemModelTester::FailureReportingMode::Fatal, nullptr);
#endif
#endif
	setModel(m_HistoryItemModel);
	setItemDelegate(new HistoryItemDelegate(this));

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	setUniformItemSizes(true);

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
	m_HistoryItemModel->UpdateFilterPattern(pattern);
}

QSize HistoryView::sizeHint() const
{
	const QSize hint = QSize(0, 0);
	if (model() && model()->rowCount() > 0)
	{
		const QSize item_size = sizeHintForIndex(model()->index(0, 0));
		const int item_count = std::min(10, model()->rowCount());
		const int view_height = item_count * item_size.height() + 5;
		return QSize(item_size.width(), view_height);
	}
	return hint;
}
