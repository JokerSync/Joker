#ifndef PHSTRIPPEOPLEMODEL_H
#define PHSTRIPPEOPLEMODEL_H

#include <QAbstractListModel>
#include "PhStrip/PhPeople.h"

class PhStripPeopleModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum PeopleRoles {
	  NameRole = Qt::UserRole + 1,
  };

  PhStripPeopleModel(QObject * parent = 0);

  void append(PhPeople *people);
  int rowCount(const QModelIndex & parent = QModelIndex()) const;
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
  void clear();

  QListIterator<PhPeople *> iterator();  
  PhPeople *findByName(QString name);

  PhPeople *create(QString name);
public slots:
  void add(QString name);
  void remove(int index);
  int indexOf(QString name);

protected:
  QHash<int, QByteArray> roleNames() const;

private:
  QList<PhPeople*> _peoples;
};

#endif // PHSTRIPPEOPLEMODEL_H
