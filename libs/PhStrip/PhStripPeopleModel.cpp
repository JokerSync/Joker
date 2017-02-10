#include "PhStripPeopleModel.h"

#include "PhTools/PhDebug.h"

PhStripPeopleModel::PhStripPeopleModel(QObject * parent) :
	QAbstractListModel(parent)
{
}

void PhStripPeopleModel::append(PhPeople *people)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_peoples << people;
	endInsertRows();
}

int PhStripPeopleModel::rowCount(const QModelIndex & parent) const {
	return _peoples.count();
}

QVariant PhStripPeopleModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _peoples.count())
		return QVariant();

	PhPeople *people = _peoples[index.row()];

	if (role == NameRole)
		return people->name();

	return QVariant();
}

bool PhStripPeopleModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _peoples.count())
		return false;

	PhPeople *people = _peoples[index.row()];

	PHDEBUG << value.toString();

	if (role == NameRole) {
		if (people->name() != value.toString()) {
			people->setName(value.toString());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}

	return false;
}

bool PhStripPeopleModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (count > 0)
	{
		beginRemoveRows(parent, row, row + count - 1);
		PhPeople *people = _peoples.takeAt(row);
		delete people;
		endRemoveRows();

		return true;
	}

	return false;
}

void PhStripPeopleModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_peoples);
	_peoples.clear();
	endRemoveRows();
}

QListIterator<PhPeople *> PhStripPeopleModel::iterator()
{
	return QListIterator<PhPeople *>(_peoples);
}

QHash<int, QByteArray> PhStripPeopleModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[NameRole] = "name";
	return roles;
}

void PhStripPeopleModel::add(QString name)
{
	create(name);
}

PhPeople* PhStripPeopleModel::create(QString name)
{
	QString finalName = name;
	// verify that a people does not already exist at this time
	int i = 1;
	while(indexOf(finalName) != -1) {
		finalName = name + " #" + QString::number(i);
		i++;
	}

	PhPeople *people = new PhPeople(finalName);
	append(people);

	return people;
}

void PhStripPeopleModel::remove(int index)
{
	removeRow(index);
}

int PhStripPeopleModel::indexOf(QString name)
{
	for(int i=0; i<rowCount(); i++) {
		PhPeople *people = _peoples[i];
		if (people->name() == name) {
			return i;
		}
	}
	return -1;
}

PhPeople* PhStripPeopleModel::findByName(QString name)
{
	for(int i=0; i<rowCount(); i++) {
		PhPeople *people = _peoples[i];
		if (people->name() == name) {
			return people;
		}
	}
	return NULL;
}
