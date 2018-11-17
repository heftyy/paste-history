#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <QSortFilterProxyModel>

#include "HistoryItem.h"

class HistoryItemModel : public QAbstractItemModel
{
public:
	HistoryItemModel(QObject* parent);

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex&) const override;

	//!< Updates the filter pattern and might force a filter and resort, returns true if proxy model changed
	bool UpdateFilterPattern(const QString& pattern);
	void AddToHistory(HistoryItemData history_item_data);
	void AddToHistory(const std::vector<HistoryItemData>& history_items_data);	

	const HistoryItemData& GetHistoryItemData(int index) const;
	void Clear();

private:
	std::string m_FilterPattern;
	std::vector<HistoryItem> m_HistoryItems;
	std::vector<size_t> m_FilterProxyMapping;
	std::unordered_map<size_t, size_t> m_TextHashToTimestampIndex;

	bool m_IsFilteringEnabled = false;

	bool IsFilterEnabled() const;
	void Invalidate();

	int MapToSource(QModelIndex index) const;

	//! Inserting a new item, look up the place where to insert it in the proxy mapping
	void AddNewHistoryItem(const HistoryItemData& history_item_data);
	//! The item already existed in history, replace our timestamp in the model with the new one
	void UpdateTimestampForHistoryItem(HistoryItemData& history_item, size_t timestamp);
};