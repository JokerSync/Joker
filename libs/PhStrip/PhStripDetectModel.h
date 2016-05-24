#ifndef PHSTRIPDETECTMODEL_H
#define PHSTRIPDETECTMODEL_H

#include <QAbstractListModel>
#include "PhStrip/PhStripDetect.h"

class PhStripDetectModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum DetectRoles {
		TimeRole = Qt::UserRole + 1
	};

	PhStripDetectModel(QObject * parent = 0);

	void append(PhStripDetect *detect);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

public slots:
	void add(PhTime time);
	void remove(int index);

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	QList<PhStripDetect*> _detects;
};

#endif // PHSTRIPDETECTMODEL_H
