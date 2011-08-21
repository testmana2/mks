#include "QMakeProjectItem.h"
#include "QtVersionManager.h"
#include "QMake.h"
#include "QMake2XUP.h"
//#include "UISimpleQMakeEditor.h"
#include "editor/UIQMakeEditor.h"

#include <XUPProjectItemHelper.h>
#include <UIMain.h>
#include <CLIToolPlugin.h>
#include <pQueuedMessageToolBar.h>
#include <PluginsManager.h>

#include <pVersion.h>

#include <QProcess>
#include <QLibrary>
#include <QFileDialog>
#include <QDebug>

#if defined( Q_OS_WIN )
	#define PLATFORM_TYPE_STRING "WINDOWS_PLATFORM"
#elif defined( Q_OS_MAC )
	#define PLATFORM_TYPE_STRING "MAC_PLATFORM"
#else
	#define PLATFORM_TYPE_STRING "OTHERS_PLATFORM"
#endif

static QSet<QString> QMakeNativeFunctions = QSet<QString>() // set better for contains test
	<< "basename"
	<< "CONFIG"
	<< "contains"
	<< "count"
	<< "dirname"
	<< "error"
	<< "eval"
	<< "exists"
	<< "find"
	<< "for"
	<< "include"
	<< "infile"
	<< "isEmpty"
	<< "join"
	<< "member"
	<< "message"
	<< "prompt"
	<< "quote"
	<< "replace"
	<< "sprintf"
	<< "system"
	<< "unique"
	<< "warning"
	;

QMakeProjectItemCacheBackend QMakeProjectItem::mCacheBackend = QMakeProjectItemCacheBackend( QMakeProjectItem::cache() );

// QMakeProjectItemCacheBackend

QMakeProjectItemCacheBackend::QMakeProjectItemCacheBackend( XUPProjectItemCache* cache )
	: XUPProjectItemCacheBackend( cache )
{
}

QStringList QMakeProjectItemCacheBackend::guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read content from environment variable when qmake run
		$(QTDIR) : read from generated makefile - Hmm what does that mean ?!
	*/
	
	const QString name = QString( variable ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	
	if ( QMakeNativeFunctions.contains( name ) ) {
		return QStringList( variable );
	}
	
	if ( !mCache ) {
		return QStringList();
	}
	
	XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
	
	// environment variable
	if ( variable.startsWith( "$$(" ) || variable.startsWith( "$(" ) ) {
		if ( name == "PWD" ) {
			const QString path = project->path();
			return path.isEmpty() ? QStringList() : QStringList( path );
		}
		else {
			const QString value = QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
			return value.isEmpty() ? QStringList() : QStringList( value );
		}
	}
	// qmake variable
	else if ( variable.startsWith( "$$[" ) ) {
		// caching result for now, we may disable if needed as user can change qmake variable outside
		if ( cachedData.value( project ).contains( name ) ) {
			return cachedData[ project ][ name ];
		}
		
		const QtVersion version = QMake::versionManager()->version( XUPProjectItemHelper::projectSettingsValue( project, "QT_VERSION" ) );
		QString output;
		
		if ( version.isValid() ) {
			QProcess query;
			query.start( QString( "%1 -query %2" ).arg( version.qmake() ).arg( name ) );
			query.waitForFinished();
			output = QString::fromLocal8Bit( query.readAll() ).trimmed();
			
			if ( output == "**Unknown**" ) {
				output.clear();
			}
		}
		
		// caching result for now, we may disable if needed as user can change qmake variable outside
		cachedData[ project ][ name ] = QStringList( output );
		return QStringList( output );
	}
	// project variable
	else {
		if ( name == "PWD" ) {
			const QString path = variableProject->path();
			return path.isEmpty() ? QStringList() : QStringList( path );
		}
		else if ( name == "_PRO_FILE_" ) {
			const QString fileName = project->fileName();
			return fileName.isEmpty() ? QStringList() : QStringList( fileName );
		}
		else if ( name == "_PRO_FILE_PWD_" ) {
			const QString path = project->path();
			return path.isEmpty() ? QStringList() : QStringList( path );
		}
		else {
			return cachedData.value( project ).value( name );
		}
	}
	
	return QStringList();
}

