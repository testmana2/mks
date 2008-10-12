#include "XUPFilteredProjectModel.h"

#include <QDebug>

XUPFilteredProjectModel::XUPFilteredProjectModel( XUPProjectModel* sourceModel )
	: QAbstractItemModel( sourceModel )
{
	setSourceModel( sourceModel );
}

XUPFilteredProjectModel::~XUPFilteredProjectModel()
{
}

QModelIndex XUPFilteredProjectModel::index( int row, int column, const QModelIndex& parent ) const
{
	/*
	if ( !hasIndex( row, column, parent ) )
		return QModelIndex();
	
	XUPItem* parentItem;
	if ( !parent.isValid() )
		parentItem = mRootProject;
	else
		parentItem = static_cast<XUPItem*>( parent.internalPointer() );
	
	XUPItem* childItem = parentItem->child( row );
	if ( childItem )
		return createIndex( row, column, childItem );
	*/
	return QModelIndex();
}

QModelIndex XUPFilteredProjectModel::parent( const QModelIndex& index ) const
{
	//if ( !index.isValid() )
		return QModelIndex();
	/*
	XUPItem* childItem = static_cast<XUPItem*>( index.internalPointer() );
	XUPItem* parentItem = childItem->parent();

	if ( !parentItem || parentItem == mRootProject )
		return QModelIndex();

	return createIndex( parentItem->row(), 0, parentItem );
	*/
}

int XUPFilteredProjectModel::rowCount( const QModelIndex& parent ) const
{
	//if ( parent.column() > 0 )
		return 0;
	/*
	XUPItem* parentItem;
	if ( !parent.isValid() )
		parentItem = mRootProject;
	else
		parentItem = static_cast<XUPItem*>( parent.internalPointer() );

#warning handle include ans custom row count, should be call directly by XUPItem::childCount();
	XUPProjectItem* project = parentItem->project();
	project->handleIncludeItem( parentItem );
	project->customRowCount( parentItem );
	
	return parentItem->childCount();
	*/
}

int XUPFilteredProjectModel::columnCount( const QModelIndex& parent ) const
{
	/*
	Q_UNUSED( parent );
	return 1;
	*/
	return 0;
}

QVariant XUPFilteredProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	/*
	if ( orientation == Qt::Horizontal && section == 0 )
	{
		if ( mRootProject )
		{
			if ( role == Qt::DecorationRole )
				return mRootProject->displayIcon();
			else if ( role == Qt::DisplayRole )
				return mRootProject->displayText();
		}
	}
	*/
	return QVariant();
}

QVariant XUPFilteredProjectModel::data( const QModelIndex& index, int role ) const
{
	/*
	if ( !index.isValid() )
		return QVariant();

	switch ( role )
	{
		case Qt::DecorationRole:
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		{
			XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );

			QDomNode node = item->node();
			QStringList attributes;
			QDomNamedNodeMap attributeMap = node.attributes();
			
			if ( role == Qt::DecorationRole )
			{
				return item->displayIcon();
			}
			else if ( role == Qt::DisplayRole )
			{
				return item->displayText();
			}
			else if ( role == Qt::ToolTipRole )
			{
				for ( int i = 0; i < attributeMap.count(); i++ )
				{
					QDomNode attribute = attributeMap.item( i );
					attributes << attribute.nodeName() +"=\"" +attribute.nodeValue() +"\"";
				}
				return attributes.join( "\n" );
			}
		}
		default:
			break;
	}
	*/
	return QVariant();
}

Qt::ItemFlags XUPFilteredProjectModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void XUPFilteredProjectModel::setSourceModel( XUPProjectModel* model )
{
	mSourceModel = model;
}

XUPProjectModel* XUPFilteredProjectModel::sourceModel() const
{
	return mSourceModel;
}
