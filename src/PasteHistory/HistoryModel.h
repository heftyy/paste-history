#pragma once

#include <QSortFilterProxyModel>

class HistoryModel : public QSortFilterProxyModel
{
public:
	HistoryModel(QObject* parent);

	QVariant data(const QModelIndex& index, int role) const override;
	void SetFilterPattern(const QString& pattern);

protected:
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
	std::string m_FilterPattern;

	bool FilteringEnabled() const;
};