#ifndef PHNEXTPEOPLEMODEL_H
#define PHNEXTPEOPLEMODEL_H

#include <QAbstractListModel>
#include "PhGraphicStrip/PhNextPeople.h"

class PhNextPeopleModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum NextPeopleRoles {
		NameRole = Qt::UserRole + 1,
		ColorRole,
		TimeInRole,
		SelectedRole,
		DurationRole
	};

	PhNextPeopleModel(QObject *parent = 0);

	void addNextPeople(const PhNextPeople *people);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void clear();

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<const PhNextPeople*> _nextPeoples;
};

#endif // PHNEXTPEOPLEMODEL_H
