#include "Clipboard.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QTimer>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <qt_windows.h>
#endif

Clipboard::Clipboard(QObject* parent)
    : QObject(parent)
{
	const QClipboard* clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::dataChanged, this, &Clipboard::OnClipboardChanged);

	m_RetryTimer = new QTimer(this);
	m_RetryTimer->setInterval(std::chrono::milliseconds(10));
	m_RetryTimer->setSingleShot(true);
	connect(m_RetryTimer, &QTimer::timeout, this, &Clipboard::RetrieveClipboardData);
}

void Clipboard::OnClipboardChanged()
{
	m_RetryTimer->start();
}

void Clipboard::RetrieveClipboardData()
{
#ifdef WIN32
	const DWORD newClipboardSequenceNumber = GetClipboardSequenceNumber();
	qDebug() << "clipboard seq: " << newClipboardSequenceNumber;
#endif

	QString text = QApplication::clipboard()->text();
	if (!text.isEmpty())
	{
		Q_EMIT DataChanged(text.toStdString());
	}
}
