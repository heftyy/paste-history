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
	bool UpdateFilterPattern(const QString& pattern, bool force = false);
	void AddToHistory(HistoryItemData history_item_data);
	void AddToHistory(const std::vector<HistoryItemData>& history_items_data);	

	const HistoryItemData& GetHistoryItemData(int index) const;
	void Clear();

private:
	std::string m_FilterPattern;
	std::vector<HistoryItem> m_HistoryItems;
	std::vector<int> m_ProxyMapping;
	std::unordered_map<size_t, size_t> m_TextHashToIndex;

	bool m_IsFilteringEnabled = false;

	bool IsFilterEnabled() const;
	void Invalidate();

	void RebuildProxyMapping();
	int MapToSource(QModelIndex index) const;

	bool LessThan(int lhs_index, int rhs_index);
};