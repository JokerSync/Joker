#ifndef PHSTRIPFILTEREDLINEMODEL_H
#define PHSTRIPFILTEREDLINEMODEL_H

#include <QSortFilterProxyModel>
#include "PhSync/PhClock.h"

class PhStripFilteredLineModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	PhStripFilteredLineModel(QObject *parent = 0);

	PhTime filterMinimumTime() const {
		return _minTime;
	}
	PhTime filterMaximumTime() const {
		return _maxTime;
	}

	void setFilterTimeBoundaries(const PhTime &minTime, const PhTime &maxTime);

public slots:
	void remove(int index);

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
	PhTime _minTime;
	PhTime _maxTime;
};

#endif // PHSTRIPFILTEREDLINEMODEL_H
