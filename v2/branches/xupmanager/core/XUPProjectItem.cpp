#include "XUPProjectItem.h"

#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>

#include <QDebug>

XUPProjectItemInfos* XUPProjectItem::mXUPProjectInfos = new XUPProjectItemInfos();

XUPProjectItem::XUPProjectItem()
	: XUPItem( QDomElement(), -1, 0 )
{
	if ( !mXUPProjectInfos->isRegisteredType( projectType() ) )
	{
		registerProjectType();
	}
}

XUPProjectItem::~XUPProjectItem()
{
}

void XUPProjectItem::setLastError( const QString& error )
{
	setTemporaryValue( "lasterror", error );
}

QString XUPProjectItem::lastError() const
{
	return temporaryValue( "lasterror" ).toString();
}

QString XUPProjectItem::fileName() const
{
	return temporaryValue( "filename" ).toString();
}

QString XUPProjectItem::path() const
{
	return QFileInfo( fileName() ).path();
}

QString XUPProjectItem::filePath( const QString& fn ) const
{
	QString fname = path().append( "/" ).append( fn );
	return QDir::cleanPath( fname );
}

XUPProjectItemInfos* XUPProjectItem::projectInfos()
{
	return mXUPProjectInfos;
}

bool XUPProjectItem::isFileBased( XUPItem* item ) const
{
	return item->type() == XUPItem::Variable && mXUPProjectInfos->isFileBased( projectType(), item->attribute( "name" ) );
}

bool XUPProjectItem::isPathBased( XUPItem* item ) const
{
	return item->type() == XUPItem::Variable && mXUPProjectInfos->isPathBased( projectType(), item->attribute( "name" ) );
}

QString XUPProjectItem::iconFileName( XUPItem* item ) const
{
	int pType = projectType();
	QString fn;
	
	if ( item->type() == XUPItem::Variable )
	{
		fn = mXUPProjectInfos->iconName( pType, item->attribute( "name" ) );
	}
	
	if ( fn.isEmpty() )
	{
		fn = item->mDomElement.nodeName();
	}
	
	if ( !fn.isEmpty() )
	{
		fn.append( ".png" );
	}
	
	return fn;
}

QString XUPProjectItem::iconsPath() const
{
	int pType = projectType();
	QString path = mXUPProjectInfos->pixmapsPath( pType );
	if ( path.isEmpty() && pType != XUPProjectItem::XUPProject )
	{
		path = mXUPProjectInfos->pixmapsPath( XUPProjectItem::XUPProject );
	}
	return path;
}

QString XUPProjectItem::variableDisplayText( const QString& variableName ) const
{
	return mXUPProjectInfos->displayText( projectType(), variableName );
}

QString XUPProjectItem::valueDisplayText( XUPItem* valueItem ) const
{
	bool mIsFileBased = isFileBased( valueItem->parent() );
	return mIsFileBased ? QFileInfo( valueItem->attribute( "content" ) ).fileName() : valueItem->attribute( "content" );
}

void XUPProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();
	
	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType );
	
	// values
	const QString mPixmapsPath = ":/items";
	const QStringList mOperators = QStringList( "=" );
	const QStringList mFilteredVariables = QStringList( "FILES" );
	const QStringList mFileVariables = QStringList( "FILES" );
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( QT_TR_NOOP( "XUP Project" ) ), QStringList( "*.xup" ) )
		<< qMakePair( tr( QT_TR_NOOP( "XUP Include Project" ) ), QStringList( "*.xui" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FILES" ), tr( QT_TR_NOOP( "Files" ) ) );
	const StringStringList mVariableIcons = StringStringList()
		<< qMakePair( QString( "FILES" ), QString( "files" ) );
	const StringStringListList mVariableSuffixes = StringStringListList()
		<< qMakePair( QString( "FILES" ), QStringList( "*" ) );
	
	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mFileVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerVariableSuffixes( pType, mVariableSuffixes );
}

void XUPProjectItem::handleIncludeItem( XUPItem* function ) const
{
	if ( function->type() == XUPItem::Function && function->attribute( "name" ).toLower() == "include" )
	{
		if ( !function->temporaryValue( "includeHandled", false ).toBool() )
		{
#warning Need use a highLevelProject wich is the most appropriate toplevel project
			const QString parameters = function->project()->interpretValue( function, "parameters" );
			const QString fn = QFileInfo( parameters ).isRelative() ? filePath( parameters ) : parameters;
			XUPProjectItem* project = newItem();
			if ( project->open( fn, attribute( "encoding" ) ) )
			{
				int count = function->count();
				project->mParentItem = function;
				project->mRowNumber = count;
				function->mChildItems[ count ] = project;
			}
			else
			{
				qWarning() << "Failed to handle include" << function->attribute( "parameters" ) << parameters;
				delete project;
			}
			function->setTemporaryValue( "includeHandled", true );
		}
	}
}

void XUPProjectItem::customRowCount( XUPItem* item ) const
{
	Q_UNUSED( item );
}

bool XUPProjectItem::open( const QString& fileName, const QString& encoding )
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
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		setLastError("no project node" );
		return false;
	}
	
	// all is ok
	setAttribute( "encoding", encoding );
	setTemporaryValue( "filename", fileName );
	setLastError( QString::null );
	mRowNumber = 0;
	file.close();
	
	return true;
}

void XUPProjectItem::close()
{
}

QList<XUPItem*> XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, const XUPItem* callerItem ) const
{
	QList<XUPItem*> variables;
	
	for ( int i = 0; i < root->count(); i++ )
	{
		XUPItem* item = root->child( i );
		
		if ( callerItem && item == callerItem )
			break;
		
		switch ( item->type() )
		{
			case XUPItem::Project:
			{
				XUPItem* pItem = item->parent();
				if ( pItem->type() == XUPItem::Function && pItem->attribute( "name" ).toLower() == "include" )
					variables << getVariables( item, variableName, callerItem );
				break;
			}
			case XUPItem::Comment:
				break;
			case XUPItem::EmptyLine:
				break;
			case XUPItem::Variable:
			{
				if ( item->attribute( "name" ) == variableName )
					variables << item;
				break;
			}
			case XUPItem::Value:
				break;
			case XUPItem::Function:
			{
				XUPProjectItem* project = item->project();
				project->handleIncludeItem( item );
				variables << getVariables( item, variableName, callerItem );
				break;
			}
			case XUPItem::Scope:
			{
				variables << getVariables( item, variableName, callerItem );
				break;
			}
			default:
				break;
		}
	}
	
	return variables;
}

QString XUPProjectItem::interpreteVariable( const QString& variableName, const XUPItem* callerItem, const QString& defaultValue ) const
{
	/*
		$${varName} or $$varName : read content from defined variable
		$$(varName) : read from environment
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '(', "" ).replace( ')', "" );
	// environment var
	if ( variableName.startsWith( "$$(" ) || name == "PWD" )
		return name != "PWD" ? qgetenv( name.toLocal8Bit().constData() ) : ( callerItem ? callerItem->project()->path() : path() );
	else
	{
		QString value;
		QList<XUPItem*> variableItems = getVariables( this, name, callerItem );
		foreach ( XUPItem* variableItem, variableItems )
		{
			const QString op = variableItem->attribute( "operator", "=" );
			QString tmp;
			for ( int i = 0; i < variableItem->count(); i++ )
			{
				XUPItem* valueItem = variableItem->child( i );
				if ( valueItem->type() == XUPItem::Value )
				{
					tmp += interpretValue( valueItem, "content" ) +" ";
					tmp = tmp.trimmed();
					if ( op == "=" )
					{
						value = tmp;
					}
				}
			}
		}
		return value;
	}
	
	return defaultValue;
}

QString XUPProjectItem::interpretValue( XUPItem* callerItem, const QString& attribute ) const
{
	QRegExp rx( "\\$\\$[\\{\\(]?(\\w+(?!\\w*\\s*[()]))[\\}\\)]?" );
	const QString content = callerItem->attribute( attribute );
	QString value = content;
	int pos = 0;
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		value.replace( rx.cap( 0 ), interpreteVariable( rx.cap( 0 ), callerItem ) );
		pos += rx.matchedLength();
	}
	return value;
}
