#include "HistoryItemModel.h"

#include <gsl/gsl>

#include <FuzzySearch.h>

#include "HistoryItem.h"
#include "HistoryViewConstants.h"

HistoryItemModel::HistoryItemModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}

QVariant HistoryItemModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int source_index = MapToSource(index);
		return QString::fromStdString(m_HistoryItems[source_index].m_DisplayText);
	}
	// assert(false, "data for role %d not implemented", role);
	return QVariant();
}

int HistoryItemModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
	{
		return gsl::narrow_cast<int>(m_HistoryItems.size());
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
	if (row >= 0 && column >= 0 && row < m_HistoryItems.size() && !parent.isValid())
	{
		return createIndex(row, column);
	}
	return QModelIndex();
}

QModelIndex HistoryItemModel::parent(const QModelIndex&) const
{
	return QModelIndex();
}

bool HistoryItemModel::UpdateFilterPattern(const QString& pattern, bool force)
{
	std::string pattern_str = pattern.toStdString();
	if (!force && m_FilterPattern == pattern_str)
		return false;

	m_FilterPattern = pattern_str;
	Invalidate();

	return true;
}

void HistoryItemModel::AddToHistory(HistoryItemData history_item_data)
{
	//! Check if the history already contains given item
	auto history_item_found = m_TextHashToIndex.find(history_item_data.m_TextHash);
	if (history_item_found != m_TextHashToIndex.end())
	{
		size_t data_item_index = history_item_found->second;
		HistoryItemData& data_by_text_hash = m_HistoryItems[data_item_index].m_HistoryItemData;
		//! The item already existed in history, replace our timestamp in the model with the new one
		data_by_text_hash.m_Timestamp = history_item_data.m_Timestamp;
		Invalidate();
	}
	else
	{
		//! Inserting a new item, look up the place where to insert it in the proxy mapping
		auto timestamp_compare = [this](const HistoryItem& item, size_t timestamp) { return item.m_HistoryItemData.m_Timestamp <= timestamp; };
		auto timestamp_found = std::lower_bound(m_HistoryItems.begin(), m_HistoryItems.end(), history_item_data.m_Timestamp, timestamp_compare);

		size_t insert_at_index = std::distance(m_HistoryItems.begin(), timestamp_found);
		int proxy_index = gsl::narrow<int>(m_HistoryItems.size() - insert_at_index);

		m_HistoryItems.emplace(m_HistoryItems.begin() + insert_at_index, history_item_data);
		if (!IsFilterEnabled())
		{
			beginInsertRows(QModelIndex(), proxy_index, proxy_index);
			m_TextHashToIndex[history_item_data.m_TextHash] = insert_at_index;
			m_ProxyMapping.
			endInsertRows();
		}
		else
		{
			Invalidate();
		}
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
	int source_index = MapToSource(createIndex(index, 0));
	return m_HistoryItems.at(source_index).m_HistoryItemData;
}

void HistoryItemModel::Clear()
{
	beginRemoveRows(QModelIndex(), 0, gsl::narrow_cast<int>(m_HistoryItems.size()));
	m_HistoryItems.clear();
	m_ProxyMapping.clear();
	endRemoveRows();
}

bool HistoryItemModel::IsFilterEnabled() const
{
	return m_FilterPattern.size() > 0;
}

void HistoryItemModel::Invalidate()
{
	beginResetModel();
	RebuildProxyMapping();
	endResetModel();
}

void HistoryItemModel::RebuildProxyMapping()
{
	if (IsFilterEnabled())
	{
		m_ProxyMapping.clear();
		m_ProxyMapping.reserve(m_HistoryItems.size());

		int history_item_index = 0;
		for (HistoryItem& history_item : m_HistoryItems)
		{
			history_item.m_PatternMatch = FuzzySearch::FuzzyMatch(m_FilterPattern, history_item.m_HistoryItemData.m_Text, FuzzySearch::MatchMode::E_STRINGS);
			if (history_item.m_PatternMatch.m_Score > 0)
			{
				m_ProxyMapping.push_back(history_item_index);
			}
			else
			{
				m_ProxyMapping.push_back(-1);
			}
			++history_item_index;
		}

		std::sort(m_ProxyMapping.begin(), m_ProxyMapping.end(), [this](int lhs, int rhs) { return LessThan(lhs, rhs); });
	}

	//m_TimestampMapping.push_back()
	m_ProxyMapping.resize(m_HistoryItems.size());
	std::iota(m_ProxyMapping.rbegin(), m_ProxyMapping.rend(), 0);
}

int HistoryItemModel::MapToSource(QModelIndex index) const
{
	if (index.row() >= 0)
	{
		return m_ProxyMapping.at(index.row());
	}
	else
	{
		return -1;
	}
}

bool HistoryItemModel::LessThan(int lhs_index, int rhs_index)
{
	if (lhs_index >= 0 && rhs_index >= 0)
	{
		const FuzzySearch::PatternMatch& left_match = m_HistoryItems[lhs_index].m_PatternMatch;
		const FuzzySearch::PatternMatch& right_match = m_HistoryItems[rhs_index].m_PatternMatch;

		if (left_match.m_Score > right_match.m_Score)
		{
			return true;
		}
		else if (left_match.m_Score == right_match.m_Score)
		{
			const HistoryItemData& left_data = m_HistoryItems[lhs_index].m_HistoryItemData;
			const HistoryItemData& right_data = m_HistoryItems[rhs_index].m_HistoryItemData;

			return left_data.m_Text.size() < right_data.m_Text.size();
		}
	}
	else
	{
		if (lhs_index >= 0)
		{
			return true;
		}
	}
	return false;
}
