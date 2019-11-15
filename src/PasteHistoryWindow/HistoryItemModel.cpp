#include "HistoryItemModel.h"

#include <gsl/gsl>

#include <FuzzySearch.h>

#include "HistoryItem.h"
#include "HistoryViewConstants.h"

std::vector<size_t> BuildFilterMapping(std::string_view filter_pattern, std::vector<HistoryItem>& history_items)
{
	std::vector<size_t> filter_proxy_mapping;
	filter_proxy_mapping.reserve(history_items.size());

	std::vector<FuzzySearch::SearchResult> search_results;
	search_results.reserve(history_items.size());

	FuzzySearch::InputPattern input_pattern(filter_pattern);

	for (size_t history_item_index = 0; history_item_index < history_items.size(); ++history_item_index)
	{
		HistoryItemData& history_item_data = history_items[history_item_index].m_HistoryItemData;
		FuzzySearch::PatternMatch pattern_match =
		    FuzzySearch::FuzzyMatch(input_pattern, history_item_data.m_Text, FuzzySearch::MatchMode::E_STRINGS);

		search_results.push_back({history_item_data.m_Text, pattern_match});
		if (pattern_match.m_Score > 0)
		{
			filter_proxy_mapping.push_back(history_item_index);
			history_items[history_item_index].m_Match = pattern_match;
		}
	}

	std::sort(filter_proxy_mapping.begin(), filter_proxy_mapping.end(), [&search_results](size_t lhs, size_t rhs) {
		return FuzzySearch::SearchResultComparator(search_results[lhs], search_results[rhs]);
	});

	return filter_proxy_mapping;
}

HistoryItemModel::HistoryItemModel(QObject* parent)
    : QAbstractItemModel(parent)
{
	m_HistoryItems.reserve(128);
}

QVariant HistoryItemModel::data(const QModelIndex& index, int role) const
{
	QVariant result;

	int source_index = MapToSource(index);
	if (source_index < 0 || source_index >= static_cast<int>(m_HistoryItems.size()))
		return result;

	if (role == Qt::DisplayRole)
	{
		result.setValue(m_HistoryItems[source_index].m_DisplayText);
	}
	else if (role == Qt::ToolTipRole)
	{
		result.setValue(QString::fromStdString(m_HistoryItems[source_index].m_HistoryItemData.m_Text));
	}
	else if (role == HistoryViewConstants::HISTORY_ITEM_FILTER_MATCH)
	{
		if (IsFilterEnabled())
		{
			result.setValue(m_HistoryItems[source_index].m_Match);
		}
		else
		{
			result.setValue(FuzzySearch::PatternMatch());
		}
	}

	return result;
}

int HistoryItemModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
	{
		if (IsFilterEnabled())
		{
			return gsl::narrow_cast<int>(m_FilterProxyMapping.size());
		}
		else
		{
			return gsl::narrow_cast<int>(m_HistoryItems.size());
		}
	}
	return 0;
}

int HistoryItemModel::columnCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
	{
		return 1;
	}
	return 0;
}

QModelIndex HistoryItemModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && column >= 0 && row < gsl::narrow_cast<int>(m_HistoryItems.size()) && !parent.isValid())
	{
		return createIndex(row, column);
	}
	return QModelIndex();
}

QModelIndex HistoryItemModel::parent(const QModelIndex&) const
{
	return QModelIndex();
}

bool HistoryItemModel::UpdateFilterPattern(const QString& pattern)
{
	std::string pattern_str = pattern.trimmed().toStdString();
	if (m_FilterPattern == pattern_str)
		return false;

	m_FilterPattern = pattern_str;
	Invalidate();

	return true;
}

void HistoryItemModel::AddToHistory(HistoryItemData history_item_data)
{
	//! Check if the history already contains given item
	auto history_item_found = m_TextHashToTimestampIndex.find(history_item_data.m_TextHash);
	if (history_item_found != m_TextHashToTimestampIndex.end())
	{
		HistoryItem& history_item = m_HistoryItems[history_item_found->second];
		UpdateTimestampForHistoryItem(history_item.m_HistoryItemData, history_item_data.m_Timestamp);
	}
	else
	{
		AddNewHistoryItem(history_item_data);
	}
}

