#include "HistoryItemModel.h"

#include <QBrush>
#include <QStandardItemModel>

#include <FuzzySearch.h>

#include "HistoryItem.h"
#include "HistoryView.h"
#include "HistoryViewConstants.h"

HistoryItemModel::HistoryItemModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

/*
QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());

        QStandardItem* item = source_model->itemFromIndex(mapToSource(index));
        HistoryViewItem* history_view_item = dynamic_cast<HistoryViewItem*>(item);
        if (history_view_item)
        {
            const HistoryItemData& item_data = history_view_item->GetHistoryItemData();

            QString display_string = item->data(Qt::DisplayRole).toString();
            return QString(display_string + "|%0|%1").arg(item_data.m_Timestamp).arg(item_data.m_MatchScore);
        }
    }

    return QSortFilterProxyModel::data(index, role);
}
*/

bool HistoryItemModel::UpdateFilterPattern(const QString& pattern, bool force)
{
	std::string pattern_str = pattern.toStdString();
	if (!force && m_FilterPattern == pattern_str)
		return false;

	m_FilterPattern = pattern_str;
	invalidate();

	return true;
}

bool HistoryItemModel::AddToHistory(HistoryItemData history_item_data)
{
	QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());
	if (source_model)
	{
		HistoryItem* history_view_item = new HistoryItem(history_item_data);
		source_model->appendRow(history_view_item);

		return true;
	}
	return false;
}

bool HistoryItemModel::AddToHistory(const std::vector<HistoryItemData>& history_items_data)
{
	QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());
	if (source_model)
	{
		for (const HistoryItemData& history_item_data : history_items_data)
		{
			source_model->appendRow(new HistoryItem(history_item_data));
		}

		return true;
	}
	return false;
}

bool HistoryItemModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QStandardItemModel* source_model = static_cast<QStandardItemModel*>(sourceModel());

	HistoryItem* left_item = static_cast<HistoryItem*>(source_model->itemFromIndex(left));
	HistoryItem* right_item = static_cast<HistoryItem*>(source_model->itemFromIndex(right));

	if (IsFilterEnabled())
	{
		const FuzzySearch::PatternMatch& left_match = left_item->GetPatternMatch();
		const FuzzySearch::PatternMatch& right_match = right_item->GetPatternMatch();
		return left_match.m_Score < right_match.m_Score;
	}
	else
	{
		const HistoryItemData& left_item_data = left_item->GetHistoryItemData();
		const HistoryItemData& right_item_data = right_item->GetHistoryItemData();
		return left_item_data.m_Timestamp < right_item_data.m_Timestamp;
	}
}

bool HistoryItemModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	const QStandardItemModel* source_model = static_cast<QStandardItemModel*>(sourceModel());
	if (IsFilterEnabled() && source_model)
	{
		const QModelIndex index = source_model->index(source_row, 0, source_parent);

		QStandardItem* item = source_model->itemFromIndex(index);
		HistoryItem* history_view_item = static_cast<HistoryItem*>(item);
		if (history_view_item)
		{
			const HistoryItemData& item_data = history_view_item->GetHistoryItemData();
			FuzzySearch::PatternMatch match = FuzzySearch::FuzzyMatch(m_FilterPattern, item_data.m_Text, FuzzySearch::MatchMode::E_STRINGS);
			history_view_item->SetPatternMatch(match);
			return match.m_Score > 0;
		}
	}

	return true;
}

bool HistoryItemModel::IsFilterEnabled() const
{
	return m_FilterPattern.size() > 0;
}
