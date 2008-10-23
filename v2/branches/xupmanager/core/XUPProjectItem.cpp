#include "XUPProjectItem.h"
#include "../iconmanager/pIconManager.h"

#include <QTextCodec>
#include <QDir>
#include <QRegExp>

#include <QDebug>

XUPProjectItemInfos* XUPProjectItem::mXUPProjectInfos = new XUPProjectItemInfos();
bool XUPProjectItem::mFoundCallerItem = false;

XUPProjectItem::XUPProjectItem()
	: XUPItem( QDomElement(), 0 )
{
}

XUPProjectItem::~XUPProjectItem()
{
}

void XUPProjectItem::setLastError( const QString& error )
{
	setTemporaryValue( "lastError", error );
}

QString XUPProjectItem::lastError() const
{
	return temporaryValue( "lastError" ).toString();
}

QString XUPProjectItem::fileName() const
{
	return temporaryValue( "fileName" ).toString();
}

QString XUPProjectItem::path() const
{
	return QFileInfo( fileName() ).path();
}

QString XUPProjectItem::filePath( const QString& fn ) const
{
	if ( fn.isEmpty() )
		return QString::null;
	QString fname = QFileInfo( fn ).isRelative() ? path().append( "/" ).append( fn ) : fn;
	return QDir::cleanPath( fname );
}

#warning need use pMonkeyStudio::getFiles()
const QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
			files << file;
		else if ( file.isDir() && recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters );
			fromDir.cdUp();
		}
	}
	return files;
}

QStringList XUPProjectItem::splitMultiLineValue( const QString& value ) const
{
	QStringList tmpValues = value.split( " " );
	bool inStr = false;
	QStringList multivalues;
	QString ajout;

	for(int ku = 0;ku < tmpValues.size();ku++)
	{
		if(tmpValues.value(ku).startsWith('"') )
				inStr = true;
		if(inStr)
		{
			if(ajout != "")
					ajout += " ";
			ajout += tmpValues.value(ku);
			if(tmpValues.value(ku).endsWith('"') )
			{
					multivalues += ajout;
					ajout = "";
					inStr = false;
			}
		}
		else
		{
			multivalues += tmpValues.value(ku);
		}
	}

	return multivalues;
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
	const QString projectPath = path();
	const QStringList variablesPath = mXUPProjectInfos->pathVariables( projectType() );
	QStringList paths;
	foreach ( const QString& variable, variablesPath )
	{
		QString tmpPaths = interpretVariable( variable );
		foreach ( QString path, splitMultiLineValue( tmpPaths ) )
		{
			path = filePath( path.remove( '"' ) );
			if ( !paths.contains( path ) && !path.startsWith( projectPath ) )
			{
				paths << path;
			}
		}
	}
	
	// add project path
	paths << projectPath;
	// sort path
	qSort( paths );
	qWarning() << "paths" << paths;

	QDir dir( path() );
	return getFiles( dir, QStringList( partialFilePath ), true );
}

XUPProjectItemInfos* XUPProjectItem::projectInfos()
{
	return mXUPProjectInfos;
}

