#include "Clipboard.h"

#include <chrono>

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

	m_DelayTimer = new QTimer(this);
	m_DelayTimer->setInterval(std::chrono::milliseconds(10));
	m_DelayTimer->setSingleShot(true);
	connect(m_DelayTimer, &QTimer::timeout, this, &Clipboard::RetrieveClipboardData);
}

void Clipboard::OnClipboardChanged()
{
	//! (QTBUG-53979)
	//! Unfortunately some programs don't release the clipboard handle soon enough so we end up clipboard->text() being empty.
	//! Adding a delay timer seems like a decent workaround even though it makes me pretty sad.
	m_DelayTimer->start();
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
