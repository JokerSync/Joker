#include "PhTrackModel.h"

PhTrackModel::PhTrackModel(QObject *parent) :
	QAbstractListModel(parent)
{
}

void PhTrackModel::addTrack(const PhTrack *track)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_tracks << track;
	endInsertRows();
}

int PhTrackModel::rowCount(const QModelIndex & parent) const {
	return _tracks.count();
}

QVariant PhTrackModel::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= _tracks.count())
		return QVariant();
	const PhTrack *track = _tracks[index.row()];
	if (role == StripTextRole)
		return QVariant::fromValue<QObject*>(track->stripTextModel());
	else if (role == StripPeopleRole)
		return QVariant::fromValue<QObject*>(track->stripPeopleModel());
	else if (role == OffDetectRole)
		return QVariant::fromValue<QObject*>(track->offDetectModel());
	else if (role == SemiOffDetectRole)
		return QVariant::fromValue<QObject*>(track->semiOffDetectModel());
	else if (role == ArrowUpDetectRole)
		return QVariant::fromValue<QObject*>(track->arrowUpDetectModel());
	else if (role == ArrowDownDetectRole)
		return QVariant::fromValue<QObject*>(track->arrowDownDetectModel());
	return QVariant();
}

QHash<int, QByteArray> PhTrackModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[StripTextRole] = "stripText";
	roles[StripPeopleRole] = "stripPeople";
	roles[OffDetectRole] = "offDetect";
	roles[SemiOffDetectRole] = "semiOffDetect";
	roles[ArrowUpDetectRole] = "arrowUpDetect";
	roles[ArrowDownDetectRole] = "arrowDownDetect";
	return roles;
}

void PhTrackModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount());
	qDeleteAll(_tracks);
	_tracks.clear();
	endRemoveRows();
}