XUPProjectItem* XUPProjectItem::parentProject() const
{
	if ( mParentItem )
		return mParentItem->project();
	return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::topLevelProject() const
{
	if ( mParentItem )
		return mParentItem->project()->topLevelProject();
	return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::rootIncludeProject() const
{
	if ( mParentItem && mParentItem->type() == XUPItem::Function && mParentItem->attribute( "name" ).toLower() == "include" )
		return mParentItem->project()->rootIncludeProject();
	return const_cast<XUPProjectItem*>( this );
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

QString XUPProjectItem::itemDisplayText( XUPItem* item )
{
	QString text;
	
	if ( item->temporaryValue( "hasDisplayText", false ).toBool() )
	{
		text = item->temporaryValue( "displayText" ).toString();
	}
	else
	{
		item->setTemporaryValue( "hasDisplayText", true );
		switch ( item->type() )
		{
			case XUPItem::Project:
				text = item->attribute( "name" );
				break;
			case XUPItem::Comment:
				text = item->attribute( "value" );
				break;
			case XUPItem::EmptyLine:
				text = tr( "%1 empty line(s)" ).arg( item->attribute( "count" ) );
				break;
			case XUPItem::Variable:
				text = variableDisplayText( item->attribute( "name" ) );
				break;
			case XUPItem::Value:
				text = item->attribute( "content" );
				break;
			case XUPItem::Function:
				text = QString( "%1(%2)" ).arg( item->attribute( "name" ) ).arg( item->attribute( "parameters" ) );
				break;
			case XUPItem::Scope:
				text = item->attribute( "name" );
				break;
			case XUPItem::DynamicFolder:
				text = item->attribute( "name" );
				break;
			case XUPItem::Folder:
				text = item->attribute( "name" );
				break;
			case XUPItem::File:
				text = QFileInfo( item->attribute( "content" ) ).fileName();
				break;
			case XUPItem::Path:
				text = item->attribute( "content" );
				break;
			default:
				text = "#Unknow";
				break;
		}
		item->setTemporaryValue( "displayText", text );
	}
	
	return text;
}

QIcon XUPProjectItem::itemDisplayIcon( XUPItem* item )
{
	QIcon icon;
	
	if ( item->temporaryValue( "hasDisplayIcon", false ).toBool() )
	{
		icon = item->temporaryValue( "displayIcon" ).value<QIcon>();
	}
	else
	{
		item->setTemporaryValue( "hasDisplayIcon", true );
		QString path = iconsPath();
		QString fn = pIconManager::filePath( iconFileName( item ), path );
		
		if ( !QFile::exists( fn ) )
		{
			path = mXUPProjectInfos->pixmapsPath( XUPProjectItem::XUPProject );
		}
		
		icon = pIconManager::icon( iconFileName( item ), path );
		item->setTemporaryValue( "displayIcon", icon );
	}
	
	return icon;
}

QList<XUPItem*> XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, const XUPItem* callerItem ) const
{
	mFoundCallerItem = false;
	QList<XUPItem*> variables;
	
	for ( int i = 0; i < root->childCount(); i++ )
	{
		XUPItem* item = root->child( i );
		
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
		
		if ( callerItem && item == callerItem )
		{
			mFoundCallerItem = true;
			break;
		}
		
		if ( mFoundCallerItem )
			break;
	}
	
	return variables;
}

QString XUPProjectItem::interpretVariable( const QString& variableName, const XUPItem* callerItem, const QString& defaultValue ) const
{
	/*
		$${varName} or $$varName : read content from defined variable
		$$(varName) : read from environment
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '(', "" ).replace( ')', "" );
	QList<QStringList> value;
	
	// environment var
	if ( variableName.startsWith( "$$(" ) || name == "PWD" )
	{
		return name != "PWD" ? qgetenv( name.toLocal8Bit().constData() ) : ( callerItem ? callerItem->project()->path() : path() );
	}
	else
	{
		QList<XUPItem*> variableItems = getVariables( this, name, callerItem );
		
		foreach ( XUPItem* variableItem, variableItems )
		{
			const QString op = variableItem->attribute( "operator", "=" );
			QStringList tmp;
			for ( int i = 0; i < variableItem->childCount(); i++ )
			{
				XUPItem* valueItem = variableItem->child( i );
				if ( valueItem->type() == XUPItem::Value ||
					valueItem->type() == XUPItem::File ||
					valueItem->type() == XUPItem::Path
				)
				{
					tmp << interpretValue( valueItem, "content" );
				}
			}
			
			if ( op == "=" )
			{
				value = QList<QStringList>() << QStringList( tmp );
			}
			else if ( op == "-=" )
			{
				value.removeAll( tmp );
			}
			else if ( op == "+=" )
			{
				value << tmp;
			}
			else if ( op == "*=" )
			{
				if ( !value.contains( tmp ) )
					value << tmp;
			}
		}
	}
	
	QStringList result;
	foreach ( const QStringList& values, value )
		result << values;
	
	return result.isEmpty() ? defaultValue : result.join( " " );
}

QString XUPProjectItem::interpretValue( XUPItem* callerItem, const QString& attribute ) const
{
	QRegExp rx( "\\$\\$[\\{\\(]?(\\w+(?!\\w*\\s*[()]))[\\}\\)]?" );
	const QString content = callerItem->attribute( attribute );
	QString value = content;
	int pos = 0;
	
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		value.replace( rx.cap( 0 ), interpretVariable( rx.cap( 0 ), callerItem ) );
		pos += rx.matchedLength();
	}
	
	return value;
}

void XUPProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();
	
	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType, const_cast<XUPProjectItem*>( this ) );
	
	// values
	const QString mPixmapsPath = ":/items";
	const QStringList mOperators = QStringList( "=" ) << "+=" << "-=" << "*=";
	const QStringList mFilteredVariables = QStringList( "FILES" );
	const QStringList mFileVariables = QStringList( "FILES" );
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( QT_TR_NOOP( "XUP Project" ) ), QStringList( "*.xup" ) )
		<< qMakePair( tr( QT_TR_NOOP( "XUP Include Project" ) ), QStringList( "*.xui" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FILES" ), tr( "Files" ) );
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
			function->setTemporaryValue( "includeHandled", true );
			const QString parameters = function->project()->rootIncludeProject()->interpretValue( function, "parameters" );
			const QString fn = filePath( parameters );
			
			XUPProjectItem* project = newProject();
			if ( project->open( fn, attribute( "encoding" ) ) )
			{
				function->addChild( project );
			}
			else
			{
				const_cast<XUPProjectItem*>( this )->setLastError( tr( "Failed to handle include project %1" ).arg( fn ) );
				delete project;
			}
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
		setLastError( "no project node" );
		return false;
	}
	
	// all is ok
	setAttribute( "encoding", encoding );
	setTemporaryValue( "fileName", fileName );
	setLastError( QString::null );
	file.close();
	
	return true;
}

void XUPProjectItem::close()
{
}