void HistoryItemModel::AddToHistory(const std::vector<HistoryItemData>& history_items_data)
{
	for (const HistoryItemData& history_item_data : history_items_data)
	{
		AddToHistory(history_item_data);
	}
}

const HistoryItemData& HistoryItemModel::GetHistoryItemData(int index) const
{
	return GetHistoryItemData(createIndex(index, 0));
}

const HistoryItemData& HistoryItemModel::GetHistoryItemData(QModelIndex index) const
{
	int source_index = MapToSource(index);
	return m_HistoryItems[source_index].m_HistoryItemData;
}

void HistoryItemModel::Clear()
{
	beginRemoveRows(QModelIndex(), 0, gsl::narrow_cast<int>(m_HistoryItems.size() - 1));
	m_HistoryItems.clear();
	m_FilterProxyMapping.clear();
	m_TextHashToTimestampIndex.clear();
	endRemoveRows();
}

bool HistoryItemModel::IsFilterEnabled() const
{
	return m_FilterPattern.size() > 0;
}

void HistoryItemModel::Invalidate()
{
	beginResetModel();
	if (IsFilterEnabled())
	{
		m_FilterProxyMapping = BuildFilterMapping(m_FilterPattern, m_HistoryItems);
	}

	endResetModel();
}

int HistoryItemModel::MapToSource(QModelIndex index) const
{
	if (index.row() >= 0)
	{
		if (IsFilterEnabled())
		{
			size_t row_index = gsl::narrow_cast<size_t>(index.row());
			Ensures(row_index < m_FilterProxyMapping.size());
			return gsl::narrow_cast<int>(m_FilterProxyMapping[row_index]);
		}
		else
		{
			return gsl::narrow_cast<int>(m_HistoryItems.size()) - index.row() - 1;
		}
	}
	else
	{
		return -1;
	}
}

void HistoryItemModel::AddNewHistoryItem(const HistoryItemData& history_item_data)
{
	auto timestamp_compare = [](const HistoryItem& item, size_t timestamp) { return item.m_HistoryItemData.m_Timestamp <= timestamp; };
	auto timestamp_found = std::lower_bound(m_HistoryItems.begin(), m_HistoryItems.end(), history_item_data.m_Timestamp, timestamp_compare);

	size_t insert_at_index = std::distance(m_HistoryItems.begin(), timestamp_found);
	Ensures(insert_at_index <= m_HistoryItems.size());

	if (!IsFilterEnabled())
	{
		const int proxy_index = static_cast<int>(m_HistoryItems.size() - insert_at_index);

		beginInsertRows(QModelIndex(), proxy_index, proxy_index);
		m_HistoryItems.emplace(m_HistoryItems.begin() + insert_at_index, history_item_data);
		m_TextHashToTimestampIndex[history_item_data.m_TextHash] = insert_at_index;
		endInsertRows();
	}
	else
	{
		m_HistoryItems.emplace(m_HistoryItems.begin() + insert_at_index, history_item_data);
		Invalidate();
	}
}

void HistoryItemModel::UpdateTimestampForHistoryItem(HistoryItemData& history_item_data, size_t timestamp)
{
	history_item_data.m_Timestamp = timestamp;
	std::sort(m_HistoryItems.begin(), m_HistoryItems.end(), [](const HistoryItem& lhs, const HistoryItem& rhs) {
		return lhs.m_HistoryItemData.m_Timestamp <= rhs.m_HistoryItemData.m_Timestamp;
	});

	for (size_t index = 0; index < m_HistoryItems.size(); ++index)
	{
		m_TextHashToTimestampIndex[m_HistoryItems[index].m_HistoryItemData.m_TextHash] = index;
	}

	Invalidate();
}
