#include "HistoryView.h"
#include "HistoryModel.h"

#include <QKeyEvent>
#include <QStandardItemModel>

#include <algorithm>
#include <chrono>

HistoryViewItem::HistoryViewItem(HistoryItemData* item_data)
    : m_HistoryItemData(item_data)
{
	QVariant variant;
	variant.setValue(item_data);
	setData(variant, HistoryListViewConstants::HISTORY_ITEM_DATA_ROLE);

	setEditable(false);
}

HistoryViewItem::~HistoryViewItem()
{
}

HistoryView::HistoryView(QWidget* parent)
    : QListView(parent)
{
	m_SourceModel = new QStandardItemModel(this);
	m_HistoryModel = new HistoryModel(this);
	m_HistoryModel->setSourceModel(m_SourceModel);
	m_HistoryModel->sort(0, Qt::DescendingOrder);
	setModel(m_HistoryModel);

	installEventFilter(this);
}

HistoryView::~HistoryView()
{
}

void HistoryView::AddToHistory(QString text, size_t text_hash, size_t timestamp)
{
	auto found = std::lower_bound(m_History.begin(), m_History.end(), text_hash,
	                              [](const HistoryItemDataPtr& item, size_t text_hash) { return item->m_TextHash < text_hash; });

	if (found != m_History.end() && (*found)->m_TextHash == text_hash)
	{
		(*found)->m_Timestamp = timestamp;
	}
	else
	{
		auto iter = m_History.emplace(found, std::make_unique<HistoryItemData>(text.toStdString(), text_hash, timestamp));
		int index = std::distance(m_History.begin(), iter);

		HistoryViewItem* history_view_item = new HistoryViewItem(iter->get());
		m_SourceModel->insertRow(index, history_view_item);
	}
}

void HistoryView::SetFilterPattern(QString pattern)
{
	m_HistoryModel->SetFilterPattern(pattern);
}

bool HistoryView::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
		bool control_pressed = key_event->modifiers() & Qt::CTRL;
		bool number_pressed = key_event->key() >= Qt::Key_0 && key_event->key() <= Qt::Key_9;
		return control_pressed && number_pressed;
	}

	return QListView::eventFilter(obj, event);
}
