#include "XUPProjectModel.h"
#include "XUPItem.h"

#include <QTextCodec>
#include <QIcon>
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
	return 1;
}

QVariant XUPProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case 0:
				return tr( "Project(s)" );
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
				QString fn = QString( ":/items/%1.png" ).arg( item->name() );
				if ( isType( item, "value" ) && isFileBased( item->parent() ) )
					fn = ":/items/file.png";
				return QIcon( fn );
			}
			else if ( role == Qt::DisplayRole )
			{
				const QString name = node.nodeName();
				const QDomElement e = node.toElement();
				if ( name == "project" )
					return e.attribute( "name", tr( "No Name" ) );
				else if ( name == "comment" )
					return e.attribute( "value" );
				else if ( name == "emptyline" )
					return tr( "%1 empty line(s)" ).arg( e.attribute( "count" ) );
				else if ( name == "variable" )
					return e.attribute( "name" );
				else if ( name == "value" )
					return e.attribute( "content" );
				else if ( name == "function" )
					return QString( "%1(%2)" ).arg( e.attribute( "name" ) ).arg( e.attribute( "parameters" ) );
				else if ( name == "scope" )
					return e.attribute( "name" );
				return QString();
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

bool XUPProjectModel::isType( XUPItem* item, const QString& type ) const
{
	return item->name() == type;
}

bool XUPProjectModel::isFileBased( XUPItem* item ) const
{
	return item->name() == "variable" && item->attributeValue( "name" ) == "FILES";
}

bool XUPProjectModel::isPathBased( XUPItem* item ) const
{
	return item->name() == "variable" && item->attributeValue( "name" ) == "FILES";
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
