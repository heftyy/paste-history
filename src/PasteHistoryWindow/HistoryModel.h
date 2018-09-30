#pragma once

#include <QSortFilterProxyModel>

class HistoryModel : public QSortFilterProxyModel
{
public:
	HistoryModel(QObject* parent);

	QVariant data(const QModelIndex& index, int role) const override;
	bool UpdateFilterPattern(const QString& pattern); //!< Updates the filter pattern and might force a filter and resort, returns true if proxy model changed

protected:
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
	std::string m_FilterPattern;

	bool FilteringEnabled() const;
};