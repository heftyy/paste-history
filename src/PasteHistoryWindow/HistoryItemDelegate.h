#pragma once

#include <QStyledItemDelegate>
#include <QFont>

class HistoryItemDelegate : public QStyledItemDelegate
{
public:
	HistoryItemDelegate(QObject* parent);

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	QFont m_Font;
	QSize m_CharacterSize;
};