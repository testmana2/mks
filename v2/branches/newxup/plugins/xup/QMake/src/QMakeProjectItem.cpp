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
		//Q_ASSERT( 0 );
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
		project->showError( QMakeProjectItem::tr( "Don't know how to interpret ~= operator" ) );
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

void QMakeProjectItem::removeValue( XUPItem* item, bool deleteFiles )
{
	switch ( item->type() ) {
		case XUPItem::Variable: {
			if ( item->attribute( "name" ) == "SUBDIRS" ) {
				item->setCacheValue( "markDeleted", "1" ); // see XUPProjectItem::removeValue
				
				foreach ( XUPItem* value, item->childrenList() ) {
					removeValue( value, false );
				}
			}
			
			break;
		}
		case XUPItem::File: {
			if ( item->parent()->attribute( "name" ) == "SUBDIRS" ) {
				XUPProjectItem* project = item->project();
				const DocumentFilterMap& filter = project->documentFilters();
				QStringList cacheFns = filter.splitValue( item->cacheValue( "content" ) );
				QSet<QString> projects;
				
				foreach ( const QString& cacheFn, cacheFns ) {
					const QString filePath = guessSubProjectFilePath( cacheFn );
					
					if ( cacheFn.isEmpty() ) {
						continue;
					}
					
					if ( !projects.contains( filePath ) ) {
						projects << filePath;
					}
				}
				
				foreach ( XUPProjectItem* proj, project->childrenProjects( false ) ) {
					const QString projectFilePath = QDir::cleanPath( QDir::toNativeSeparators( proj->fileName() ) );
					
					if ( projects.contains( projectFilePath ) ) {
						projects.remove( projectFilePath );
						project->removeChild( proj );
					}
				}
			}
			
			break;
		}
		default:
			break;
	}
	
	XUPProjectItem::removeValue( item, deleteFiles );
}

QString QMakeProjectItem::quoteString() const
{
	return "\"";
}

QString QMakeProjectItem::defaultOperator() const
{
	return "*=";
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
		showError( tr( "Xml error in '%1':\n%2 on line %3, column %4" )
			.arg( fileName )
			.arg( errorMsg )
			.arg( errorLine )
			.arg( errorColumn )
		);
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
    
	if ( mDomElement.isNull() ) {
		showError( tr( "Invalid project: no project node in '%1'" ).arg( fileName ) );
		return false;
	}
	
	// check xup version
	const QString docVersion = mDomElement.attribute( "version" );
    
	if ( pVersion( docVersion ) < XUP_VERSION ) { 
		showError( tr( "The document format is too old, current version is '%1', your document is '%2' in '%3'" )
			.arg( XUP_VERSION )
			.arg( docVersion )
			.arg( fileName )
		);
		return false;
	}
	
	// all is ok
	mCodec = codec;
	mFileName = fileName;
	
	const XUPItem* parent = XUPItem::parent();
	const bool isIncludeProject = parent && parent->type() == XUPItem::Function && parent->attribute( "name" ) == "include";
	
	if ( isIncludeProject ) {
		XUPProjectItem::cache()->update( parent->project()->rootIncludeProject(), this );
	}
	else {
		XUPProjectItem::cache()->build( this );
	}
	
	emitDataChanged();
    return true;
}

QString QMakeProjectItem::targetFilePath( XUPProjectItem::TargetType targetType )
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
		default:
			return QString::null;
	}

	XUPProjectItem* tlProject = topLevelProject();
	const QString key = QString( "%1_%2" ).arg( PLATFORM_TYPE_STRING ).arg( targetTypeString );
	QString target = tlProject->filePath( XUPProjectItemHelper::projectSettingsValue( tlProject, key ) );
	QFileInfo targetInfo( target );
	
	if ( !targetInfo.exists() || ( !targetInfo.isExecutable() && !QLibrary::isLibrary( target ) ) ) {
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
		
		const QString userTarget = QFileDialog::getOpenFileName( MonkeyCore::mainWindow(), tr( "Point please project %1target" ).arg( type ), tlProject->path() );
		targetInfo.setFile( userTarget );
		
		if ( !userTarget.isEmpty() ) {
			target = userTarget;
		}
		
		if ( targetInfo.exists() ) {
			XUPProjectItemHelper::setProjectSettingsValue( tlProject, key, tlProject->relativeFilePath( userTarget ) );
			tlProject->save();
		}
	}
	
	return target;
}

