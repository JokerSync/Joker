#include "PhStripLineModel.h"

PhStripLineModel::PhStripLineModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhStripLineModel::append(PhStripLine *line)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_lines << line;
	endInsertRows();
}

int PhStripLineModel::rowCount(const QModelIndex & parent) const {
	return _lines.count();
}

QVariant PhStripLineModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _lines.count())
		return QVariant();
	PhStripLine *line = _lines[index.row()];
	if (role == ContentRole)
		return line->content();
	else if (role == TrackNumberRole)
		return line->y();
	//else if (role == ColorRole)
	//	return stripText->color();
	else if (role == TimeInRole)
		return line->timeIn();
	else if (role == TimeOutRole)
		return line->timeOut();
	else if (role == TextsRole)
		return QVariant::fromValue<QObject*>(line->textModel());
	else if (role == DetectsRole)
		return QVariant::fromValue<QObject*>(line->detectModel());
	return QVariant();
}

bool PhStripLineModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _lines.count())
		return false;
	PhStripLine *line = _lines[index.row()];

	// TODO emit dataChanged?
	if (role == ContentRole) {
		line->setContent(value.toString());
		return true;
	}
	else if (role == TimeInRole) {
		line->setTimeIn(value.toInt());
		return true;
	}
	else if (role == TimeOutRole) {
		line->setTimeOut(value.toInt());
		return true;
	}
	else if (role == TrackNumberRole) {
		line->setY(value.toFloat());
		return true;
	}

	return false;
}

bool PhStripLineModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	PhStripLine *line = _lines.takeAt(row);
	delete line;
	endRemoveRows();
}

QHash<int, QByteArray> PhStripLineModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ContentRole] = "content";
	roles[TrackNumberRole] = "trackNumber";
	roles[TimeInRole] = "timeIn";
	roles[TimeOutRole] = "timeOut";
	roles[TextsRole] = "texts";
	roles[DetectsRole] = "detects";
	return roles;
}

void PhStripLineModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_lines);
	_lines.clear();
	endRemoveRows();
}

void PhStripLineModel::add(PhTime timeIn, PhTime timeOut, float y)
{
	float height = _lines.first()->height();
	append(new PhStripLine(timeIn, _lines.first()->people(), timeOut, y, "phrase", height));
}

void PhStripLineModel::remove(int index)
{
	removeRow(index);
}
