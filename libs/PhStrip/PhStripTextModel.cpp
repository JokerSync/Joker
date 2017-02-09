#include "PhStripTextModel.h"

PhStripTextModel::PhStripTextModel(QObject *parent) :
	QAbstractListModel(parent),
	_duration(0)
{
}

void PhStripTextModel::append(PhStripText *text)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_texts << text;
	endInsertRows();

	updateDuration();
}

int PhStripTextModel::rowCount(const QModelIndex & parent) const {
	return _texts.count();
}

QVariant PhStripTextModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _texts.count())
		return QVariant();

	PhStripText *stripText = _texts[index.row()];

	if (role == ContentRole)
		return stripText->content();
	else if (role == DurationRole)
		return stripText->duration();
	else if (role == TypeOutRole)
		return stripText->typeOut();
	return QVariant();
}

bool PhStripTextModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.row() < 0 || index.row() >= _texts.count())
		return false;

	PhStripText *stripText = _texts[index.row()];

	if (role == ContentRole) {
		if (stripText->content() != value.toString()) {
			stripText->setContent(value.toString());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}
	else if (role == DurationRole) {
		if (stripText->duration() != value.toInt()) {
			stripText->setDuration(value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
			updateDuration();
		}
		return true;
	}
	else if (role == TypeOutRole) {
		if (stripText->typeOut() != value.toInt()) {
			stripText->setTypeOut((PhStripDetect::PhDetectType)value.toInt());
			emit dataChanged(index, index, QVector<int>(1, role));
		}
		return true;
	}

	return false;
}

bool PhStripTextModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (count > 0) {
		beginRemoveRows(parent, row, row + count - 1);
		for(int i=0; i<count; i++) {
			PhStripText *text = _texts.takeAt(row);
			delete text;
		}
		endRemoveRows();

		updateDuration();
	}
}

QHash<int, QByteArray> PhStripTextModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ContentRole] = "content";
	roles[DurationRole] = "duration";
	roles[TypeOutRole] = "typeOut";
	return roles;
}

void PhStripTextModel::setDuration(PhTime duration)
{
	if (duration != _duration) {
		_duration = duration;
		emit durationChanged();
	}
}

void PhStripTextModel::updateDuration()
{
	PhTime duration = 0;
	foreach(PhStripText *text, _texts) {
		duration += text->duration();
	}
	setDuration(duration);
}

void PhStripTextModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_texts);
	_texts.clear();
	endRemoveRows();

	setDuration(0);
}

QListIterator<PhStripText *> PhStripTextModel::iterator()
{
	return QListIterator<PhStripText *>(_texts);
}

PhTime PhStripTextModel::duration() const
{
	return _duration;
}

void PhStripTextModel::addText(QString content, PhTime timeOut, PhStripDetect::PhDetectType typeOut)
{
	append(new PhStripText(content, timeOut, typeOut));
}

void PhStripTextModel::split(int splitIndex, int splitCharIndex, PhTime splitTime, PhStripDetect::PhDetectType splitType)
{
	PhStripText *text = _texts[splitIndex];
	QString splitContent = text->content().mid(splitCharIndex);
	PhTime splitDuration = text->duration() - splitTime;

	// add a new text with the split content
	beginInsertRows(QModelIndex(), splitIndex, splitIndex);
	_texts.insert(splitIndex, new PhStripText(text->content().left(splitCharIndex), splitTime, splitType));
	endInsertRows();

	// update the current text
	setData(index(splitIndex+1), splitContent, ContentRole);
	setData(index(splitIndex+1), splitDuration, DurationRole);

	updateDuration();
}

void PhStripTextModel::merge(int mergeIndex)
{
	PhStripText *text = _texts[mergeIndex];
	PhStripText *nextText = _texts[mergeIndex+1];

	setData(index(mergeIndex), text->content() + nextText->content(), ContentRole);
	setData(index(mergeIndex), text->duration() + nextText->duration(), DurationRole);
	setData(index(mergeIndex), nextText->typeOut(), TypeOutRole);

	removeRow(mergeIndex + 1);
}

void PhStripTextModel::removeText(int index)
{
	removeRow(index);
}
