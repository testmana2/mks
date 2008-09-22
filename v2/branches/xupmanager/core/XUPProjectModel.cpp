#include "XUPProjectModel.h"
#include "XUPItem.h"

#include <QTextCodec>
#include <QDebug>

XUPProjectModel::XUPProjectModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mRootItem = 0;
}

XUPProjectModel::~XUPProjectModel()
{
	close();
}

QModelIndex XUPProjectModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
		return QModelIndex();
	
	XUPItem* parentItem;
	if ( !parent.isValid() )
		parentItem = mRootItem;
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

	if ( !parentItem || parentItem == mRootItem )
		return QModelIndex();

	return createIndex( parentItem->row(), 0, parentItem );
}

int XUPProjectModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.column() > 0 )
		return 0;

	XUPItem* parentItem;
	if ( !parent.isValid() )
		parentItem = mRootItem;
	else
		parentItem = static_cast<XUPItem*>( parent.internalPointer() );

	return parentItem->node().childNodes().count();
}

int XUPProjectModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 2;
}

QVariant XUPProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case 0:
				return tr( "Name" );
			case 1:
				return tr( "Value" );
			default:
				return QVariant();
		}
	}
	return QVariant();
}

QVariant XUPProjectModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
		return QVariant();

	if ( role != Qt::DisplayRole && role != Qt::ToolTipRole )
		return QVariant();

	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );

	QDomNode node = item->node();
	QStringList attributes;
	QDomNamedNodeMap attributeMap = node.attributes();

	switch ( index.column() )
	{
		case 0:
			if ( role == Qt::DisplayRole )
				return node.nodeName();
			else
			{
				for ( int i = 0; i < attributeMap.count(); i++ )
				{
					QDomNode attribute = attributeMap.item( i );
					attributes << attribute.nodeName() +"=\"" +attribute.nodeValue() +"\"";
				}
				return attributes.join("\n");
			}
		case 1:
			return node.nodeValue().split( "\n" ).join( " " );
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

bool XUPProjectModel::open( const QString& fileName, const QString& encoding )
{
	// get QFile
	QFile file( fileName );
	
	// check existence
	if ( !file.exists() )
	{
		setLastError( "file not exists" );
		return false;
	}
	
	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		setLastError( "can't open file for reading" );
		return false;
	}
	
	// decode content
	QTextCodec* codec = QTextCodec::codecForName( encoding.toUtf8() );
	QString buffer = codec->toUnicode( file.readAll() );
	
	// parse content
	QDomDocument doc;
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !doc.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	if ( doc.firstChildElement( "project" ).isNull() )
	{
		setLastError("no project node" );
		return false;
	}
	
	// all is ok
	mLastError.clear();
	mDocument = doc;
	mRootItem = new XUPItem( mDocument, 0 );
	file.close();
	return true;
}

bool XUPProjectModel::close()
{
	if ( mRootItem )
	{
		delete mRootItem;
		mRootItem = 0;
	}
	return true;
}
