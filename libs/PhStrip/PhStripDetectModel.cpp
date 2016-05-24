#include "PhStripDetectModel.h"

PhStripDetectModel::PhStripDetectModel(QObject * parent) :
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

	if (role == TimeRole)
		return detect->relativeTime();

	return QVariant();
}

bool PhStripDetectModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _detects.count())
		return false;

	PhStripDetect *detect = _detects[index.row()];

	if (role == TimeRole) {
		if (detect->relativeTime() != value.toInt()) {
			detect->setRelativeTime(value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
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
	roles[TimeRole] = "time";
	return roles;
}

void PhStripDetectModel::add(PhTime time)
{
	PhStripDetect::PhDetectType type = PhStripDetect::Labial;
	append(new PhStripDetect(type, time));
}

void PhStripDetectModel::remove(int index)
{
	removeRow(index);
}
