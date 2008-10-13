#include "XUPFilteredProjectModel.h"
#include "XUPProjectItem.h"

#include <QDebug>

XUPFilteredProjectModel::XUPFilteredProjectModel( QObject* parent, XUPProjectModel* sourceModel )
	: QAbstractItemModel( parent )
{
	mSourceModel = 0;
	setSourceModel( sourceModel );
}

XUPFilteredProjectModel::~XUPFilteredProjectModel()
{
}

QModelIndex XUPFilteredProjectModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( mSourceModel && mSourceModel->mRootProject && parent == QModelIndex() && column < columnCount() && row < rowCount() )
	{
		return createIndex( row, column, mItems[ mSourceModel->mRootProject ].at( row ) );
	}
	
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
	return mSourceModel && mSourceModel->mRootProject ? mItems[ mSourceModel->mRootProject ].count() : 0;

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
	return mSourceModel ? mSourceModel->columnCount() : 0;
}

QVariant XUPFilteredProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	return mSourceModel ? mSourceModel->headerData( section, orientation, role ) : QVariant();
}

QVariant XUPFilteredProjectModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
		return QVariant();

	switch ( role )
	{
		case Qt::DecorationRole:
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		{
			XUPItem* item = mItems[ mSourceModel->mRootProject ].at( index.row() );
			
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
				return "Tooltip";
			}
		}
		default:
			break;
	}
	
	return QVariant();
}

Qt::ItemFlags XUPFilteredProjectModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

XUPItemList XUPFilteredProjectModel::getFilteredVariables( const XUPItem* root ) const
{
	XUPItemList variables;
	XUPProjectItem* rootProject = mSourceModel->mRootProject;
	
	for ( int i = 0; i < root->childCount(); i++ )
	{
		XUPItem* item = root->child( i );
		
		switch ( item->type() )
		{
			case XUPItem::Project:
			{
				/*
				XUPItem* pItem = item->parent();
				if ( pItem->type() == XUPItem::Function && pItem->attribute( "name" ).toLower() == "include" )
					variables << getFilteredVariables( item, variableName, callerItem );
				*/
				break;
			}
			case XUPItem::Comment:
				break;
			case XUPItem::EmptyLine:
				break;
			case XUPItem::Variable:
			{
				const QStringList filteredVariables = rootProject->projectInfos()->filteredVariables( rootProject->projectType() );
				if ( filteredVariables.contains( item->attribute( "name" ) ) )
					variables << item;
				break;
			}
			case XUPItem::Value:
				break;
			case XUPItem::Function:
			{
				variables << getFilteredVariables( item );
				break;
			}
			case XUPItem::Scope:
			{
				variables << getFilteredVariables( item );
				break;
			}
			default:
				break;
		}
	}
	
	return variables;
}

void XUPFilteredProjectModel::setSourceModel( XUPProjectModel* model )
{
	mSourceModel = model;
	
	reset();
	mItems.clear();
	
	if ( !mSourceModel )
		return;
	
	beginInsertColumns( QModelIndex(), 0, 0 );
	endInsertColumns();
	
	// populate tree...
	XUPItemList variables = getFilteredVariables( mSourceModel->mRootProject );
	mItems[ mSourceModel->mRootProject ] = variables;
	
	const int count = variables.count();
	if ( count > 0 )
	{
		beginInsertRows( QModelIndex(), 0, count -1 );
		endInsertRows();
	}
}

XUPProjectModel* XUPFilteredProjectModel::sourceModel() const
{
	return mSourceModel;
}
