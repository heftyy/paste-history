#include "HistoryView.h"

#include <QEvent>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QStandardItemModel>
#include <QTooltip>

#include "HistoryItemDelegate.h"
#include "HistoryItemModel.h"

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

			QString tooltip_text = m_HistoryItemModel->data(index_at_pos, Qt::ToolTipRole).toString();
			tooltip_text = tooltip_text.replace("\t", "").split(" ", QString::SplitBehavior::SkipEmptyParts).join(" ");

			//! 16 is the hardcoded offset in QToolTip::showText :(
			const QPoint global_tooltip_pos = mapToGlobal(item_rect.topRight()) - QPoint(0, 15);

			const QPoint global_cursor_pos = mapToGlobal(help_event->pos());
			const QRect current_screen_rect = QGuiApplication::screenAt(global_cursor_pos)->availableGeometry();

			const QFontMetrics font_metrics(QToolTip::font());
			const QRect tooltip_rect = QRect(QPoint(0, 0), font_metrics.size(Qt::TextExpandTabs, tooltip_text, 16));

			const bool toolip_fits = current_screen_rect.contains(tooltip_rect.translated(global_tooltip_pos));
			QPoint offset(0, 0);
			if (!toolip_fits)
			{
				const QRect main_window_geometry = topLevelWidget()->geometry();

				const int main_window_offset = global_tooltip_pos.x() - main_window_geometry.left();
				//! 7 seems to be the tooltip margins and i don't know how to find the real value
				offset = -QPoint(main_window_offset + tooltip_rect.width() + 7, 0);
			}

			QToolTip::showText(global_tooltip_pos + offset, tooltip_text);
		}
		else
		{
			QToolTip::hideText();
			event->ignore();
		}

		return true;
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
