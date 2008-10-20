#include "XUPFilteredProjectModel.h"
#include "XUPProjectItem.h"

#include <QDebug>

void debug( XUPItem* root, const QMap<XUPItem*, XUPItemList>& mItems )
{
	static int prof = 0;
	QString prep = QString().fill( ' ', prof );
	qWarning( root->displayText().prepend( prep ).toLocal8Bit().constData() );
	foreach ( XUPItem* item, mItems.value( root ) )
	{
		prof += 4;
		debug( item, mItems );
		prof -= 4;
	}
}

bool xupItemLessThan( const XUPItem* left, const XUPItem* right )
{
	return left->operator<( *right );
}

void qSortItems( XUPItemList& items )
{
	qSort( items.begin(), items.end(), xupItemLessThan );
}

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
/*
	if ( mSourceModel && mSourceModel->mRootProject && column < columnCount( parent ) && row < rowCount( parent ) )
	{
		XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
		if ( !parentItem )
			parentItem = mSourceModel->mRootProject;
		return createIndex( row, column, mItems[ parentItem ].at( row ) );
	}
*/
	return QModelIndex();
}

QModelIndex XUPFilteredProjectModel::parent( const QModelIndex& index ) const
{
	return QModelIndex();
/*
	if ( !index.isValid() )
		return QModelIndex();
		
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
	XUPItem* parentItem = 0;
	foreach ( XUPItem* parent, mItems.keys() )
	{
		if ( mItems[ parent ].contains( item ) )
		{
			parentItem = parent;
			break;
		}
	}
	
	if ( !parentItem || parentItem == mSourceModel->mRootProject )
		return QModelIndex();
	
	int row = 0;
	foreach ( XUPItem* parent, mItems.keys() )
	{
		if ( mItems[ parent ].contains( parentItem ) )
		{
			row = mItems[ parent ].indexOf( parentItem );
			break;
		}
	}
	
	return createIndex( row, 0, parentItem );
*/
}

int XUPFilteredProjectModel::rowCount( const QModelIndex& parent ) const
{
	return 0;
/*
	if ( parent.column() > 0 || !mSourceModel || !mSourceModel->mRootProject )
		return 0;
		
	XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
	if ( !parentItem )
	{
		parentItem = mSourceModel->mRootProject;
	}
	
	return mItems[ parentItem ].count();
*/
}

int XUPFilteredProjectModel::columnCount( const QModelIndex& parent ) const
{
	return 0;
/*
	Q_UNUSED( parent );
	return mSourceModel ? mSourceModel->columnCount() : 0;
*/
}

QVariant XUPFilteredProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	return mSourceModel ? mSourceModel->headerData( section, orientation, role ) : QVariant();
}

QVariant XUPFilteredProjectModel::data( const QModelIndex& index, int role ) const
{
	return QVariant();
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
*/
}

Qt::ItemFlags XUPFilteredProjectModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

XUPItemMappingIterator XUPFilteredProjectModel::createMapping( XUPItem* item, bool sort ) const
{
	XUPItemMappingIterator it = mItemsMapping.constFind( item );
	if ( it != mItemsMapping.constEnd() ) // was mapped already
		return it;

	Mapping* m = new Mapping;
	
	/*
	int count = item->childCount();
	for ( int i = 0; i < count; ++i )
	{
		m->mMappedChildren << item->child( i );
	}
	
	if ( sort )
	{
		qSortItems( m->mMappedChildren );
	}
	*/

	it = XUPItemMappingIterator( mItemsMapping.insert( item, m ) );
	m->mIterator = it;
/*
	if ( item != mSourceModel->mRootProject )
	{
		XUPItem* parentItem = item->project()->parentProject();
		XUPItemMappingIterator it2 = createMapping( parentItem );
		Q_ASSERT( it2 != mItemsMapping.constEnd() );
		it2.value()->mMappedChildren << item;
	}
*/
	Q_ASSERT( it != mItemsMapping.constEnd() );
	Q_ASSERT( it.value() );

	return it;
}

void XUPFilteredProjectModel::setSourceModel( XUPProjectModel* model )
{
	mSourceModel = model;
	
	reset();
	mItemsMapping.clear();
	
	if ( !mSourceModel )
		return;
	
	// header
	beginInsertColumns( QModelIndex(), 0, 0 );
	endInsertColumns();
	
	// tree items
	populateFromItem( mSourceModel->mRootProject );
	
	// debug
	//debug( mSourceModel->mRootProject, mItems );
}

XUPProjectModel* XUPFilteredProjectModel::sourceModel() const
{
	return mSourceModel;
}

XUPItemList XUPFilteredProjectModel::getFilteredVariables( const XUPItem* root ) const
{
	XUPItemList variables;
	XUPProjectItem* rootProject = mSourceModel->mRootProject;
	const QStringList filteredVariables = rootProject->projectInfos()->filteredVariables( rootProject->projectType() );
	
	for ( int i = 0; i < root->childCount(); i++ )
	{
		XUPItem* child = root->child( i );
		
		switch ( child->type() )
		{
			case XUPItem::Project:
			{
				/*
				XUPItem* pItem = item->parent();
				if ( pItem->type() == XUPItem::Function && pItem->attribute( "name" ).toLower() == "include" )
				*/
					//variables << getFilteredVariables( child );
				break;
			}
			case XUPItem::Comment:
				break;
			case XUPItem::EmptyLine:
				break;
			case XUPItem::Variable:
			{
				if ( filteredVariables.contains( child->attribute( "name" ) ) )
				{
					variables << child;
				}
				variables << getFilteredVariables( child );
				break;
			}
			case XUPItem::Value:
				break;
			case XUPItem::Function:
			{
				variables << getFilteredVariables( child );
				break;
			}
			case XUPItem::Scope:
			{
				variables << getFilteredVariables( child );
				break;
			}
			default:
				break;
		}
	}
	
	return variables;
}

XUPItemList XUPFilteredProjectModel::getValues( const XUPItem* root ) const
{
	XUPItemList values;
	for ( int i = 0; i < root->childCount(); i++ )
	{
		XUPItem* child = root->child( i );
		switch ( child->type() )
		{
			case XUPItem::Value:
			case XUPItem::File:
			case XUPItem::Path:
				values << child;
				break;
			case XUPItem::Folder:
				values << getValues( child );
			default:
				break;
		}
	}
	return values;
}

void XUPFilteredProjectModel::populateFromItem( XUPItem* item )
{
	XUPProjectItem* project = item->project();
	XUPItemMappingIterator projectIterator = createMapping( project, false );
	
	XUPItemList variables = getFilteredVariables( item );
	foreach ( XUPItem* variable, variables )
	{
		XUPItem* tmp;
		tmp = projectIterator.value()->findVariable( variable->attribute( "name" ) );
		if ( tmp )
		{
			variable = tmp;
		}
		else
		{
			projectIterator.value()->mMappedChildren << variable;
		}
		
		XUPItemMappingIterator variableIterator = createMapping( variable, false );
		XUPItemList& variableValues = variableIterator.value()->mMappedChildren;
		foreach ( XUPItem* value, getValues( variable ) )
		{
			if ( !variableIterator.value()->findValue( value->attribute( "content" ) ) )
			{
				variableValues << value;
			}
		}
		
		qSortItems( variableValues );
	}
	
	XUPItemList& projectVariables = projectIterator.value()->mMappedChildren;
	qSortItems( projectVariables );
	int count = projectVariables.count();
	if ( count > 0 )
	{
		beginInsertRows( QModelIndex(), 0, count -1 );
		endInsertRows();
	}
}