QStringList QMakeProjectItemCacheBackend::guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const
{
	if ( !mCache ) {
		return QStringList();
	}
	
	//const QRegExp rx( "\\$\\$?[\\{\\(\\[]?([\\w\\.]+(?!\\w*\\s*\\{\\[\\(\\)\\]\\}))[\\]\\)\\}]?" );
	const QRegExp rx( "(?:[^$]|^)(\\${1,2}(?!\\$+)[{(\\[]?[\\w._]+[})\\]]?)" );
	XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
	QString loopContent = content.join( " " );
	QStringList guessed = content;
	int pos = 0;

	while ( ( pos = rx.indexIn( loopContent, pos ) ) != -1 ) {
		const QString capture = rx.cap( 1 );
		const QString guessedVariableContent = guessedVariable( project, valueProject, capture ).join( " " );
		loopContent.replace( capture, guessedVariableContent );
		guessed.replaceInStrings( capture, guessedVariableContent );
		pos += guessedVariableContent.length();
	}
	
#ifndef QT_NO_DEBUG
	QString s = guessed.join( " " );
	
	foreach ( const QString& f, QMakeNativeFunctions ) {
		s.replace( QString( "$$%1" ).arg( f ), QString::null );
	}
	
	if ( s.contains( "$" ) ) {
		qWarning() << "Failed guessing";
		qWarning() << content;
		qWarning() << loopContent;
		qWarning() << guessed;
		qWarning() << cachedData.value( project );
		Q_ASSERT( 0 );
	}
#endif

	return guessed;
}

void QMakeProjectItemCacheBackend::updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op )
{
	if ( !mCache ) {
		return;
	}
	
	XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
	
	if ( op == "=" || op.isEmpty() ) {
		cachedData[ project ][ variable ] = values;
	}
	else if ( op == "-=" ) {
		const DocumentFilterMap& filter = project->documentFilters();
		const QStringList opValue = filter.splitValue( values.join( " " ) ).toSet().toList();
		
		foreach ( const QString& value, opValue ) {
			cachedData[ project ][ variable ].replaceInStrings( QRegExp( QString( "\\b%1\\b" ).arg( value ) ), QString::null );
		}
	}
	else if ( op == "+=" ) {
		cachedData[ project ][ variable ] << values;
	}
	else if ( op == "*=" ) {
		const DocumentFilterMap& filter = project->documentFilters();
		QSet<QString> currentValues = filter.splitValue( cachedData[ project ][ variable ].join( " " ) ).toSet();
		
		foreach ( const QString& value, values ) {
			const QStringList newValues = filter.splitValue( value ).toSet().toList();
			QStringList parts;
			
			foreach ( const QString& value, newValues ) {
				if ( !currentValues.contains( value ) ) {
					currentValues << value;
					parts << value;
				}
			}
			
			if ( !parts.isEmpty() ) {
				cachedData[ project ][ variable ] << parts.join( " " );
			}
		}
	}
	else if ( op == "~=" ) {
		project->topLevelProject()->setLastError( XUPProjectItem::tr( "Don't know how to interpret ~= operator" ) );
	}
}

bool QMakeProjectItemCacheBackend::cacheRecursiveScanHook( XUPProjectItem* _project, XUPItem* item )
{
	QMakeProjectItem* project = qobject_cast<QMakeProjectItem*>( _project );
	
	if ( !project ) {
		return false;
	}
	
    bool changed = false;
    
    // handle include projects
	if ( item->type() == XUPItem::Function && item->attribute( "name" ).toLower() == "include" ) {
		if ( project->handleIncludeFile( item ) ) {
            changed = true;
        }
	}
	
	// handle sub projects
	if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == "SUBDIRS" ) {
		if ( project->handleSubdirs( item ) ) {
            changed = true;
        }
	}
    
    return changed;
}

// QMakeProjectItem

QMakeProjectItem::QMakeProjectItem()
	: XUPProjectItem()
{
}

QMakeProjectItem::~QMakeProjectItem()
{
}

QString QMakeProjectItem::toNativeString() const
{
	return QMake2XUP::convertToPro( mDocument );
}

QString QMakeProjectItem::projectType() const
{
	return PLUGIN_NAME;
}

