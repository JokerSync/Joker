#include "PhStripLineModel.h"
#include "PhStripPeopleModel.h"

#include "PhTools/PhDebug.h"

PhStripLineModel::PhStripLineModel(QObject *parent) :
	QAbstractListModel(parent),
	_timeIn(PHTIMEMAX),
	_timeOut(PHTIMEMIN)
{
}

void PhStripLineModel::append(PhStripLine *line)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_lines << line;
	endInsertRows();

	if (line->timeIn() < _timeIn) {
		setTimeIn(line->timeIn());
	}

	if (line->timeOut() > _timeOut) {
		setTimeOut(line->timeOut());
	}

	connect(line, &PhStripLine::timeOutChanged, this, &PhStripLineModel::updateTimeOut);
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
	else if (role == TypeInRole)
		return line->typeIn();
	else if (role == TextsRole)
		return QVariant::fromValue<QObject*>(line->textModel());
	else if (role == UnlinkedDetectsRole)
		return QVariant::fromValue<QObject*>(line->detectModel());
	else if (role == PeopleNameRole)
		return line->people()->name();
	else if (role == DurationRole)
		return line->duration();
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
			updateTimeIn();
			updateTimeOut();
		}
		return true;
	}
	else if (role == TypeInRole) {
		if (line->typeIn() != value.toInt()) {
			line->setTypeIn((PhStripDetect::PhDetectType)value.toInt());
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

void PhStripLineModel::assignLineToPeople(int row, PhPeople *people)
{
	QModelIndex index = this->index(row);

	if (index.row() < 0 || index.row() >= _lines.count())
		return;

	PhStripLine *line = _lines[index.row()];

	if (line->people() != people) {
		line->setPeople(people);
		emit dataChanged(index, index, QVector<int>(1, PeopleNameRole));
	}
}

PhTime PhStripLineModel::timeIn()
{
	return _timeIn;
}

PhTime PhStripLineModel::timeOut()
{
	return _timeOut;
}

bool PhStripLineModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (count > 0) {
		beginRemoveRows(parent, row, row + count - 1);
		for(int i=0; i<count; i++) {
			PhStripLine *line = _lines.takeAt(row);
			delete line;
		}
		endRemoveRows();

		updateTimeIn();
		updateTimeOut();

		return true;
	}

	return false;
}

QHash<int, QByteArray> PhStripLineModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[TrackNumberRole] = "trackNumber";
	roles[TimeInRole] = "timeIn";
	roles[TypeInRole] = "typeIn";
	roles[TextsRole] = "texts";
	roles[UnlinkedDetectsRole] = "unlinkedDetects";
	roles[PeopleNameRole] = "peopleName";
	roles[DurationRole] = "duration";
	return roles;
}

void PhStripLineModel::setTimeIn(const PhTime &timeIn)
{
	if (_timeIn != timeIn) {
		_timeIn = timeIn;
		emit timeInChanged();
	}
}

void PhStripLineModel::setTimeOut(const PhTime &timeOut)
{
	if (_timeOut != timeOut) {
		_timeOut = timeOut;
		emit timeOutChanged();
	}
}

void PhStripLineModel::updateTimeIn()
{
	PhTime timeIn = PHTIMEMAX;
	foreach(PhStripLine *line, _lines) {
		if (line->timeIn() < timeIn) {
			timeIn = line->timeIn();
		}
	}

	setTimeIn(timeIn);
}

void PhStripLineModel::updateTimeOut()
{
	QModelIndex index = this->index(_lines.indexOf((PhStripLine*)sender()));
	// index might not be valid if this is a line deletion
	if (index.isValid()) {
		emit dataChanged(index, index, QVector<int>(1, DurationRole));
	}

	PhTime timeOut = PHTIMEMIN;
	foreach(PhStripLine *line, _lines) {
		if (line->timeOut() > timeOut) {
			timeOut = line->timeOut();
		}
	}

	setTimeOut(timeOut);
}

void PhStripLineModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_lines);
	_lines.clear();
	endRemoveRows();

	setTimeIn(PHTIMEMAX);
	setTimeOut(PHTIMEMIN);
}

QListIterator<PhStripLine *> PhStripLineModel::iterator()
{
	return QListIterator<PhStripLine *>(_lines);
}

void PhStripLineModel::add(PhTime time, float y, PhPeople *people, PhStripDetect::PhDetectType typeIn)
{
	float height = 0.25;
	PhStripLine *line = new PhStripLine(time, typeIn, people, y, height);
	append(line);
}

void PhStripLineModel::remove(int index)
{
	removeRow(index);
}

void PhStripLineModel::peopleChanged(const QModelIndex &topLeftPeople, const QModelIndex &bottomRightPeople, const QVector<int> &roles)
{
	if (!roles.contains(PhStripPeopleModel::NameRole)) {
		return;
	}

	PHDEBUG << "refreshing names";
	// one of the name has changed! emit signals to refresh all names
	QModelIndex topLeft = this->index(0);
	QModelIndex bottomRight = this->index(_lines.count()-1);
	emit dataChanged(topLeft, bottomRight, QVector<int>(1, PeopleNameRole));
}
