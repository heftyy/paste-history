#pragma once

#include <string>
#include <vector>

#include <QSortFilterProxyModel>

#include "HistoryItem.h"

class HistoryItemModel : public QSortFilterProxyModel
{
public:
	HistoryItemModel(QObject* parent);

	bool UpdateFilterPattern(const QString& pattern, bool force = false); //!< Updates the filter pattern and might force a filter and resort, returns true if proxy model changed
	bool AddToHistory(HistoryItemData history_item_data);
	bool AddToHistory(const std::vector<HistoryItemData>& history_items_data);

protected:
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
	struct FilterMatch
	{
		int m_MatchScore;
		std::vector<int> m_Matches;
	};

	std::string m_FilterPattern;
	std::vector<FilterMatch> m_FilterMatches;

	bool IsFilterEnabled() const;
};