bool QMakeProjectItem::open( const QString& fileName, const QString& codec )
{
	QString buffer = QMake2XUP::convertFromPro( fileName, codec );
    
	// parse content
	QString errorMsg;
	int errorLine;
	int errorColumn;
    
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) ) {
		topLevelProject()->setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
    
	if ( mDomElement.isNull() ) {
		topLevelProject()->setLastError("no project node" );
		return false;
	}
	
	// check xup version
	const QString docVersion = mDomElement.attribute( "version" );
    
	if ( pVersion( docVersion ) < XUP_VERSION ) { 
		topLevelProject()->setLastError( tr( "The document format is too old, current version is '%1', your document is '%2'" ).arg( XUP_VERSION ).arg( docVersion ) );
		return false;
	}
	
	// all is ok
	mCodec = codec;
	mFileName = fileName;
	topLevelProject()->setLastError( QString::null );
	
	const XUPItem* parent = XUPItem::parent();
	const bool isIncludeProject = parent && parent->type() == XUPItem::Function && parent->attribute( "name" ) == "include";
	
	if ( isIncludeProject ) {
		XUPProjectItem::cache()->update( parent->project()->rootIncludeProject(), this );
	}
	else {
		XUPProjectItem::cache()->build( this );
	}
	
    return true;
}

QString QMakeProjectItem::targetFilePath( bool allowToAskUser, XUPProjectItem::TargetType targetType)
{
	QString targetTypeString;
	
	switch ( targetType ) {
		case XUPProjectItem::DefaultTarget:
			targetTypeString = QLatin1String( "TARGET_DEFAULT" );
			break;
		case XUPProjectItem::DebugTarget:
			targetTypeString = QLatin1String( "TARGET_DEBUG" );
			break;
		case XUPProjectItem::ReleaseTarget:
			targetTypeString = QLatin1String( "TARGET_RELEASE" );
			break;
	}

	XUPProjectItem* tlProject = topLevelProject();
	const QString key = QString( "%1_%2" ).arg( PLATFORM_TYPE_STRING ).arg( targetTypeString );
	QString target = tlProject->filePath( XUPProjectItemHelper::projectSettingsValue( this, key ) );
	QFileInfo targetInfo( target );
	
	if ( !targetInfo.exists() || ( !targetInfo.isExecutable() && !QLibrary::isLibrary( target ) ) ) {
		if ( allowToAskUser ) {
			QString type;
			
			switch ( targetType ) {
				case XUPProjectItem::DebugTarget:
					type = tr( "debug" ) +" ";
					break;
				case XUPProjectItem::ReleaseTarget:
					type = tr( "release" ) +" ";
					break;
				default:
					break;
			}
			
			const QString userTarget = QFileDialog::getOpenFileName( MonkeyCore::mainWindow(), tr( "Point please project %1 target" ).arg( type ), tlProject->path() );
			targetInfo.setFile( userTarget );
			
			if ( !userTarget.isEmpty() ) {
				target = userTarget;
			}
			
			if ( targetInfo.exists() ) {
				XUPProjectItemHelper::setProjectSettingsValue( this, key, tlProject->relativeFilePath( userTarget ) );
				save();
			}
		}
	}
	
	return target;
}

bool QMakeProjectItem::handleIncludeFile( XUPItem* function )
{
	XUPProjectItem* project = function->project();
	const QString filePath = project->filePath( function->cacheValue( "parameters" ) );
	QStringList projects;
	
	// search already handled include project
	foreach ( XUPItem* child, function->childrenList() ) {
		if ( child->type() == XUPItem::Project ) {
			projects << child->project()->fileName();
		}
	}
	
	// check if project is already handled
	if ( projects.contains( filePath ) ) {
		return false;
	}

	// open project
	XUPProjectItem* includeProject = new QMakeProjectItem();
	function->addChild( includeProject );

	// remove and delete project if can't open
	if ( !includeProject->open( filePath, project->codec() ) ) {
		function->removeChild( includeProject );
		topLevelProject()->setLastError( tr( "Failed to handle include file %1" ).arg( filePath ) );
        return false;
	}
    
    return true;
}

