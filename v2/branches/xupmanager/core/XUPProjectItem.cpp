#include "XUPProjectItem.h"

#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

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
	mLastError = error;
}

QString XUPProjectItem::lastError() const
{
	return mLastError;
}

QString XUPProjectItem::fileName() const
{
	return mFileName;
}

QString XUPProjectItem::filePath( const QString& fileName ) const
{
	QString fn = QFileInfo( mFileName ).path().append( "/" );
	fn.append( fileName );
	return QDir::cleanPath( fn );
}

XUPProjectItemInfos* XUPProjectItem::projectInfos()
{
	return mXUPProjectInfos;
}

bool XUPProjectItem::isFileBased( XUPItem* item ) const
{
	return item->typeId() == XUPItem::Variable && mXUPProjectInfos->isFileBased( projectType(), item->attribute( "name" ) );
}

bool XUPProjectItem::isPathBased( XUPItem* item ) const
{
	return item->typeId() == XUPItem::Variable && mXUPProjectInfos->isPathBased( projectType(), item->attribute( "name" ) );
}

QString XUPProjectItem::iconFileName( XUPItem* item ) const
{
	int pType = projectType();
	QString fn;
	
	if ( item->typeId() == XUPItem::Variable )
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
	mFileName = fileName;
	mLastError.clear();
	mRowNumber = 0;
	file.close();
	
	return true;
}

void XUPProjectItem::close()
{
}

void XUPProjectItem::customRowCount( XUPItem* item )
{
	Q_UNUSED( item );
}
