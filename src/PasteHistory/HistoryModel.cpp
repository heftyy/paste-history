#include "HistoryModel.h"
#include "HistoryView.h"

#include <QBrush>

#include <FuzzySearch.h>

HistoryModel::HistoryModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());

		QStandardItem* item = source_model->itemFromIndex(mapToSource(index));
		HistoryViewItem* history_view_item = static_cast<HistoryViewItem*>(item);

		const HistoryItemData& item_data = history_view_item->GetHistoryItemData();

		QString display_string = item->data(Qt::DisplayRole).toString();
		return QString(display_string + "|%0|%1").arg(item_data.m_Timestamp).arg(item_data.m_MatchScore);
	}

	return QSortFilterProxyModel::data(index, role);
}

bool HistoryModel::UpdateFilterPattern(const QString& pattern)
{
	std::string pattern_str = pattern.toStdString();
	if (m_FilterPattern == pattern_str)
		return false;

	m_FilterPattern = pattern_str;
	invalidate();

	return true;
}

bool HistoryModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());

	HistoryViewItem* left_item = static_cast<HistoryViewItem*>(source_model->itemFromIndex(left));
	HistoryViewItem* right_item = static_cast<HistoryViewItem*>(source_model->itemFromIndex(right));

	HistoryItemData& left_item_data = left_item->GetHistoryItemData();
	HistoryItemData& right_item_data = right_item->GetHistoryItemData();

	if (FilteringEnabled())
	{
		return left_item_data.m_MatchScore < right_item_data.m_MatchScore;
	}
	else
	{
		return left_item_data.m_Timestamp < right_item_data.m_Timestamp;
	}
}

bool HistoryModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	if (!FilteringEnabled())
	{
		return true;
	}

	QStandardItemModel* source_model = qobject_cast<QStandardItemModel*>(sourceModel());
	QModelIndex index = source_model->index(source_row, 0, source_parent);

	QStandardItem* item = source_model->itemFromIndex(index);
	HistoryViewItem* history_view_item = static_cast<HistoryViewItem*>(item);

	HistoryItemData& item_data = history_view_item->GetHistoryItemData();

	item_data.m_MatchScore = FuzzySearch::FuzzyMatch(m_FilterPattern, item_data.m_Text, 0, FuzzySearch::MatchMode::E_STRINGS, item_data.m_Matches);

	return item_data.m_MatchScore > 0;
}

bool HistoryModel::FilteringEnabled() const
{
	return m_FilterPattern.size() > 0;
}
