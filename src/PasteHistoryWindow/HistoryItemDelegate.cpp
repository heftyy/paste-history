#include "HistoryItemDelegate.h"

#include <QApplication>
#include <QFontDatabase>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "HistoryViewConstants.h"
#include <FuzzySearch.h>

#include <QPainter>
#include <QStyleOptionViewItem>

static void DrawFragment(QPainter* painter, const QFont& font, QString label, QRect& rect, int& current_width, int text_flags, int from,
                         int to)
{
	if (from == to)
		return;

	// Wrap text manually
	bool wrap_text = text_flags & Qt::TextWrapAnywhere;
	text_flags &= ~Qt::TextWrapAnywhere;

	painter->setFont(font);
	QRect rect_copy = rect;

	const int end_char = to == -1 ? label.size() : to;
	const int len = end_char - from;
	if (from > label.size() || len < 0 || len > (label.size() - from))
		return;

	QStringRef fragment(&label, from, len);

	for (QChar character : fragment)
	{
		int width = painter->fontMetrics().horizontalAdvance(character);

		if (wrap_text && current_width + width >= rect.width())
		{
			current_width = 0;
			rect.setTop(rect.top() + painter->fontMetrics().height());
			rect_copy.setTop(rect.top());

			rect.setBottom(rect.bottom() + painter->fontMetrics().height());
			rect_copy.setBottom(rect.bottom());
		}

		rect_copy.setLeft(rect.left() + current_width);
		painter->drawText(rect_copy, text_flags, character);

		current_width += width;
	}
}

static void DrawLabelWithHighlightedMatch(QPainter* painter, const QString& label, const QFont& font, int text_flags, QRect rect,
                                          const std::vector<int>& matches)
{
	QFont bold_font = font;
	bold_font.setWeight(QFont::ExtraBold);
	int fragment_start = 0;
	int fragment_end = 0;
	int current_width = 0;
	for (int i = 0; i < matches.size(); ++i)
	{
		int match = matches[i];

		if (match > fragment_end)
		{
			DrawFragment(painter, bold_font, label, rect, current_width, text_flags, fragment_start, fragment_end);
			DrawFragment(painter, font, label, rect, current_width, text_flags, fragment_end, match);
			fragment_start = match;
		}
		fragment_end = match + 1;
	}
	DrawFragment(painter, bold_font, label, rect, current_width, text_flags, fragment_start, fragment_end);
	DrawFragment(painter, font, label, rect, current_width, text_flags, fragment_end, -1);
}

HistoryItemDelegate::HistoryItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
	m_Font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);

	const QFontMetrics fixed_font_metrics(m_Font);
	m_CharacterSize = QSize(fixed_font_metrics.averageCharWidth(), fixed_font_metrics.height());
}

void HistoryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	const QStyle* style = option.widget ? option.widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

	const QString number = QString("%0.").arg(index.row() + 1);
	const QString text = index.model()->data(index, Qt::DisplayRole).toString();

	const int number_text_width = 20;
	const QRect rect = option.rect;

	painter->setFont(m_Font);
	painter->setRenderHint(QPainter::TextAntialiasing);

	const QRect number_rect(0, rect.top(), number_text_width, rect.height());
	const QRect text_rect(number_text_width, rect.top(), rect.width() - number_text_width, rect.height());
	painter->setPen(Qt::darkGray);
	painter->drawText(number_rect, Qt::AlignHCenter | Qt::AlignVCenter, number);

	QVariant matches_variant = index.data(HistoryViewConstants::HISTORY_ITEM_FILTER_MATCH);
	FuzzySearch::PatternMatch pattern_match = matches_variant.value<FuzzySearch::PatternMatch>();

	painter->setPen(Qt::lightGray);
	DrawLabelWithHighlightedMatch(painter, text, m_Font, Qt::AlignLeft | Qt::AlignVCenter, text_rect, pattern_match.m_Matches);

	painter->restore();
}

QSize HistoryItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
	return QSize(m_CharacterSize.width() * (HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH + 10), m_CharacterSize.height() + 8);
}
