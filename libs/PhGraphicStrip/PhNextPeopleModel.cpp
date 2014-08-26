#include "PhNextPeopleModel.h"

PhNextPeopleModel::PhNextPeopleModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhNextPeopleModel::addNextPeople(const PhNextPeople *nextPeople)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_nextPeoples << nextPeople;
	endInsertRows();
}

int PhNextPeopleModel::rowCount(const QModelIndex & parent) const {
	return _nextPeoples.count();
}

QVariant PhNextPeopleModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _nextPeoples.count())
		return QVariant();
	const PhNextPeople *nextPeople = _nextPeoples[index.row()];
	if (role == NameRole)
		return nextPeople->name();
	else if (role == ColorRole)
		return nextPeople->color();
	else if (role == TimeInRole)
		return nextPeople->timeIn();
	else if (role == SelectedRole)
		return nextPeople->selected();
	else if (role == DurationRole)
		return nextPeople->duration();
	return QVariant();
}

QHash<int, QByteArray> PhNextPeopleModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[NameRole] = "name";
	roles[ColorRole] = "color";
	roles[TimeInRole] = "timeIn";
	roles[SelectedRole] = "selected";
	roles[DurationRole] = "duration";
	return roles;
}

void PhNextPeopleModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_nextPeoples);
	_nextPeoples.clear();
	endRemoveRows();
}
