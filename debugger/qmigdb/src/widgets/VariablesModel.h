#ifndef VARIABLESMODEL_H
#define VARIABLESMODEL_H

#include <QStandardItem>
#include <QAbstractItemModel>

#include "QGdbDriver.h"

class VariablesModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	VariablesModel (QGdb::Driver* driver);
	virtual ~VariablesModel ();
	
	// QAbstractItemModel standard item set
	QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex parent ( const QModelIndex & index ) const;
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	
protected:
	QStandardItem mArgumentsRoot;
	QStandardItem mLocalsRoot;
	QStandardItem mWatchesRoot;
	
	QGdb::Driver* mDriver;
	
protected slots:
	void localsUpdated ();
	void argumentsUpdated ();
	
signals:
	void expand (QModelIndex index); // signal for QTreeView
};

#endif // VARIABLESMODEL_H
