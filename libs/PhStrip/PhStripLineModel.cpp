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
	else if (role == PeopleNameRole)
		return line->people()->name();
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
	roles[PeopleNameRole] = "peopleName";
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

void PhStripLineModel::addDetect(PhTime time, float y)
{
	// this is called to create a new line, unless there is a line below the mouse
	// if there is a line below the mouse, and if this line is opened, this adds a closing sign
	// if that line is already closed, add un unlinked detect inside it

	foreach (PhStripLine *l, _lines)
	{
		if (l->timeIn() == time
				&& l->y() == y) {
			// there is a line starting there
			// return immediately
			return;
		}
	}

	foreach (PhStripLine *l, _lines)
	{
		if (l->textModel()->rowCount() == 0
				&& l->timeIn() <= time
				&& l->y() == y) {
			// found a matching opened line
			PhStripDetect::PhDetectType typeOut = PhStripDetect::On;
			PhStripText *text = new PhStripText("", time - l->timeIn(), typeOut);
			l->textModel()->append(text);
			return;
		}
	}

	foreach (PhStripLine *l, _lines)
	{
		PhTime timeOut = l->timeIn();
		QListIterator<PhStripText *> i = l->textModel()->iterator();
		while(i.hasNext()) {
			timeOut += i.next()->duration();
		}

		if (timeOut == time && l->y() == y) {
			// this is a line closing here
			// return immediately
			return;
		}
	}

	foreach (PhStripLine *l, _lines)
	{
		PhTime timeOut = l->timeIn();
		QListIterator<PhStripText *> i = l->textModel()->iterator();
		while(i.hasNext()) {
			timeOut += i.next()->duration();
		}

		if (l->timeIn() < time
				&& timeOut > time
				&& l->y() == y) {
			// found a matching closed line
			PhStripDetect::PhDetectType type = PhStripDetect::On;
			PhStripDetect *detect = new PhStripDetect(type, time - l->timeIn());
			l->detectModel()->append(detect);
			return;
		}
	}

	PhStripDetect::PhDetectType typeIn = PhStripDetect::On;
	PhPeople *people;
	float height = 0.25;

	// FIXME
	if (_lines.length() > 0)
	{
		people = _lines[0]->people();
	}
	else
	{
		people = new PhPeople();
	}

	PhStripLine *line = new PhStripLine(time, typeIn, people, y, height);
	append(line);
}

void PhStripLineModel::remove(int index)
{
	removeRow(index);
}
