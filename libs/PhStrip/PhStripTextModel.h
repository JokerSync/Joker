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
		DurationRole,
		TypeOutRole
	};

	PhStripTextModel(QObject * parent = 0);

	QList<PhStripText*> texts() {
		return _texts;
	}

	void append(PhStripText *stripText);
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	void clear();

	QListIterator<PhStripText *> iterator();

	PhTime duration() const;

signals:
	void durationChanged();

public slots:
	void addText(QString content, PhTime timeOut, PhStripDetect::PhDetectType typeOut);
	void removeText(int index);	
	void split(int index, int splitCharIndex, PhTime splitTime);
	void merge(int mergeIndex);

protected:
	QHash<int, QByteArray> roleNames() const;

private:
	void setDuration(PhTime duration);
	void updateDuration();

	QList<PhStripText*> _texts;
	PhTime _duration;
};

#endif // PHSTRIPTEXTMODEL_H