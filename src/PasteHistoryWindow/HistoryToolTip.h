#pragma once

#include <string_view>

#include <QWidget>
#include <QFont>

class HistoryToolTip : public QWidget
{
	Q_OBJECT

public:
	HistoryToolTip(QWidget* parent);

	void SetupToolTip(std::string_view, QPoint left, QPoint right);
	void paintEvent(QPaintEvent* event) override;

private:
	QFont m_Font;
	QString m_ToolTipText;
	QRect m_ToolTipRect;
};