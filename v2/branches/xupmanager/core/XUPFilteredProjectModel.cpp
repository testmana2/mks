#include "XUPFilteredProjectModel.h"
#include "XUPProjectItem.h"

#include <QDebug>

void debug( XUPItem* root, const XUPItemMapping& mItems, int mode = 0 )
{
	if ( mode == 0 )
	{
		static int prof = 0;
		QString prep = QString().fill( ' ', prof );
		qWarning( root->displayText().prepend( prep ).toLocal8Bit().constData() );
		XUPItemMappingIterator it = mItems.constFind( root );
		Q_ASSERT( it != mItems.constEnd() );
		Q_ASSERT( it.value() );
		foreach ( XUPItem* item, it.value()->mMappedChildren )
		{
			prof += 4;
			debug( item, mItems );
			prof -= 4;
		}
	}
	else
	{
		foreach ( XUPItem* item, mItems.keys() )
		{
			qWarning() << "Mapped" << item->displayText();
			foreach ( XUPItem* it, mItems.constFind( item ).value()->mMappedChildren )
			{
				qWarning() << "\tChild" << it->displayText();
			}
		}
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

QModelIndex XUPFilteredProjectModel::index( int row, int column, const QModelIndex& parentProxy ) const
{
	if ( !mSourceModel || column > 0 )
		return QModelIndex();
	
	XUPItem* item = parentProxy.isValid() ? mapToSource( parentProxy ) : mSourceModel->mRootProject;
	XUPItemMappingIterator it = mItemsMapping.constFind( item );
	
	if ( it != mItemsMapping.constEnd() )
	{
		XUPItem* item = it.value()->mMappedChildren.value( row );
		if ( item )
		{
			it = mItemsMapping.constFind( item );
			if ( it != mItemsMapping.constEnd() )
			{
				QModelIndex index = createIndex( row, column, *it );
				it.value()->mProxyIndex = index;
				return index;
			}
		}
	}
	
	return QModelIndex();
}

QModelIndex XUPFilteredProjectModel::parent( const QModelIndex& proxyIndex ) const
{
	XUPItem* item = proxyIndex.isValid() ? mapToSource( proxyIndex ) : mSourceModel->mRootProject;
	XUPItemMappingIterator it = mItemsMapping.constFind( item );

	if ( it != mItemsMapping.constEnd() )
	{
		XUPItem* parentItem = it.value()->mParent;
		it = mItemsMapping.constFind( parentItem );
		if ( it != mItemsMapping.constEnd() )
		{
			return it.value()->mProxyIndex;
		}
	}

	return QModelIndex();
}

int XUPFilteredProjectModel::rowCount( const QModelIndex& proxyParent ) const
{
	if ( mSourceModel )
	{
		XUPItem* item = proxyParent.isValid() ? mapToSource( proxyParent ) : mSourceModel->mRootProject;
		XUPItemMappingIterator it = mItemsMapping.constFind( item );
		
		if ( it != mItemsMapping.constEnd() )
		{
			return it.value()->mMappedChildren.count();
		}
	}
	
	return 0;
}

int XUPFilteredProjectModel::columnCount( const QModelIndex& proxyParent ) const
{
	Q_UNUSED( proxyParent );
	return mSourceModel ? 1 : 0;
}

QVariant XUPFilteredProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	return mSourceModel ? mSourceModel->headerData( section, orientation, role ) : QVariant();
}

QVariant XUPFilteredProjectModel::data( const QModelIndex& proxyIndex, int role ) const
{
	if ( !proxyIndex.isValid() )
	{
		return QVariant();
	}

	switch ( role )
	{
		case Qt::DecorationRole:
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		{
			XUPItem* item = mapToSource( proxyIndex );
			
			Q_ASSERT( item );
			
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

Qt::ItemFlags XUPFilteredProjectModel::flags( const QModelIndex& proxyIndex ) const
{
	if ( !proxyIndex.isValid() )
	{
		return 0;
	}
	
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// MAPPING

XUPItemMappingIterator XUPFilteredProjectModel::indexToIterator( const QModelIndex& proxyIndex ) const
{
	Q_ASSERT( proxyIndex.isValid() );
	const void* p = proxyIndex.internalPointer();
	Q_ASSERT( p );
	XUPItemMappingIterator it = static_cast<const Mapping*>( p )->mIterator;
	Q_ASSERT( it != mItemsMapping.constEnd() );
	Q_ASSERT( it.value() );
	return it;
}

XUPItem* XUPFilteredProjectModel::mapToSource( const QModelIndex& proxyIndex ) const
{
	if ( proxyIndex.isValid() )
	{
		XUPItemMappingIterator it = indexToIterator( proxyIndex );
		if ( it != mItemsMapping.constEnd() )
			return it.key();
	}
	return 0;
}

QModelIndex XUPFilteredProjectModel::mapFromSource( XUPItem* sourceItem ) const
{
	XUPItemMappingIterator it = mItemsMapping.constFind( sourceItem );
	if ( it != mItemsMapping.constEnd() )
		return it.value()->mProxyIndex;
	return QModelIndex();
}

XUPItemMappingIterator XUPFilteredProjectModel::createMapping( XUPItem* item, XUPItem* parent ) const
{
	XUPItemMappingIterator it = mItemsMapping.constFind( item );
	if ( it != mItemsMapping.constEnd() ) // was mapped already
		return it;

	Mapping* m = new Mapping;
	it = XUPItemMappingIterator( mItemsMapping.insert( item, m ) );
	m->mParent = parent;
	m->mIterator = it;

	if ( item != mSourceModel->mRootProject )
	{
		Q_ASSERT( parent );
		XUPItemMappingIterator parentIt = createMapping( parent );
		Q_ASSERT( parentIt != mItemsMapping.constEnd() );
		parentIt.value()->mMappedChildren << item;
	}

	Q_ASSERT( it != mItemsMapping.constEnd() );
	Q_ASSERT( it.value() );

	return it;
}

void XUPFilteredProjectModel::removeMapping( XUPItem* item )
{
	if ( Mapping* m = mItemsMapping.take( item ) )
	{
		for ( int i = 0; i < m->mMappedChildren.size(); ++i )
		{
			removeMapping( m->mMappedChildren.at( i ) );
		}
		
		if ( item != mSourceModel->mRootProject )
		{
			XUPItemMappingIterator parentIt = mItemsMapping.constFind( m->mParent );
			if ( parentIt != mItemsMapping.constEnd() )
			{
				parentIt.value()->mMappedChildren.removeAll( item );
			}
		}
		
		delete m;
	}
}

void XUPFilteredProjectModel::clearMapping()
{
	qDeleteAll( mItemsMapping );
	mItemsMapping.clear();
}

void XUPFilteredProjectModel::setSourceModel( XUPProjectModel* model )
{
	if ( mSourceModel )
	{
		disconnect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
	}
	
	mSourceModel = model;
	
	if ( mSourceModel )
	{
		clearMapping();
		
		connect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
		
		reset();
		
		// header
		beginInsertColumns( QModelIndex(), 0, 0 );
		endInsertColumns();
		
		// tree items
		populateProject( mSourceModel->mRootProject, true );
	}
}

XUPProjectModel* XUPFilteredProjectModel::sourceModel() const
{
	return mSourceModel;
}

XUPItemList XUPFilteredProjectModel::getFilteredVariables( const XUPItem* root )
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
				populateProject( child->project(), false );
				break;
			case XUPItem::Comment:
				break;
			case XUPItem::EmptyLine:
				break;
			case XUPItem::Variable:
				if ( filteredVariables.contains( child->attribute( "name" ) ) )
				{
					variables << child;
				}
				variables << getFilteredVariables( child );
				break;
			case XUPItem::Value:
				break;
			case XUPItem::Function:
				variables << getFilteredVariables( child );
				break;
			case XUPItem::Scope:
				variables << getFilteredVariables( child );
				break;
			default:
				break;
		}
	}
	
	return variables;
}

XUPItemList XUPFilteredProjectModel::getValues( const XUPItem* root )
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

void XUPFilteredProjectModel::populateProject( XUPProjectItem* project, bool updateView )
{
	XUPItemMappingIterator projectIterator = createMapping( project, project->parentProject() );
	
	XUPItemList variables = getFilteredVariables( project );
	foreach ( XUPItem* variable, variables )
	{
		XUPItemList tmpValuesItem = getValues( variable );
		XUPItem* tmp = projectIterator.value()->findVariable( variable->attribute( "name" ) );
		if ( tmp )
		{
			variable = tmp;
		}
		
		XUPItemMappingIterator variableIterator = createMapping( variable, project );
		foreach ( XUPItem* value, tmpValuesItem )
		{
			if ( !variableIterator.value()->findValue( value->attribute( "content" ) ) )
			{
				createMapping( value, variable );
			}
		}
		
		XUPItemList& variableValues = variableIterator.value()->mMappedChildren;
		qSortItems( variableValues );
	}
	
	XUPItemList& projectVariables = projectIterator.value()->mMappedChildren;
	qSortItems( projectVariables );
	
	if ( updateView )
	{
		int count = projectVariables.count();
		if ( count > 0 )
		{
			QModelIndex proxyIndex = mapFromSource( project );
			beginInsertRows( proxyIndex, 0, count -1 );
			endInsertRows();
		}
	}
}

void XUPFilteredProjectModel::internal_rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
	XUPItem* firstItem = static_cast<XUPItem*>( parent.child( start, 0 ).internalPointer() );
	XUPItemMappingIterator firstIt = mItemsMapping.constFind( firstItem );
	
	if ( firstIt == mItemsMapping.constEnd() )
	{
		return;
	}
	
	XUPItem* parentItem = firstIt.value()->mParent;
	XUPItemMappingIterator parentIt = mItemsMapping.constFind( parentItem );
	
	if ( parentIt != mItemsMapping.constEnd() )
	{
		QModelIndex parentProxy = mapFromSource( parentItem );
		for ( int i = start; i < end +1; i++ )
		{
			XUPItem* childItem = static_cast<XUPItem*>( parent.child( i, 0 ).internalPointer() );
			XUPItemMappingIterator childIt = mItemsMapping.constFind( childItem );
			
			if ( childIt != mItemsMapping.constEnd() )
			{
				QModelIndex childProxy = mapFromSource( childItem );
				int row = childProxy.row();
				beginRemoveRows( parentProxy, row, row );
				removeMapping( childItem );
				endRemoveRows();
			}
		}
	}
}
