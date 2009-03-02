#ifndef VARIABLESMODEL_H
#define VARIABLESMODEL_H

#include <QAbstractItemModel>

#include "VariablesModelItem.h"
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
	QGdb::VariablesModelItem mArgumentsRoot;
	QGdb::VariablesModelItem mLocalsRoot;
	QGdb::VariablesModelItem mWatchesRoot;
	
	QGdb::Driver* mDriver;
	
	/* Read information from src item and update dst item according with it.
	   Usage: dst contains current variables. Debuger made step and variables updated.
	   New variables should be readed to src, than we should update dst.
	   After update changed variables will be highlighted.
	   Method is recursive
	 */
	void mergeItems (QGdb::VariablesModelItem* dst, QGdb::VariablesModelItem* src);
	
protected slots:
	void localsUpdated ();
	void argumentsUpdated ();
	
signals:
	void expand (QModelIndex index); // signal for QTreeView
};

#endif // VARIABLESMODEL_H
