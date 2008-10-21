#ifndef XUPFILTEREDPROJECTMODEL_H
#define XUPFILTEREDPROJECTMODEL_H

#include "XUPProjectModel.h"
#include "XUPItem.h"

#include <QMap>

typedef QMap<XUPItem*, class Mapping*> XUPItemMapping;
typedef XUPItemMapping::const_iterator XUPItemMappingIterator;

struct Mapping
{
	Mapping()
	{
		mParent = 0;
	}
	
	QModelIndex mProxyIndex;
	XUPItem* mParent;
	QList<XUPItem*> mMappedChildren;
	XUPItemMappingIterator mIterator;
	
	XUPItem* findVariable( const QString& name ) const
	{
		foreach ( XUPItem* item, mMappedChildren )
		{
			if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == name )
			{
				return item;
			}
		}
		return 0;
	}
	
	XUPItem* findValue( const QString& content ) const
	{
		foreach ( XUPItem* item, mMappedChildren )
		{
			switch ( item->type() )
			{
				case XUPItem::Value:
				case XUPItem::File:
				case XUPItem::Path:
					if ( item->attribute( "content" ) == content )
					{
						return item;
					}
					break;
				default:
					break;
			}
		}
		return 0;
	}
};

class XUPFilteredProjectModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	XUPFilteredProjectModel( QObject* parent = 0, XUPProjectModel* sourceModel = 0 );
	virtual ~XUPFilteredProjectModel();

	// QAbstractItemModel reimplementation
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	
	XUPItemMappingIterator indexToIterator( const QModelIndex& proxyIndex ) const;
	XUPItem* mapToSource( const QModelIndex& proxyIndex ) const;
	QModelIndex mapFromSource( XUPItem* sourceItem ) const;
	
	void setSourceModel( XUPProjectModel* model );
	XUPProjectModel* sourceModel() const;
	
	XUPItemList getFilteredVariables( const XUPItem* root );
	XUPItemList getValues( const XUPItem* root );
	
	void populateProject( XUPProjectItem* item, bool updateView );

protected:
	XUPProjectModel* mSourceModel;
	mutable XUPItemMapping mItemsMapping;
	
	XUPItemMappingIterator createMapping( XUPItem* item, XUPItem* parent = 0 ) const;
};

#endif // XUPFILTEREDPROJECTMODEL_H
