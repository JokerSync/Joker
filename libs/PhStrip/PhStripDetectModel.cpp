#include "PhStripDetectModel.h"

PhStripDetectModel::PhStripDetectModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhStripDetectModel::append(PhStripDetect *detect)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_detects << detect;
	endInsertRows();
}

int PhStripDetectModel::rowCount(const QModelIndex & parent) const {
	return _detects.count();
}

QVariant PhStripDetectModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _detects.count())
		return QVariant();
	PhStripDetect *detect = _detects[index.row()];
	if (role == TimeInRole)
		return detect->timeIn();
	return QVariant();
}

bool PhStripDetectModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _detects.count())
		return false;
	PhStripDetect *detect = _detects[index.row()];

	// TODO emit dataChanged?
	if (role == TimeInRole) {
		detect->setTimeIn(value.toInt());
		return true;
	}

	return false;
}

bool PhStripDetectModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	PhStripDetect *detect = _detects.takeAt(row);
	delete detect;
	endRemoveRows();
}

QHash<int, QByteArray> PhStripDetectModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[TimeInRole] = "timeIn";
	return roles;
}

void PhStripDetectModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_detects);
	_detects.clear();
	endRemoveRows();
}

void PhStripDetectModel::add(PhTime timeIn)
{
	PhStripDetect::PhDetectType type = PhStripDetect::Labial;
	PhPeople *people = _detects.first()->people();
	PhTime timeOut = timeIn;
	int y = 0;
	append(new PhStripDetect(type, timeIn, people, timeOut, y));
}

void PhStripDetectModel::remove(int index)
{
	removeRow(index);
}
