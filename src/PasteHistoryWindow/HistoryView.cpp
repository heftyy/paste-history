#include "HistoryView.h"

#include <QEvent>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QStandardItemModel>
#include <QTooltip>

#include "HistoryItemDelegate.h"
#include "HistoryItemModel.h"
#include "HistoryToolTip.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
#include <QAbstractItemModelTester>
#endif

HistoryView::HistoryView(QWidget* parent)
    : QListView(parent)
{
	m_HistoryItemModel = new HistoryItemModel(this);
	m_HistoryItemDelegate = new HistoryItemDelegate(this);
#ifndef NDEBUG
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
	new QAbstractItemModelTester(m_HistoryItemModel, QAbstractItemModelTester::FailureReportingMode::Fatal, nullptr);
#endif
#endif
	setModel(m_HistoryItemModel);
	setItemDelegate(m_HistoryItemDelegate);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	installEventFilter(this);

	m_ToolTip = new HistoryToolTip(this);
	m_ToolTip->hide();
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

bool HistoryView::viewportEvent(QEvent* event)
{
	if (event->type() == QEvent::ToolTip)
	{
		const QHelpEvent* help_event = static_cast<QHelpEvent*>(event);
		const QModelIndex index_at_pos = indexAt(help_event->pos());
		if (index_at_pos.isValid())
		{
			const QRect item_rect = visualRect(index_at_pos);

			const QPoint left = mapToGlobal(item_rect.topLeft());
			const QPoint right = mapToGlobal(item_rect.topRight());

			m_ToolTip->SetupToolTip(m_HistoryItemModel->data(index_at_pos, Qt::ToolTipRole).toString(), left, right);
			m_ToolTip->show();
		}
		else
		{
			m_ToolTip->hide();
			event->ignore();
		}

		return true;
	}
	else if (event->type() == QEvent::Leave || event->type() == QEvent::FocusOut || event->type() == QEvent::Hide)
	{
		m_ToolTip->hide();
	}
	return QListView::viewportEvent(event);
}

void HistoryView::UpdateFilterPattern(QString pattern)
{
	m_HistoryItemModel->UpdateFilterPattern(pattern);
}

QSize HistoryView::sizeHint() const
{
	if (model() && model()->rowCount() > 0)
	{
		const QSize item_size = sizeHintForIndex(model()->index(0, 0));
		const int item_count = 10;
		const int view_height = item_count * item_size.height() + 5;
		return QSize(item_size.width(), view_height);
	}
	return QSize(200, 300);
}
