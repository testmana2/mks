#include "XUPProjectItem.h"
#include "pIconManager.h"
#include "BuilderPlugin.h"
#include "CompilerPlugin.h"
#include "DebuggerPlugin.h"
#include "InterpreterPlugin.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"

#include <QTextCodec>
#include <QDir>
#include <QRegExp>
#include <QProcess>

#include <QDebug>

const QString XUP_VERSION = "1.1.0";

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

QString XUPProjectItem::relativeFilePath( const QString& fileName ) const
{
	QDir dir( path() );
	return dir.relativeFilePath( fileName );
}

QStringList XUPProjectItem::splitMultiLineValue( const QString& value ) const
{
	QStringList tmpValues = value.split( " ", QString::SkipEmptyParts );
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

QString XUPProjectItem::matchingPath( const QString& left, const QString& right ) const
{
	QString result;
	for ( int i = 1; i < left.count() +1; i++ )
	{
		result = left.left( i );
		if ( !right.startsWith( result ) )
		{
			result.chop( 1 );
			break;
		}
	}
	
	if ( QDir::drives().contains( result ) || result.isEmpty() )
	{
		return QString::null;
	}
	
	return result;
}

QStringList XUPProjectItem::compressedPaths( const QStringList& paths ) const
{
	QStringList pathsList = paths;
	QStringList result;
	
	qSort( pathsList );
	foreach ( const QString& path, pathsList )
	{
		if ( result.isEmpty() )
		{
			result << path;
		}
		else
		{
			QString matching = matchingPath( path, result.last() );
			if ( matching.isEmpty() )
			{
				result << path;
			}
			else
			{
				result.removeLast();
				result << matching;
			}
		}
	}
	
	return result;
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
	XUPProjectItem* riProject = rootIncludeProject();
	const QString projectPath = path();
	const QStringList variablesPath = mXUPProjectInfos->pathVariables( projectType() );
	QStringList paths;
	
	// add project path and variables content based on path
	paths << projectPath;
	foreach ( const QString& variable, variablesPath )
	{
		QString tmpPaths = riProject->variableCache().value( variable );
		
		foreach ( QString path, splitMultiLineValue( tmpPaths ) )
		{
			path = filePath( path.remove( '"' ) );
			if ( !paths.contains( path ) && !path.startsWith( projectPath ) )
			{
				paths << path;
			}
		}
	}
	
	// get compressed path list
	paths = compressedPaths( paths );
	
	//qWarning() << "looking in" << paths;
	
	// get all matching files in paths
	QFileInfoList files;
	QDir dir;
	
	foreach ( const QString& path, paths )
	{
		dir.setPath( path );
		files << pMonkeyStudio::getFiles( dir, QFileInfo( partialFilePath ).fileName(), true );
	}

	return files;
}

XUPProjectItemInfos* XUPProjectItem::projectInfos()
{
	return mXUPProjectInfos;
}

QMap<QString, QString>&  XUPProjectItem::variableCache()
{
	return mVariableCache;
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
	return mXUPProjectInfos->iconsPath( projectType() );
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

XUPItemList XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, const XUPItem* callerItem, bool recursive ) const
{
	mFoundCallerItem = false;
	XUPItemList variables;
	
	for ( int i = 0; i < root->childCount(); i++ )
	{
		XUPItem* item = root->child( i );
		
		switch ( item->type() )
		{
			case XUPItem::Project:
			{
				if ( recursive )
				{
					XUPItem* pItem = item->parent();
					if ( pItem->type() == XUPItem::Function && pItem->attribute( "name" ).toLower() == "include" )
					{
						variables << getVariables( item, variableName, callerItem );
					}
				}
				break;
			}
			case XUPItem::Comment:
				break;
			case XUPItem::EmptyLine:
				break;
			case XUPItem::Variable:
			{
				if ( item->attribute( "name" ) == variableName )
				{
					variables << item;
				}
				break;
			}
			case XUPItem::Value:
				break;
			case XUPItem::Function:
			{
				if ( recursive )
				{
					variables << getVariables( item, variableName, callerItem );
				}
				break;
			}
			case XUPItem::Scope:
			{
				if ( recursive )
				{
					variables << getVariables( item, variableName, callerItem );
				}
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

QString XUPProjectItem::toString() const
{
	return mDocument.toString( 4 );
}

XUPItem* XUPProjectItem::projectSettingsScope( bool create ) const
{
	XUPProjectItem* project = topLevelProject();
	
	if ( project )
	{
		const QString mScopeName = "XUPProjectSettings";
		XUPItemList items = project->childrenList();
		
		foreach ( XUPItem* child, items )
		{
			if ( child->type() == XUPItem::Scope && child->attribute( "name" ) == mScopeName )
			{
				child->setAttribute( "nested", "false" );
				return child;
			}
		}
		
		if ( create )
		{
			XUPItem* scope = project->addChild( XUPItem::Scope, 0 );
			scope->setAttribute( "name", mScopeName );
			scope->setAttribute( "nested", "false" );
			
			XUPItem* emptyline = project->addChild( XUPItem::EmptyLine, 1 );
			emptyline->setAttribute( "count", "1" );
			
			return scope;
		}
	}
	
	return 0;
}

QStringList XUPProjectItem::projectSettingsValues( const QString& variableName, const QStringList& defaultValues ) const
{
	QStringList values;
	XUPProjectItem* project = topLevelProject();
	
	if ( project )
	{
		XUPItem* scope = projectSettingsScope( false );
		
		if ( scope )
		{
			XUPItemList variables = getVariables( scope, variableName, 0, false );
			
			foreach ( const XUPItem* variable, variables )
			{
				foreach ( const XUPItem* child, variable->childrenList() )
				{
					if ( child->type() == XUPItem::Value )
					{
						values << child->attribute( "content" );
					}
				}
			}
		}
	}
	
	if ( values.isEmpty() )
	{
		// a hack that allow xupproejct settings variable to be added to project node
		if ( defaultValues.isEmpty() )
		{
			values = QStringList( attribute( variableName.toLower() ) );
		}
		else
		{
			values = defaultValues;
		}
	}
	
	return values;
}

void XUPProjectItem::setProjectSettingsValues( const QString& variableName, const QStringList& values )
{	
	XUPProjectItem* project = topLevelProject();
	
	if ( project )
	{
		XUPItem* scope = projectSettingsScope( !values.isEmpty() );
		
		if ( !scope )
		{
			return;
		}
		
		XUPItemList variables = getVariables( scope, variableName, 0, false );
		XUPItem* variable = variables.value( 0 );
		bool haveVariable = variable;
		
		if ( !haveVariable && values.isEmpty() )
		{
			return;
		}
		
		if ( haveVariable && values.isEmpty() )
		{
			scope->removeChild( variable );
			return;
		}
		
		if ( !haveVariable )
		{
			variable = scope->addChild( XUPItem::Variable );
			variable->setAttribute( "name", variableName );
			variable->setAttribute( "multiline", "true" );
		}
		
		QStringList cleanValues = values;
		foreach ( XUPItem* child, variable->childrenList() )
		{
			if ( child->type() == XUPItem::Value )
			{
				QString value = child->attribute( "content" );
				if ( cleanValues.contains( value ) )
				{
					cleanValues.removeAll( value );
				}
				else if ( !cleanValues.contains( value ) )
				{
					variable->removeChild( child );
				}
			}
		}
		
		foreach ( const QString& value, cleanValues )
		{
			XUPItem* valueItem = variable->addChild( XUPItem::Value );
			valueItem->setAttribute( "content", value );
		}
	}
}

void XUPProjectItem::addProjectSettingsValues( const QString& variableName, const QStringList& values )
{
	XUPProjectItem* project = topLevelProject();
	
	if ( project )
	{
		XUPItem* scope = projectSettingsScope( !values.isEmpty() );
		
		if ( !scope )
		{
			return;
		}
		
		XUPItemList variables = getVariables( scope, variableName, 0, false );
		XUPItem* variable = variables.value( 0 );
		bool haveVariable = variable;
		
		if ( !haveVariable && values.isEmpty() )
		{
			return;
		}
		
		if ( haveVariable && values.isEmpty() )
		{
			return;
		}
		
		if ( !haveVariable )
		{
			variable = scope->addChild( XUPItem::Variable );
			variable->setAttribute( "name", variableName );
			variable->setAttribute( "multiline", "true" );
		}
		
		QStringList cleanValues = values;
		foreach ( XUPItem* child, variable->childrenList() )
		{
			if ( child->type() == XUPItem::Value )
			{
				QString value = child->attribute( "content" );
				if ( cleanValues.contains( value ) )
				{
					cleanValues.removeAll( value );
				}
			}
		}
		
		foreach ( const QString& value, cleanValues )
		{
			XUPItem* valueItem = variable->addChild( XUPItem::Value );
			valueItem->setAttribute( "content", value );
		}
	}
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
		<< qMakePair( tr( "XUP Project" ), QStringList( "*.xup" ) )
		<< qMakePair( tr( "XUP Include Project" ), QStringList( "*.xui" ) );
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

void XUPProjectItem::unRegisterProjectType() const
{
	mXUPProjectInfos->unRegisterType( projectType() );
}

QString XUPProjectItem::getVariableContent( const QString& variableName )
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	
	// environment var
	if ( variableName.startsWith( "$$(" ) || variableName.startsWith( "$(" ) )
	{
		if ( name == "PWD" )
		{
			return rootIncludeProject()->path();
		}
		else
		{
			return QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
		}
	}
	else
	{
		if ( name == "PWD" )
		{
			return project()->path();
		}
		else
		{
			return rootIncludeProject()->variableCache().value( name );
		}
	}
	
	return QString::null;
}

QString XUPProjectItem::interpretContent( const QString& content )
{
	QRegExp rx( "\\$\\$?[\\{\\(\\[]?([\\w\\.]+(?!\\w*\\s*\\{\\[\\(\\)\\]\\}))[\\]\\)\\}]?" );
	QString value = content;
	int pos = 0;
	
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		value.replace( rx.cap( 0 ), getVariableContent( rx.cap( 0 ) ) );
		pos += rx.matchedLength();
	}
	
	return value;
}

bool XUPProjectItem::handleIncludeFile( XUPItem* function )
{
	const QString parameters = function->cacheValue( "parameters" );
	const QString fn = filePath( parameters );
	QStringList projects;
	
	foreach ( XUPItem* cit, function->childrenList() )
	{
		if ( cit->type() == XUPItem::Project )
		{
			projects << cit->project()->fileName();
		}
	}
	
	// check if project is already handled
	if ( projects.contains( fn ) )
	{
		return true;
	}
	
	// open project
	XUPProjectItem* project = newProject();
	function->addChild( project );
	
	// remove and delete project if can't open
	if ( !project->open( fn, temporaryValue( "codec" ).toString() ) )
	{
		function->removeChild( project );
		topLevelProject()->setLastError( tr( "Failed to handle include file %1" ).arg( fn ) );
		return false;
	}
	
	return true;
}

bool XUPProjectItem::analyze( XUPItem* item )
{
	QStringList values;
	
	foreach ( XUPItem* cItem, item->childrenList() )
	{
		switch ( cItem->type() )
		{
			case XUPItem::Value:
			case XUPItem::File:
			case XUPItem::Path:
			{
				QString content = interpretContent( cItem->attribute( "content" ) );
				values << content;
				
				cItem->setCacheValue( "content", content );
				break;
			}
			case XUPItem::Function:
			{
				QString parameters = interpretContent( cItem->attribute( "parameters" ) );
				
				cItem->setCacheValue( "parameters", parameters );
				break;
			}
			case XUPItem::Project:
			case XUPItem::Comment:
			case XUPItem::EmptyLine:
			case XUPItem::Variable:
			case XUPItem::Scope:
			case XUPItem::DynamicFolder:
			case XUPItem::Folder:
			default:
				break;
		}
		
		if ( !analyze( cItem ) )
		{
			return false;
		}
	}
	
	if ( item->type() == XUPItem::Variable )
	{
		QString name = item->attribute( "name" );
		QString op = item->attribute( "operator", "=" );
		XUPProjectItem* proj = rootIncludeProject();
		
		if ( op == "=" )
		{
			proj->variableCache()[ name ] = values.join( " " );
		}
		else if ( op == "-=" )
		{
			foreach ( const QString& value, values )
			{
				proj->variableCache()[ name ].replace( QRegExp( QString( "\\b%1\\b" ).arg( value ) ), QString::null );
			}
		}
		else if ( op == "+=" )
		{
			proj->variableCache()[ name ] += " " +values.join( " " );
		}
		else if ( op == "*=" )
		{
			//if ( !proj->variableCache()[ name ].contains( content ) )
			{
				proj->variableCache()[ name ] += " " +values.join( " " );
			}
		}
	}
	
	// handle include projects
	if ( item->attribute( "name" ).toLower() == "include" )
	{
		if ( !handleIncludeFile( item ) )
		{
			return false;
		}
	}
	
	return true;
}

bool XUPProjectItem::open( const QString& fileName, const QString& codec )
{
	// get QFile
	QFile file( fileName );
	
	// check existence
	if ( !file.exists() )
	{
		topLevelProject()->setLastError( "file not exists" );
		return false;
	}
	
	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		topLevelProject()->setLastError( "can't open file for reading" );
		return false;
	}
	
	// decode content
	QTextCodec* c = QTextCodec::codecForName( codec.toUtf8() );
	QString buffer = c->toUnicode( file.readAll() );
	
	// parse content
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		topLevelProject()->setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		topLevelProject()->setLastError( "no project node" );
		return false;
	}
	
	// all is ok
	setTemporaryValue( "codec", codec );
	setTemporaryValue( "fileName", fileName );
	topLevelProject()->setLastError( QString::null );
	file.close();
	
	return true;
}

bool XUPProjectItem::save()
{
	// try open file for writing
	QFile file( temporaryValue( "fileName" ).toString() );
	
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		return false;
	}
	
	// erase file content
	file.resize( 0 );
	
	// set xup version
	setAttribute( "version", XUP_VERSION );
	
	// encode content
	QTextCodec* codec = QTextCodec::codecForName( temporaryValue( "codec" ).toString().toUtf8() );
	QByteArray content = codec->fromUnicode( toString() );
	
	// write content
	bool result = file.write( content ) != -1;
	file.close();
	
	// set error message if needed
	if ( result )
	{
		topLevelProject()->setLastError( QString::null );
	}
	else
	{
		topLevelProject()->setLastError( tr( "Can't write content" ) );
	}
	
	return result;
}

BuilderPlugin* XUPProjectItem::builder( const QString& plugin ) const
{
	return MonkeyCore::pluginsManager()->plugin<BuilderPlugin*>( PluginsManager::stAll, projectSettingsValue( "BUILDER", plugin ) );
}

CompilerPlugin* XUPProjectItem::compiler( const QString& plugin ) const
{
	return MonkeyCore::pluginsManager()->plugin<CompilerPlugin*>( PluginsManager::stAll, projectSettingsValue( "COMPILER", plugin ) );
}

DebuggerPlugin* XUPProjectItem::debugger( const QString& plugin ) const
{
	return MonkeyCore::pluginsManager()->plugin<DebuggerPlugin*>( PluginsManager::stAll, projectSettingsValue( "DEBUGGER", plugin ) );
}

InterpreterPlugin* XUPProjectItem::interpreter( const QString& plugin ) const
{
	return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, projectSettingsValue( "INTERPRETER", plugin ) );
}

