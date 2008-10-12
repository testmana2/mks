#ifndef XUPFILTEREDPROJECTMODEL_H
#define XUPFILTEREDPROJECTMODEL_H

#include "XUPProjectModel.h"

class XUPFilteredProjectModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	XUPFilteredProjectModel( XUPProjectModel* sourceModel = 0 );
	virtual ~XUPFilteredProjectModel();

	// QAbstractItemModel reimplementation
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	
	void setSourceModel( XUPProjectModel* model );
	XUPProjectModel* sourceModel() const;
	

protected:
	XUPProjectModel* mSourceModel;
};

#endif // XUPFILTEREDPROJECTMODEL_H
