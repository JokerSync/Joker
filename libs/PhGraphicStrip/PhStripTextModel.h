#ifndef PHSTRIPTEXTMODEL_H
#define PHSTRIPTEXTMODEL_H

#include <QAbstractListModel>
#include "PhStrip/PhStripText.h"

class PhStripTextModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum NextPeopleRoles {
		ContentRole = Qt::UserRole + 1,
		//ColorRole,
		TimeInRole,
		SelectedRole,
		TimeOutRole
	};

	PhStripTextModel(QObject *parent = 0);

	void addStripText(const PhStripText *stripText);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void clear();

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<const PhStripText*> _stripTexts;
};

#endif // PHSTRIPTEXTMODEL_H
