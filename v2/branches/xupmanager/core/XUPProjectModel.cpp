#include "XUPProjectModel.h"
#include "XUPProjectItem.h"

#include <QDebug>

XUPProjectModel::XUPProjectModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mRootProject = 0;
}

XUPProjectModel::~XUPProjectModel()
{
	if ( mRootProject )
		mRootProject->close();
}

QModelIndex XUPProjectModel::index( int row, int column, const QModelIndex& parent ) const
{
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
	
	return QModelIndex();
}

QModelIndex XUPProjectModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return QModelIndex();

	XUPItem* childItem = static_cast<XUPItem*>( index.internalPointer() );
	XUPItem* parentItem = childItem->parent();

	if ( !parentItem || parentItem == mRootProject )
		return QModelIndex();

	return createIndex( parentItem->row(), 0, parentItem );
}

int XUPProjectModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.column() > 0 )
		return 0;

	XUPItem* parentItem;
	if ( !parent.isValid() )
		parentItem = mRootProject;
	else
		parentItem = static_cast<XUPItem*>( parent.internalPointer() );
	
	// try opening include projects if needed
	int count = parentItem->node().childNodes().count();
	if ( parentItem->isType( XUPItem::Function ) && parentItem->attribute( "name", QString::null ).toLower() == "include" )
	{
		XUPItem* child = parentItem->child( count );
		if ( child )
		{
			count++;
		}
		else
		{
			const QString fn = parentItem->value();
			XUPProjectItem* project = new XUPProjectItem();
			if ( project->open( fn, mEncoding ) )
			{
				project->mParentItem = parentItem;
				project->mRowNumber = count;
				parentItem->mChildItems[ count ] = project;
				count++;
			}
		}
	}

	return count;
}

int XUPProjectModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant XUPProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && section == 0 )
	{
		if ( mRootProject )
		{
			if ( role == Qt::DecorationRole )
				return mRootProject->icon();
			else if ( role == Qt::DisplayRole )
				return mRootProject->text();
		}
		else if ( role == Qt::DisplayRole )
			return tr( "No opened project" );
	}
	return QVariant();
}

QVariant XUPProjectModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
		return QVariant();

	if ( role != Qt::DecorationRole && role != Qt::DisplayRole && role != Qt::ToolTipRole )
		return QVariant();

	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );

	QDomNode node = item->node();
	QStringList attributes;
	QDomNamedNodeMap attributeMap = node.attributes();

	switch ( index.column() )
	{
		case 0:
			if ( role == Qt::DecorationRole )
			{
				return item->icon();
			}
			else if ( role == Qt::DisplayRole )
			{
				return item->text();
			}
			else
			{
				for ( int i = 0; i < attributeMap.count(); i++ )
				{
					QDomNode attribute = attributeMap.item( i );
					attributes << attribute.nodeName() +"=\"" +attribute.nodeValue() +"\"";
				}
				return attributes.join( "\n" );
			}
		default:
			return QVariant();
	}
}

Qt::ItemFlags XUPProjectModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void XUPProjectModel::setLastError( const QString& error )
{
	mLastError = error;
}

QString XUPProjectModel::lastError() const
{
	return mLastError;
}

bool XUPProjectModel::open( XUPProjectItem* projectItem, const QString& fileName, const QString& encoding )
{
	XUPProjectItem* tmpProject = projectItem;
	if ( tmpProject->open( fileName, encoding ) )
	{
		mEncoding = encoding;
		mRootProject = tmpProject;
		return true;
	}
	else
	{
		delete tmpProject;
	}
	return false;
}

void XUPProjectModel::close()
{
	if ( mRootProject )
	{
		delete mRootProject;
		mRootProject = 0;
	}
}

bool XUPProjectModel::save()
{
	setLastError( tr( "Can't save project, not yet implemented !" ) );
	return false;
}

QString XUPProjectModel::rootProjectName() const
{
	return mRootProject ? mRootProject->text() : QString::null;
}
