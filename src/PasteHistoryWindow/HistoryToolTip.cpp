#include "HistoryToolTip.h"

#include <QGuiApplication>
#include <QScreen>
#include <QStyleOption>
#include <QStylePainter>
#include <QToolTip>

constexpr int HISTORY_TOOLTIP_MARGIN = 3;
constexpr int HISTORY_TOOLTIP_X_OFFSET = 3;
constexpr int HISTORY_TOOLTIP_Y_OFFSET = 3;

void HistoryToolTip::SetupToolTip(QString tooltip_text, QPoint left, QPoint right)
{
	m_ToolTipText = tooltip_text.replace("\t", "").split(" ", QString::SplitBehavior::SkipEmptyParts).join(" ");

	const QFontMetrics font_metrics(QToolTip::font());
	const QSize tooltip_size = font_metrics.size(Qt::TextExpandTabs, m_ToolTipText);

	const QRect current_screen_rect = QGuiApplication::screenAt(right)->availableGeometry();
	const QRect tooltip_rect(right, tooltip_size);
	const bool toolip_fits_on_right = current_screen_rect.contains(tooltip_rect);

	if (toolip_fits_on_right)
	{
		m_ToolTipRect = QRect(right, tooltip_rect.size());
		m_ToolTipRect.adjust(-HISTORY_TOOLTIP_MARGIN, -HISTORY_TOOLTIP_MARGIN, HISTORY_TOOLTIP_MARGIN, HISTORY_TOOLTIP_MARGIN);
		m_ToolTipRect.translate(HISTORY_TOOLTIP_MARGIN + HISTORY_TOOLTIP_X_OFFSET, HISTORY_TOOLTIP_MARGIN + HISTORY_TOOLTIP_Y_OFFSET);
	}
	else
	{
		const QPoint tooltip_pos = left - QPoint(tooltip_rect.width(), 0);
		m_ToolTipRect = QRect(tooltip_pos, tooltip_rect.size());
		m_ToolTipRect.adjust(-HISTORY_TOOLTIP_MARGIN, -HISTORY_TOOLTIP_MARGIN, HISTORY_TOOLTIP_MARGIN, HISTORY_TOOLTIP_MARGIN);
		m_ToolTipRect.translate(-HISTORY_TOOLTIP_MARGIN - HISTORY_TOOLTIP_X_OFFSET, HISTORY_TOOLTIP_MARGIN + HISTORY_TOOLTIP_Y_OFFSET);
	}

	setGeometry(m_ToolTipRect);
	update();
}

void HistoryToolTip::paintEvent(QPaintEvent* /*event*/)
{
	QStyleOption option;
	option.initFrom(this);
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setFont(QToolTip::font());
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

	const QRect text_rect(QRect(QPoint(HISTORY_TOOLTIP_MARGIN, 0), m_ToolTipRect.size()));
	style()->drawItemText(&painter, text_rect, Qt::AlignLeft | Qt::AlignVCenter, option.palette, true, m_ToolTipText);
}
