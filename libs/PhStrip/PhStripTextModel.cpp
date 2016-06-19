#include "PhStripTextModel.h"

PhStripTextModel::PhStripTextModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhStripTextModel::append(PhStripText *text)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_texts << text;
	endInsertRows();
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
	beginRemoveRows(parent, row, row + count - 1);
	PhStripText *text = _texts.takeAt(row);
	delete text;
	endRemoveRows();
}

QHash<int, QByteArray> PhStripTextModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ContentRole] = "content";
	roles[DurationRole] = "duration";
	roles[TypeOutRole] = "typeOut";
	return roles;
}

void PhStripTextModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_texts);
	_texts.clear();
	endRemoveRows();
}

QListIterator<PhStripText *> PhStripTextModel::iterator()
{
	return QListIterator<PhStripText *>(_texts);
}

void PhStripTextModel::addText(QString content, PhTime timeOut, PhStripDetect::PhDetectType typeOut)
{
	append(new PhStripText(content, timeOut, typeOut));
}

void PhStripTextModel::split(int splitIndex, int splitCharIndex, PhTime splitTime)
{
	PhStripText *text = _texts[splitIndex];
	QString splitContent = text->content().mid(splitCharIndex);
	PhTime splitDuration = text->duration() - splitTime;

	// update the current text
	setData(index(splitIndex), text->content().left(splitCharIndex), ContentRole);
	setData(index(splitIndex), splitTime, DurationRole);

	// add a new text with the split content
	beginInsertRows(QModelIndex(), splitIndex+1, splitIndex+1);
	_texts.insert(splitIndex+1, new PhStripText(splitContent, splitDuration, PhStripDetect::Labial));
	endInsertRows();
}

void PhStripTextModel::removeText(int index)
{
	removeRow(index);
}
