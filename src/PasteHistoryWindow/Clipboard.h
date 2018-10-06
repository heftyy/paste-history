#pragma once

#include <QObject>

class Clipboard : public QObject
{
	Q_OBJECT

public:
	Clipboard(QObject* parent);

Q_SIGNALS:
	void DataChanged(const std::string& text);

private:
	class QTimer* m_RetryTimer;

	void OnClipboardChanged();
	void RetrieveClipboardData();
};