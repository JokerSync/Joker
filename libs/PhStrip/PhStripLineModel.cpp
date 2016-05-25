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
	if (role == TrackNumberRole)
		return line->y();
	else if (role == TimeInRole)
		return line->timeIn();
	else if (role == TextsRole)
		return QVariant::fromValue<QObject*>(line->textModel());
	else if (role == UnlinkedDetectsRole)
		return QVariant::fromValue<QObject*>(line->detectModel());
	return QVariant();
}

bool PhStripLineModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _lines.count())
		return false;
	PhStripLine *line = _lines[index.row()];

	if (role == TimeInRole) {
		if (line->timeIn() != value.toInt()) {
			line->setTimeIn(value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}
	else if (role == TrackNumberRole) {
		if (line->y() != value.toFloat()) {
			line->setY(value.toFloat());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
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
	roles[TrackNumberRole] = "trackNumber";
	roles[TimeInRole] = "timeIn";
	roles[TextsRole] = "texts";
	roles[UnlinkedDetectsRole] = "unlinkedDetects";
	return roles;
}

void PhStripLineModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_lines);
	_lines.clear();
	endRemoveRows();
}

QListIterator<PhStripLine *> PhStripLineModel::iterator()
{
	return QListIterator<PhStripLine *>(_lines);
}

void PhStripLineModel::add(PhTime timeIn, PhStripDetect::PhDetectType typeIn, PhPeople *people, float y, float height)
{
	append(new PhStripLine(timeIn, typeIn, people, y, height));
}

void PhStripLineModel::remove(int index)
{
	removeRow(index);
}
