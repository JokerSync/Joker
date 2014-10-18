#include "PhStripTextModel.h"

PhStripTextModel::PhStripTextModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhStripTextModel::addStripText(const PhStripText *stripText)
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
	const PhStripText *stripText = _stripTexts[index.row()];
	if (role == ContentRole)
		return stripText->content();
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

QHash<int, QByteArray> PhStripTextModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ContentRole] = "content";
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
