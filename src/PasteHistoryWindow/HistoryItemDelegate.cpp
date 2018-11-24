#include "HistoryItemDelegate.h"

#include <QApplication>
#include <QFontDatabase>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "HistoryViewConstants.h"

HistoryItemDelegate::HistoryItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
	m_Font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

	const QFontMetrics fixed_font_metrics(m_Font);
	m_CharacterSize = QSize(fixed_font_metrics.averageCharWidth(), fixed_font_metrics.height());
}

void HistoryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// initStyleOption(&option, index);
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
	style->drawItemText(painter, number_rect, Qt::AlignHCenter | Qt::AlignVCenter, option.palette, true, number, QPalette::Dark);
	style->drawItemText(painter, text_rect, Qt::AlignLeft | Qt::AlignVCenter, option.palette, true, text, QPalette::Light);

	/*
	QRect rect = option.rect;
	QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
	if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	    cg = QPalette::Inactive;

	// set pen color
	if (option.state & QStyle::State_Selected)
	    painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
	else
	    painter->setPen(option.palette.color(cg, QPalette::Text));

	const int number_text_width = 20;

	QString text = index.model()->data(index, Qt::DisplayRole).toString();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(HistoryItemDelegateInternals::NUMBER_COLOR);
	painter->drawText(QRect(0, rect.top(), number_text_width, rect.height()), Qt::AlignHCenter | Qt::AlignVCenter, QString("%0.").arg(index.row() + 1));
	painter->setPen(HistoryItemDelegateInternals::TEXT_COLOR);
	painter->drawText(QRect(number_text_width, rect.top(), rect.width() - number_text_width, rect.height()), Qt::AlignLeft | Qt::AlignVCenter, text);
	*/
	painter->restore();
}

QSize HistoryItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
	return QSize(m_CharacterSize.width() * (HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH + 10), m_CharacterSize.height() + 8);
}