void XUPProjectItem::addCommand( const pCommand& cmd, const QString& mnu )
{
	if ( cmd.isValid() )
	{
		emit installCommandRequested( cmd, mnu );
		mCommands.insertMulti( mnu, cmd );
	}
}

void XUPProjectItem::installCommands()
{
	// get plugins
	BuilderPlugin* bp = builder();
	CompilerPlugin* cp = compiler();
	//DebuggerPlugin* dp = debugger();
	InterpreterPlugin* ip = interpreter();
	
	bool emptyBuilderBuildMenu = MonkeyCore::menuBar()->menu( "mBuilder/mBuild" )->actions().isEmpty();
	bool emptyInterpreterMenu = MonkeyCore::menuBar()->menu( "mInterpreter" )->actions().isEmpty();
	
	// build command
	if ( bp && emptyBuilderBuildMenu )
	{
		pCommand cmd = bp->buildCommand();
		
		if ( cp )
		{
			cmd.addParsers( cp->compileCommand().parsers() );
		}
		
		cmd.setUserData( QVariant::fromValue( &mCommands ) );
		cmd.setProject( this );
		cmd.setSkipOnError( false );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// clean
		cmd.setText( tr( "Clean" ) );
		cmd.setArguments( "clean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// distclean
		cmd.setText( tr( "Distclean" ) );
		cmd.setArguments( "distclean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// rebuild
		cmd.setText( tr( "Rebuild" ) );
		cmd.setCommand( ( QStringList() << tr( "Clean" ) << tr( "Build" ) ).join( ";" ) );
		cmd.setArguments( QString() );
		addCommand( cmd, "mBuilder/mRebuild" );
	}
	
	// compile file command
	if ( cp && emptyBuilderBuildMenu )
	{
		pCommand cmd = cp->compileCommand();
		cmd.setUserData( QVariant::fromValue( &mCommands ) );
		cmd.setProject( this );
		cmd.setSkipOnError( false );
		addCommand( cmd, "mBuilder/mBuild" );
	}
	
	// interprete file command
	if ( ip && emptyInterpreterMenu )
	{
		pCommand cmd = ip->interpretCommand();
		cmd.setUserData( QVariant::fromValue( &mCommands ) );
		cmd.setProject( this );
		cmd.setSkipOnError( false );
		addCommand( cmd, "mInterpreter" );
	}
	
	// install builder user command
	if ( bp )
	{
		foreach ( pCommand cmd, bp->userCommands() )
		{
			if ( cp )
				cmd.addParsers( cp->compileCommand().parsers() );
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder/mUserCommands" );
		}
	}
	
	// install compiler user command
	if ( cp )
	{
		foreach ( pCommand cmd, cp->userCommands() )
		{
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder/mUserCommands" );
		}
	}
	/*
	// install debugger user command
	if ( dp )
	{
		foreach ( pCommand cmd, dp->userCommands() )
		{
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mDebugger/mUserCommands" );
		}
	}
	*/
	// install interpreter user command
	if ( ip )
	{
		foreach ( pCommand cmd, ip->userCommands() )
		{
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mInterpreter/mUserCommands" );
		}
	}
}

void XUPProjectItem::uninstallCommands()
{
	foreach ( const pCommand& cmd, mCommands.values() )
		emit uninstallCommandRequested( cmd, mCommands.key( cmd ) );
	mCommands.clear();
}
