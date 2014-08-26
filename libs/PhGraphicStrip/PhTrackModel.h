#ifndef PHTRACKMODEL_H
#define PHTRACKMODEL_H

#include <QAbstractListModel>
#include "PhGraphicStrip/PhTrack.h"

class PhTrackModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum TrackRoles {
		StripTextRole = Qt::UserRole + 1,
		StripPeopleRole,
		OffDetectRole,
		SemiOffDetectRole,
		ArrowUpDetectRole,
		ArrowDownDetectRole
	};

	PhTrackModel(QObject *parent = 0);

	const PhTrack *track(int index) {
		return _tracks[index];
	}

	void addTrack(const PhTrack *track);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void clear();

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<const PhTrack*> _tracks;
};

#endif // PHTRACKMODEL_H