bool QMakeProjectItem::handleSubdirs( XUPItem* subdirs )
{
	XUPProjectItem* project = subdirs->project();
	const DocumentFilterMap& filter = project->documentFilters();
	QStringList projects;
	bool created = false;
	
	// search all sub project to handle
	foreach ( XUPItem* child, subdirs->childrenList() ) {
		if ( child->type() == XUPItem::File ) {
			QStringList cacheFns = filter.splitValue( child->cacheValue( "content" ) );
			
			foreach ( const QString& cacheFn, cacheFns ) {
				if ( cacheFn.isEmpty() ) {
					continue;
				}
				
				QFileInfo file( filePath( cacheFn ) );
				
				if ( file.isDir() ) {
					file.setFile( QString( "%1/%2.pro" ).arg( file.absoluteFilePath() ).arg( file.fileName() ) );
				}
				
				const QString filePath = QDir::cleanPath( QDir::toNativeSeparators( file.absoluteFilePath() ) );
				
				if ( !projects.contains( filePath ) ) {
					projects << filePath;
				}
			}
		}
	}
	
	// remove already handled sub projects
	foreach ( XUPItem* child, project->childrenList() ) {
		if ( child->type() == XUPItem::Project ) {
			const QString filePath = QDir::cleanPath( QDir::toNativeSeparators( child->project()->fileName() ) );
			
			if ( projects.contains( filePath ) ) {
				projects.removeAll( filePath );
			}
		}
	}
	
	// open missing sub projects
	foreach ( const QString& filePath, projects ) {
		// open project
		XUPProjectItem* subProject = new QMakeProjectItem();
		project->addChild( subProject );
		
		// remove and delete project if can't open
		if ( subProject->open( filePath, project->codec() ) ) {
			created = true;
		}
		else {
			project->removeChild( subProject );
			topLevelProject()->setLastError( tr( "Failed to handle subdirs file %1" ).arg( filePath ) );
			continue;
		}
	}
	
	return created;
}

