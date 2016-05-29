#ifndef PHSTRIPLINETMODEL_H
#define PHSTRIPLINETMODEL_H

#include <QListIterator>
#include <QAbstractListModel>
#include "PhStrip/PhStripLine.h"

class PhStripLineModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum NextPeopleRoles {
		TrackNumberRole = Qt::UserRole + 1,
		TimeInRole,
		TextsRole,
		UnlinkedDetectsRole,
		PeopleNameRole
	};

	PhStripLineModel(QObject *parent = 0);

	void append(PhStripLine *line);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	void clear();

	QListIterator<PhStripLine *> iterator();

public slots:
	void addDetect(PhTime timeIn, float y);
	void remove(int index);

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<PhStripLine*> _lines;
};

#endif // PHSTRIPLINEMODEL_H
