#include "PhStripCutModel.h"

PhStripCutModel::PhStripCutModel(QObject * parent) :
	QAbstractListModel(parent)
{
}

void PhStripCutModel::append(PhStripCut *cut)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_cuts << cut;
	endInsertRows();
}

int PhStripCutModel::rowCount(const QModelIndex & parent) const {
	return _cuts.count();
}

QVariant PhStripCutModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _cuts.count())
		return QVariant();

	PhStripCut *cut = _cuts[index.row()];

	if (role == TimeRole)
		return cut->timeIn();

	if (role == TypeRole)
		return cut->type();

	return QVariant();
}

bool PhStripCutModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _cuts.count())
		return false;

	PhStripCut *cut = _cuts[index.row()];

	if (role == TimeRole) {
		if (cut->timeIn() != value.toInt()) {
			cut->setTimeIn(value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}

	return false;
}

bool PhStripCutModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (count > 0)
	{
		beginRemoveRows(parent, row, row + count - 1);
		PhStripCut *cut = _cuts.takeAt(row);
		delete cut;
		endRemoveRows();

		return true;
	}

	return false;
}

void PhStripCutModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_cuts);
	_cuts.clear();
	endRemoveRows();
}

QListIterator<PhStripCut *> PhStripCutModel::iterator()
{
	return QListIterator<PhStripCut *>(_cuts);
}

QHash<int, QByteArray> PhStripCutModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[TimeRole] = "time";
	roles[TypeRole] = "type";
	return roles;
}

void PhStripCutModel::add(PhTime time)
{
	// verify that a cut does not already exist at this time
	foreach(PhStripCut *cut, _cuts) {
		if (cut->timeIn() == time) {
			return;
		}
	}

	PhStripCut::PhCutType type = PhStripCut::Simple;
	append(new PhStripCut(time, type));
}

void PhStripCutModel::remove(int index)
{
	removeRow(index);
}
