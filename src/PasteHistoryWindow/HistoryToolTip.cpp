#include "HistoryToolTip.h"

#include <QGuiApplication>
#include <QScreen>
#include <QStyleOption>
#include <QStylePainter>
#include <QToolTip>
#include <QPoint>

#include <algorithm>
#include <limits>

constexpr int HISTORY_TOOLTIP_MARGIN = 3;
constexpr int HISTORY_TOOLTIP_X_OFFSET = 3;
constexpr int HISTORY_TOOLTIP_Y_OFFSET = 3;

QString RemoveLeadingIndentationFromString(std::string_view tooltip_text)
{
	size_t tab_count = std::numeric_limits<size_t>::max();
	size_t space_count = std::numeric_limits<size_t>::max();

	for (size_t index = 0; index < tooltip_text.size();)
	{
		const size_t first_not_tab = tooltip_text.find_first_not_of('\t', index);
		tab_count = std::min(tab_count, first_not_tab - index);

		const size_t first_not_space = tooltip_text.find_first_not_of(' ', index);
		space_count = std::min(space_count, first_not_space - index);

		index = tooltip_text.find('\n', index);
		if (index != std::string::npos)
			++index;
	}

	const QString result(tooltip_text.data());
	const size_t characters_to_remove = std::max(tab_count, space_count);
	if (characters_to_remove)
	{
		QStringList split_string = result.split("\n", QString::SplitBehavior::SkipEmptyParts);
		for (QString& line : split_string)
		{
			line.remove(0, static_cast<int>(characters_to_remove));
		}

		return split_string.join("\n");
	}

	return result;
}

HistoryToolTip::HistoryToolTip(QWidget* parent)
    : QWidget(parent)
{
	setWindowFlags(Qt::ToolTip);
	m_Font = QToolTip::font();
}

void HistoryToolTip::SetupToolTip(std::string_view tooltip_text, QPoint left, QPoint right)
{
	m_ToolTipText = RemoveLeadingIndentationFromString(tooltip_text);

	const QFontMetrics font_metrics(m_Font);
	const QSize tooltip_size = font_metrics.size(Qt::TextExpandTabs, m_ToolTipText);

	const QRect current_screen_rect = QGuiApplication::screenAt(right)->availableGeometry();
	const int max_tooltip_width = (current_screen_rect.width() - (right.x() - left.x()) - 30) / 2;
	const QRect tooltip_rect(right, QSize(std::min(tooltip_size.width(), max_tooltip_width), tooltip_size.height()));
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
	painter.setFont(m_Font);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

	const QRect text_rect(QRect(QPoint(HISTORY_TOOLTIP_MARGIN, 0), m_ToolTipRect.size()));
	style()->drawItemText(&painter, text_rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextExpandTabs, option.palette, true, m_ToolTipText);
}