void QMakeProjectItem::installCommands()
{
	// get plugins
	CLIToolPlugin* bp = builder();
    
	// config variable
	//QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>( rootIncludeProject() );
	QStringList config = documentFilters().splitValue( cachedVariableValue( "CONFIG" ) );
	bool haveDebug = config.contains( "debug" );
	bool haveRelease = config.contains( "release" );
	bool haveDebugRelease = config.contains( "debug_and_release" );
	
	// temp command
	pCommand cmd;
	
	// build command
	if ( bp )
	{
		cmd = bp->command();
	}
	cmd.setProject( this );
	cmd.setWorkingDirectory( path() );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	cmd.setExecutableCheckingEnabled( false );
	const pCommand cmdBuild = cmd;
	
	// get qt version
	QtVersionManager* manager = QMake::versionManager();
	const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( this, "QT_VERSION" ) );
	
	// evaluate some variables
	QString s;
	s = cachedVariableValue( "TARGET" );
	
	if ( s.isEmpty() )
	{
		s = QFileInfo( fileName() ).baseName();
	}
	
	const QString target = s;
	s = cachedVariableValue( "DESTDIR" );
	
	if ( s.isEmpty() )
	{
		s = cachedVariableValue( "DLLDESTDIR" );
	}
	
	if ( QDir( s ).isRelative() )
	{
		s.prepend( "$cpp$/" );
	}
	
	if ( s.endsWith( '/' ) )
	{
		s.chop( 1 );
	}
	
	const QString destdir = s;
	
	pCommand cmdBuildDebug;
	pCommand cmdBuildRelease;
	
	// compiler
	if ( bp && cmdBuild.isValid() )
	{
		// build debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug" );
			}
			addCommand( cmd, "mBuilder" );
			cmdBuildDebug = cmd;
		}
		
		// build release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release" );
			}
			addCommand( cmd, "mBuilder" );
			cmdBuildRelease = cmd;
		}
		
		// build all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build All" ) );
			cmd.setArguments( "all" );
			addCommand( cmd, "mBuilder" );
		}
		
		addSeparator(  "mBuilder" );
		
		// clean debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug-clean" );
			}
			else
			{
				cmd.setArguments( "clean" );
			}
			addCommand( cmd, "mBuilder" );
		}
		
		// clean release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release-clean" );
			}
			else
			{
				cmd.setArguments( "clean" );
			}
			addCommand( cmd, "mBuilder" );
		}
		
		// clean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean All" ) );
			cmd.setArguments( "clean" );
			addCommand( cmd, "mBuilder" );
		}

		pCommand cmdDistcleanRelease;
		pCommand cmdDistcleanDebug;
		pCommand cmdDistclean;
		pCommand cmdQmake;
		
		// distclean debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug-distclean" );
			}
			else
			{
				cmd.setArguments( "distclean" );
			}
			addCommand( cmd, "mBuilder" );
			cmdDistcleanDebug = cmd;
		}
		
		// distclean release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release-distclean" );
			}
			else
			{
				cmd.setArguments( "distclean" );
			}
			addCommand( cmd, "mBuilder" );
			cmdDistcleanRelease = cmd;
		}
		
		// distclean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean All" ) );
			cmd.setArguments( "distclean" );
			addCommand( cmd, "mBuilder" );
			cmdDistclean = cmd;
		}
		
		addSeparator(  "mBuilder" );
		
		if ( version.isValid() )
		{
			// qmake command
			cmd = pCommand();
			cmd.setText( tr( "QMake" ) );
			cmd.setCommand( version.qmake() );
			cmd.setArguments( version.qmakeParameters() );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			cmdQmake = cmd;
			
			// rebuild debug
			if ( haveDebug || haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistcleanDebug << cmdQmake << cmdBuildDebug;
				addCommands( "mBuilder", tr( "Rebuild Debug" ), cmds );
			}
			
			// rebuild release
			if ( haveRelease || haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistcleanRelease << cmdQmake << cmdBuildRelease;
				addCommands( "mBuilder", tr( "Rebuild Release" ), cmds );
			}
			
			// rebuild all
			if ( haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistclean << cmdQmake << cmdBuild;
				addCommands( "mBuilder", tr( "Rebuild All" ), cmds );
			}
		}
		
		addSeparator(  "mBuilder" );
		
		// execute debug
		if ( haveDebug || haveDebugRelease )
		{
			const QString debugTarget = targetFilePath( false, XUPProjectItem::DebugTarget );
			const QString debugPath = debugTarget.isEmpty() ? path() : QFileInfo( debugTarget ).absolutePath();
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Debug" ) );
			cmd.setCommand( debugTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( debugPath );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder" );
		}
		
		// execute release
		if ( haveRelease || haveDebugRelease )
		{
			const QString releaseTarget = targetFilePath( false, XUPProjectItem::ReleaseTarget );
			const QString releasePath = releaseTarget.isEmpty() ? path() : QFileInfo( releaseTarget ).absolutePath();
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Release" ) );
			cmd.setCommand( releaseTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( releasePath );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder" );
		}
		
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) )
		{
			const QString defaultTarget = targetFilePath( false, XUPProjectItem::DefaultTarget );
			const QString defaultPath = defaultTarget.isEmpty() ? path() : QFileInfo( defaultTarget ).absolutePath();
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute" ) );
			cmd.setCommand( defaultTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( defaultPath );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder" );
		}
		
		addSeparator(  "mBuilder" );
		
		// add qt commands only if possible
		if ( version.isValid() )
		{
			// qmake command
			addCommand( cmdQmake, "mBuilder" );
			
			// lupdate command
			cmd = pCommand();
			cmd.setText( tr( "lupdate" ) );
			cmd.setCommand( version.lupdate() );
			cmd.setArguments( "\"$cp$\"" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder" );
			
			// lrelease command
			cmd = pCommand();
			cmd.setText( tr( "lrelease" ) );
			cmd.setCommand( version.lrelease() );
			cmd.setArguments( "\"$cp$\"" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder" );
			
			addSeparator(  "mBuilder" );
		}
		else if ( XUPProjectItemHelper::projectSettingsValue( this, "SHOW_QT_VERSION_WARNING", "1" ) == "1" )
		{
			XUPProjectItemHelper::setProjectSettingsValue( this, "SHOW_QT_VERSION_WARNING", "0" );
			topLevelProject()->save();
			MonkeyCore::messageManager()->appendMessage( tr( "Some actions can't be created, because there is no default Qt version setted, please go in your project settings ( %1 ) to fix this." ).arg( displayText() ) );
		}
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

XUPProjectItemCacheBackend* QMakeProjectItem::cacheBackend() const
{
	return &QMakeProjectItem::mCacheBackend;
}

bool QMakeProjectItem::edit()
{
	/*if ( UISimpleQMakeEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted ) {
        XUPProjectItem::cache()->build( rootIncludeProject() );
        return true;
	}
    
	return false;*/
	
	UIQMakeEditor dlg( MonkeyCore::mainWindow() );
	dlg.setupProject( this );
	return dlg.exec() == QDialog::Accepted;
}

bool QMakeProjectItem::editProjectFiles()
{
	/*UIQMakeEditor dlg( MonkeyCore::mainWindow() );
	dlg.setupProject( this );
	dlg.showProjectFilesPage();
	return dlg.exec() == QDialog::Accepted;*/
	return edit();
}

CLIToolPlugin* QMakeProjectItem::builder() const
{
    const QtVersionManager* manager = QMake::versionManager();
    const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( const_cast<QMakeProjectItem*>( this ), "QT_VERSION" ) );
	const QString name = version.isValid() && version.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) ? "MSVCMake" : "GNUMake";
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, name );
}