QString QMakeProjectItem::guessSubProjectFilePath( const QString& subdirsValue ) const
{
	if ( subdirsValue.isEmpty() ) {
		return QString::null;
	}
	
	QFileInfo file( filePath( subdirsValue ) );
	
	if ( file.isDir() ) {
		QDir dir( file.absoluteFilePath() );
		const QString mask = QString( "%1.pro" ).arg( file.fileName() );
		const QFileInfoList files = pMonkeyStudio::getFiles( dir, QStringList( mask ), false );
		file.setFile( files.value( 0 ).absoluteFilePath() );
	}
	
	return QDir::cleanPath( QDir::toNativeSeparators( file.absoluteFilePath() ) );
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
		showError( tr( "Failed to handle include file '%1'" ).arg( filePath ) );
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
				const QString filePath = guessSubProjectFilePath( cacheFn );
				
				if ( cacheFn.isEmpty() ) {
					continue;
				}
				
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
			showError( tr( "Failed to handle subdirs file '%1'" ).arg( filePath ) );
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
	QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>( rootIncludeProject() );
	QMakeProjectItem* tlProject = qobject_cast<QMakeProjectItem*>( topLevelProject() );
	const QStringList config = documentFilters().splitValue( riProject->cachedVariableValue( "CONFIG" ) );
	const bool haveDebug = config.contains( "debug" );
	const bool haveRelease = config.contains( "release" );
	const bool haveDebugRelease = config.contains( "debug_and_release" );
	// get qt version
	const QtVersionManager* manager = QMake::versionManager();
	const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( tlProject, "QT_VERSION" ) );
	const bool showQtWarning = XUPProjectItemHelper::projectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "1" ) == "1";
	// temp variables
	pCommand cmd;
	QString s;
	
	// build command
	if ( bp ) {
		cmd = bp->command();
	}
	
	cmd.setProject( this );
	cmd.setWorkingDirectory( path() );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	cmd.setExecutableCheckingType( XUPProjectItem::NoTarget );
	
	// base command
	const pCommand cmdBuild = cmd;
	
	// evaluate some variables
	s = cachedVariableValue( "TARGET" );
	
	if ( s.isEmpty() ) {
		s = QFileInfo( fileName() ).baseName();
	}
	
	const QString target = s;
	
	s = cachedVariableValue( "DESTDIR" );
	
	if ( s.isEmpty() ) {
		s = cachedVariableValue( "DLLDESTDIR" );
	}
	
	if ( QDir( s ).isRelative() ) {
		s.prepend( "$cpp$/" );
	}
	
	if ( s.endsWith( '/' ) ) {
		s.chop( 1 );
	}
	
	const QString destdir = s;
	
	pCommand cmdBuildDebug;
	pCommand cmdBuildRelease;
	
	// compiler
	if ( bp && cmdBuild.isValid() ) {
		// build debug
		if ( haveDebug || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Build Debug" ) );
			if ( haveDebugRelease ) {
				cmd.setArguments( "debug" );
			}
			addCommand( "mBuilder", cmd );
			cmdBuildDebug = cmd;
		}
		
		// build release
		if ( haveRelease || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Build Release" ) );
			if ( haveDebugRelease ) {
				cmd.setArguments( "release" );
			}
			addCommand( "mBuilder", cmd );
			cmdBuildRelease = cmd;
		}
		
		// build all
		if ( haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Build All" ) );
			cmd.setArguments( "all" );
			addCommand( "mBuilder", cmd );
		}
		
		// default build
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Build" ) );
			addCommand( "mBuilder", cmd );
		}
		
		addSeparator(  "mBuilder" );
		
		// clean debug
		if ( haveDebug || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Debug" ) );
			if ( haveDebugRelease ) {
				cmd.setArguments( "debug-clean" );
			}
			else {
				cmd.setArguments( "clean" );
			}
			addCommand( "mBuilder", cmd );
		}
		
		// clean release
		if ( haveRelease || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Release" ) );
			if ( haveDebugRelease ) {
				cmd.setArguments( "release-clean" );
			}
			else {
				cmd.setArguments( "clean" );
			}
			addCommand( "mBuilder", cmd );
		}
		
		// clean all
		if ( haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Clean All" ) );
			cmd.setArguments( "clean" );
			addCommand( "mBuilder", cmd );
		}
		
		// default clean
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Clean" ) );
			cmd.setArguments( "clean" );
			addCommand( "mBuilder", cmd );
		}

		pCommand cmdDistcleanRelease;
		pCommand cmdDistcleanDebug;
		pCommand cmdDistclean;
		pCommand cmdQmake;
		
		// distclean debug
		if ( haveDebug || haveDebugRelease ) {
			cmdDistcleanDebug = cmdBuild;
			cmdDistcleanDebug.setText( tr( "Distclean Debug" ) );
			if ( haveDebugRelease ) {
				cmdDistcleanDebug.setArguments( "debug-distclean" );
			}
			else {
				cmdDistcleanDebug.setArguments( "distclean" );
			}
			addCommand( "mBuilder", cmdDistcleanDebug );
		}
		
		// distclean release
		if ( haveRelease || haveDebugRelease ) {
			cmdDistcleanRelease = cmdBuild;
			cmdDistcleanRelease.setText( tr( "Distclean Release" ) );
			if ( haveDebugRelease ) {
				cmdDistcleanRelease.setArguments( "release-distclean" );
			}
			else {
				cmdDistcleanRelease.setArguments( "distclean" );
			}
			addCommand( "mBuilder", cmdDistcleanRelease );
		}
		
		// distclean all
		if ( haveDebugRelease ) {
			cmdDistclean = cmdBuild;
			cmdDistclean.setText( tr( "Distclean All" ) );
			cmdDistclean.setArguments( "distclean" );
			addCommand( "mBuilder", cmdDistclean );
		}
		
		// default distclean
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean" ) );
			cmd.setArguments( "distclean" );
			addCommand( "mBuilder", cmd );
		}
		
		addSeparator(  "mBuilder" );
		
		if ( version.isValid() ) {
			// qmake command
			cmdQmake = pCommand();
			cmdQmake.setText( tr( "QMake" ) );
			cmdQmake.setCommand( version.qmake() );
			cmdQmake.setArguments( version.qmakeParameters() );
			cmdQmake.setWorkingDirectory( "$cpp$" );
			cmdQmake.setProject( this );
			cmdQmake.setSkipOnError( false );
			cmdQmake.addParser( "QMake" );
			
			// rebuild debug
			if ( haveDebug || haveDebugRelease ) {
				cmd = pCommand( tr( "Rebuild Debug" ) );
				cmd.addChildCommand( cmdDistcleanDebug );
				cmd.addChildCommand( cmdQmake );
				cmd.addChildCommand( cmdBuildDebug );
				addCommand( "mBuilder", cmd );
			}
			
			// rebuild release
			if ( haveRelease || haveDebugRelease ) {
				cmd = pCommand( tr( "Rebuild Release" ) );
				cmd.addChildCommand( cmdDistcleanRelease );
				cmd.addChildCommand( cmdQmake );
				cmd.addChildCommand( cmdBuildRelease );
				addCommand( "mBuilder", cmd );
			}
			
			// rebuild all
			if ( haveDebugRelease ) {
				cmd = pCommand( tr( "Rebuild All" ) );
				cmd.addChildCommand( cmdDistclean );
				cmd.addChildCommand( cmdQmake );
				cmd.addChildCommand( cmdBuild );
				addCommand( "mBuilder", cmd );
			}
		}
		
		addSeparator(  "mBuilder" );
		
		// execute debug
		if ( haveDebug || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Debug" ) );
			cmd.setCommand( "$target$" );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QString::null);
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingType( XUPProjectItem::DebugTarget );
			addCommand( "mBuilder", cmd );
		}
		
		// execute release
		if ( haveRelease || haveDebugRelease ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Release" ) );
			cmd.setCommand( "$target$" );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QString::null );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingType( XUPProjectItem::ReleaseTarget );
			addCommand( "mBuilder", cmd );
		}
		
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) ) {
			cmd = cmdBuild;
			cmd.setText( tr( "Execute" ) );
			cmd.setCommand( "$target$" );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QString::null );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingType( XUPProjectItem::DefaultTarget );
			addCommand( "mBuilder", cmd );
		}
		
		addSeparator(  "mBuilder" );
		
		// add qt commands only if possible
		if ( version.isValid() ) {
			// qmake command
			addCommand( "mBuilder", cmdQmake );
			
			// lupdate command
			cmd = pCommand();
			cmd.setText( tr( "lupdate" ) );
			cmd.setCommand( version.lupdate() );
			cmd.setArguments( "\"$cp$\"" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( "mBuilder", cmd );
			
			// lrelease command
			cmd = pCommand();
			cmd.setText( tr( "lrelease" ) );
			cmd.setCommand( version.lrelease() );
			cmd.setArguments( "\"$cp$\"" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( "mBuilder", cmd );
			
			addSeparator(  "mBuilder" );
		}
		else if ( showQtWarning ) {
			XUPProjectItemHelper::setProjectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "0" );
			tlProject->save();
			MonkeyCore::messageManager()->appendMessage( tr( "Qt was not found, some actions have been disabled.\n"
				"Edit the QMake plugin configuration to fix the problem." ) );
		}
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

XUPProjectItemCacheBackend* QMakeProjectItem::cacheBackend() const
{
	return &QMakeProjectItem::mCacheBackend;
}

UIXUPEditor* QMakeProjectItem::newEditDialog() const
{
	return new UIQMakeEditor( MonkeyCore::mainWindow() );
}

CLIToolPlugin* QMakeProjectItem::builder() const
{
	XUPProjectItem* tlProject = topLevelProject();
    const QtVersionManager* manager = QMake::versionManager();
    const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( tlProject, "QT_VERSION" ) );
	const QString name = version.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) ? "MSVCMake" : "GNUMake";
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, name );
}
