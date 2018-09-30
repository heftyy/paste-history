#include "HistoryView.h"
#include "HistoryModel.h"

#include <QKeyEvent>
#include <QStandardItemModel>
#include <QVariant>

#include <algorithm>
#include <chrono>

HistoryViewItem::HistoryViewItem(HistoryItemData& item_data)
    : m_HistoryItemData(item_data)
{
	QVariant variant;
	variant.setValue(&item_data);
	setData(variant, HistoryViewConstants::HISTORY_ITEM_DATA_ROLE);

	QString text = QString::fromStdString(item_data.m_Text);
	QString display_string = text.simplified();
	if (display_string.length() > HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH)
	{
		const int display_string_length = display_string.length() - HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH;
		display_string.replace(HistoryViewConstants::DISPLAY_STRING_MAX_LENGTH, display_string_length, "...");
	}

	setData(display_string, Qt::DisplayRole);

	setEditable(false);
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
		size_t index = std::distance(m_History.begin(), iter);

		HistoryViewItem* history_view_item = new HistoryViewItem(*iter->get());
		m_SourceModel->insertRow(static_cast<int>(index), history_view_item);
	}
}

bool HistoryView::IsShortutKey(QKeyEvent* key_event)
{
	if (key_event->type() == QEvent::KeyPress || key_event->type() == QEvent::KeyRelease)
	{
		bool control_pressed = key_event->modifiers() & Qt::CTRL;
		bool number_pressed = key_event->key() >= Qt::Key_0 && key_event->key() <= Qt::Key_9;

		return control_pressed && number_pressed;
	}

	return false;
}

void HistoryView::UpdateFilterPattern(QString pattern)
{
	if (m_HistoryModel->UpdateFilterPattern(pattern))
	{
		update();
	}
}
