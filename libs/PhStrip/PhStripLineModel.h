#ifndef PHSTRIPLINETMODEL_H
#define PHSTRIPLINETMODEL_H

#include <QListIterator>
#include <QAbstractListModel>
#include "PhStrip/PhStripLine.h"
#include "PhStrip/PhStripPeopleModel.h"

class PhStripLineModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum NextPeopleRoles {
		TrackNumberRole = Qt::UserRole + 1,
		TimeInRole,
		TypeInRole,
		TextsRole,
		UnlinkedDetectsRole,
		PeopleNameRole,
		DurationRole
	};

	PhStripLineModel(QObject *parent, PhStripPeopleModel *peopleModel);

	void append(PhStripLine *line);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	void clear();

	QListIterator<PhStripLine *> iterator();

	PhTime timeIn();
	PhTime timeOut();

signals:
	void timeInChanged();
	void timeOutChanged();

public slots:
	void add(PhTime timeIn, float y, PhPeople *people, PhStripDetect::PhDetectType typeIn);
	void peopleChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

protected:
	QHash<int, QByteArray> roleNames() const;

private slots:
	void updateTimeIn();
	void updateTimeOut();

private:
	void setTimeIn(const PhTime &timeIn);
	void setTimeOut(const PhTime &timeOut);

	QList<PhStripLine*> _lines;
	PhTime _timeIn;
	PhTime _timeOut;
	PhStripPeopleModel *_peopleModel;
};

#endif // PHSTRIPLINEMODEL_H
