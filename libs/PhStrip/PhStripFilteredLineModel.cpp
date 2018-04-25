#include "PhStripFilteredLineModel.h"
#include "PhStripLineModel.h"
#include "PhTools/PhDebug.h"

PhStripFilteredLineModel::PhStripFilteredLineModel(QObject *parent)
	: QSortFilterProxyModel(parent),
	_minTime(PHTIMEMIN),
	_maxTime(PHTIMEMAX)
{
}

void PhStripFilteredLineModel::setFilterTimeBoundaries(const PhTime &minTime, const PhTime &maxTime)
{
	if (minTime != _minTime || maxTime != _maxTime) {
		_minTime = minTime;
		_maxTime = maxTime;
		invalidateFilter();
	}
}

void PhStripFilteredLineModel::remove(int index)
{
	removeRow(index);
}

bool PhStripFilteredLineModel::filterAcceptsRow(int sourceRow,
                                                const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	PhTime timeIn = (PhTime) sourceModel()->data(index, PhStripLineModel::TimeInRole).toInt();
	PhTime duration = (PhTime) sourceModel()->data(index, PhStripLineModel::DurationRole).toInt();
	PhTime timeOut = timeIn + duration;

	return timeOut > _minTime && timeIn < _maxTime;
}
