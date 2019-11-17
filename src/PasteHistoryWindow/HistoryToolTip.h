#pragma once

#include <QPoint>
#include <QString>
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

	void SetupToolTip(QString tooltip_text, QPoint left, QPoint right);
	void paintEvent(QPaintEvent* event) override;

private:
	QString m_ToolTipText;
	QRect m_ToolTipRect;
};