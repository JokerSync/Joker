#ifndef PHSTRIPLOOPMODEL_H
#define PHSTRIPLOOPMODEL_H

#include <QAbstractListModel>
#include "PhStrip/PhStripLoop.h"

class PhStripLoopModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum LoopRoles {
		TimeRole = Qt::UserRole + 1,
		LabelRole
	};

	PhStripLoopModel(QObject * parent = 0);

	void append(PhStripLoop *cut);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	void clear();

	QListIterator<PhStripLoop *> iterator();

public slots:
	void add(PhTime time, QString label = "");
	void remove(int index);

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<PhStripLoop*> _loops;
};

#endif // PHSTRIPLOOPMODEL_H
