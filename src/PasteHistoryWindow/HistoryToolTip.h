#pragma once

#include <string_view>

#include <QPoint>
#include <QWidget>

class HistoryToolTip : public QWidget
{
	Q_OBJECT

public:
	HistoryToolTip(QWidget* parent)
	    : QWidget(parent)
	{
		setWindowFlags(Qt::ToolTip);
	}

	void SetupToolTip(std::string_view, QPoint left, QPoint right);
	void paintEvent(QPaintEvent* event) override;

private:
	QString m_ToolTipText;
	QRect m_ToolTipRect;
};