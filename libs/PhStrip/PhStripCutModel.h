#ifndef PHSTRIPCUTMODEL_H
#define PHSTRIPCUTMODEL_H

#include <QAbstractListModel>
#include "PhStrip/PhStripCut.h"

class PhStripCutModel : public QAbstractListModel
{
   Q_OBJECT
public:
   enum CutRoles {
	   TimeRole = Qt::UserRole + 1,
	   TypeRole
   };

   PhStripCutModel(QObject * parent = 0);

   void append(PhStripCut *cut);
   int rowCount(const QModelIndex & parent = QModelIndex()) const;
   QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role);
   bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
   void clear();

   QListIterator<PhStripCut *> iterator();

public slots:
   void add(PhTime time);
   void remove(int index);

protected:
   QHash<int, QByteArray> roleNames() const;

private:
   QList<PhStripCut*> _cuts;
};

#endif // PHSTRIPCUTMODEL_H
