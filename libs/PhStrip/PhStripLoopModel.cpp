#include "PhStripLoopModel.h"

PhStripLoopModel::PhStripLoopModel(QObject * parent) :
	QAbstractListModel(parent)
{
}

void PhStripLoopModel::append(PhStripLoop *loop)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_loops << loop;
	endInsertRows();
}

int PhStripLoopModel::rowCount(const QModelIndex & parent) const {
	return _loops.count();
}

QVariant PhStripLoopModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _loops.count())
		return QVariant();

	PhStripLoop *loop = _loops[index.row()];

	if (role == TimeRole)
		return loop->timeIn();

	if (role == LabelRole)
		return loop->label();

	return QVariant();
}

bool PhStripLoopModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _loops.count())
		return false;

	PhStripLoop *loop = _loops[index.row()];

	if (role == TimeRole) {
		if (loop->timeIn() != value.toInt()) {
			loop->setTimeIn(value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}

	if (role == LabelRole) {
		if (loop->label() != value.toString()) {
			loop->setLabel(value.toString());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}

	return false;
}

bool PhStripLoopModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	PhStripLoop *loop = _loops.takeAt(row);
	delete loop;
	endRemoveRows();
}

void PhStripLoopModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_loops);
	_loops.clear();
	endRemoveRows();
}

QListIterator<PhStripLoop *> PhStripLoopModel::iterator()
{
	return QListIterator<PhStripLoop *>(_loops);
}

QHash<int, QByteArray> PhStripLoopModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[TimeRole] = "time";
	roles[LabelRole] = "label";
	return roles;
}

void PhStripLoopModel::add(PhTime time, QString label)
{
	// verify that a loop does not already exist at this time
	foreach(PhStripLoop *loop, _loops) {
		if (loop->timeIn() == time) {
			return;
		}
	}

	// TODO: auto-numbering
	if (label == "") {
		label = "default";
	}

	append(new PhStripLoop(time, label));
}

void PhStripLoopModel::remove(int index)
{
	removeRow(index);
}
