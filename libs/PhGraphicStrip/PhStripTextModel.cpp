#include "PhStripTextModel.h"

PhStripTextModel::PhStripTextModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhStripTextModel::append(PhStripText *stripText)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_stripTexts << stripText;
	endInsertRows();
}

int PhStripTextModel::rowCount(const QModelIndex & parent) const {
	return _stripTexts.count();
}

QVariant PhStripTextModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _stripTexts.count())
		return QVariant();
	PhStripText *stripText = _stripTexts[index.row()];
	if (role == ContentRole)
		return stripText->content();
	else if (role == TrackNumberRole)
		return stripText->y();
	//else if (role == ColorRole)
	//	return stripText->color();
	else if (role == TimeInRole)
		return stripText->timeIn();
	else if (role == SelectedRole)
		return stripText->selected();
	else if (role == TimeOutRole)
		return stripText->timeOut();
	return QVariant();
}

bool PhStripTextModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _stripTexts.count())
		return false;
	PhStripText *stripText = _stripTexts[index.row()];

	// TODO emit dataChanged?
	if (role == ContentRole) {
		stripText->setContent(value.toString());
		return true;
	}
	else if (role == TimeInRole) {
		stripText->setTimeIn(value.toInt());
		return true;
	}
	else if (role == TimeOutRole) {
		stripText->setTimeOut(value.toInt());
		return true;
	}
	else if (role == TrackNumberRole) {
		stripText->setY(value.toFloat());
		return true;
	}

	return false;
}

bool PhStripTextModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	PhStripText *text = _stripTexts.takeAt(row);
	delete text;
	endRemoveRows();
}

QHash<int, QByteArray> PhStripTextModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ContentRole] = "content";
	roles[TrackNumberRole] = "trackNumber";
	//roles[ColorRole] = "color";
	roles[TimeInRole] = "timeIn";
	roles[SelectedRole] = "selected";
	roles[TimeOutRole] = "timeOut";
	return roles;
}

void PhStripTextModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_stripTexts);
	_stripTexts.clear();
	endRemoveRows();
}

void PhStripTextModel::addText(PhTime timeIn, PhTime timeOut, float y)
{
	float height = _stripTexts.first()->height();
	append(new PhStripText(timeIn, _stripTexts.first()->people(), timeOut, y, "phrase", height));
}

void PhStripTextModel::removeText(int index)
{
	removeRow(index);
}
