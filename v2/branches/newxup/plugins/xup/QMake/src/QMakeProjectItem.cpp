#include "QMakeProjectItem.h"
#include "QMake2XUP.h"
#include "QMake.h"
#include "QtVersionManager.h"

#include <pMonkeyStudio.h>
#include <pQueuedMessageToolBar.h>
#include <CLIToolPlugin.h>
#include "PluginsManager.h"
#include "UIMain.h"

#include "UISimpleQMakeEditor.h"

#include <QObject>
#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>
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

QHash<QString, QString> QMakeProjectItem::mVariableLabels;
QHash<QString, QString> QMakeProjectItem::mVariableIcons;

QStringList QMakeProjectItem::mFileVariables = QStringList( "FORMS" ) << "FORMS3" << "HEADERS"
		<< "SOURCES" << "OBJECTIVE_SOURCES" << "YACCSOURCES" << "LEXSOURCES"
		<< "TRANSLATIONS" << "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE" << "SUBDIRS" << "OTHER_FILES";
QStringList QMakeProjectItem::mPathVariables = QStringList( "INCLUDEPATH" ) << "DEPENDPATH"<< "VPATH";
QStringList QMakeProjectItem::mFilteredVariables = QStringList() << "FORMS" << "FORMS3"
		<< "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "YACCSOURCES" << "LEXSOURCES"
		<< "TRANSLATIONS" << "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE"
		<< "INCLUDEPATH" << "DEPENDPATH" << "VPATH" << "LIBS" << "DEFINES" << "OTHER_FILES";


QMakeProjectItem::QMakeProjectItem()
	: XUPProjectItem()
{
	if (mVariableIcons.isEmpty()) // if not initialised
	{
		initHashes();
	}
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

void QMakeProjectItem::initHashes()
{
	mVariableLabels["FORMS"] = tr( "Forms Files" );
	mVariableLabels["FORMS3"] = tr( "Forms 3 Files" );
	mVariableLabels["HEADERS"] = tr( "Headers Files" );
	mVariableLabels["SOURCES"] = tr( "Sources Files" );
	mVariableLabels["OBJECTIVE_SOURCES"] = tr( "Objective Sources Files" );
	mVariableLabels["TRANSLATIONS"] = tr( "Qt Translations Files" );
	mVariableLabels["RESOURCES"] = tr( "Qt Resources Files" );
	mVariableLabels["RC_FILE"] = tr( "Resources Files" );
	mVariableLabels["RES_FILE"] = tr( "Compiled Resources Files" );
	mVariableLabels["DEF_FILE"] = tr( "Definitions Files" );
	mVariableLabels["SUBDIRS"] = tr( "Sub Projects" );
	mVariableLabels["INCLUDEPATH"] = tr( "Includes Paths" );
	mVariableLabels["DEPENDPATH"] = tr( "Depends Paths" );
	mVariableLabels["VPATH"] = tr( "Virtuals Paths" );
	mVariableLabels["LIBS"] = tr( "Libraries Files" );
	mVariableLabels["DEFINES"] = tr( "Defines" );
	mVariableLabels["OTHER_FILES"] = tr( "Other Files" );

	mVariableIcons["FORMS"] = "forms.png";
	mVariableIcons["FORMS3"] = "forms.png";
	mVariableIcons["HEADERS"] = "headers.png";
	mVariableIcons["SOURCES"] = "sources.png";
	mVariableIcons["OBJECTIVE_SOURCES"] = "objective_sources.png";
	mVariableIcons["TRANSLATIONS"] = "translations.png";
	mVariableIcons["RESOURCES"] = "resources.png";
	mVariableIcons["RC_FILE"] = "rc_file.png";
	mVariableIcons["RES_FILE"] = "res_file.png";
	mVariableIcons["DEF_FILE"] = "def_file.png";
	mVariableIcons["SUBDIRS"] = "project.png";
	mVariableIcons["INCLUDEPATH"] = "includepath.png";
	mVariableIcons["DEPENDPATH"] = "dependpath.png";
	mVariableIcons["VPATH"] = "vpath.png";
	mVariableIcons["LIBS"] = "libs.png";
	mVariableIcons["DEFINES"] = "defines.png";
	mVariableIcons["OTHER_FILES"] = "file.png";
}

bool QMakeProjectItem::handleSubdirs( XUPItem* subdirs )
{
	QStringList projects;
	XUPProjectItem* proj = subdirs->project();
	
	foreach ( XUPItem* cit, subdirs->childrenList() )
	{
		if ( cit->type() == XUPItem::File )
		{
			QStringList cacheFns = splitMultiLineValue( interpretContent( cit->content() ) );
			
			foreach ( QString cacheFn, cacheFns )
			{
				if ( cacheFn.isEmpty() )
				{
					continue;
				}
				
				QString fn = filePath( cacheFn ); // content interpreted some lines above
				QFileInfo fi( fn );
				
				if ( cacheFn.endsWith( "/" ) )
				{
					cacheFn.chop( 1 );
				}
				
				int sepPos = cacheFn.lastIndexOf( "/" );
				
				if ( sepPos != -1 )
				{
					cacheFn = cacheFn.mid( sepPos +1 );
				}
				
				if ( fi.isDir() )
				{
					fi.setFile( fn, QString( "%1.pro" ).arg( cacheFn ) );
				}
				
				fn = fi.absoluteFilePath();
				
				if ( !projects.contains( fn ) )
				{
					projects << fn;
				}
			}
		}
	}
	
	foreach ( XUPItem* cit, proj->childrenList() )
	{
		if ( cit->type() == XUPItem::Project )
		{
			if ( projects.contains( cit->project()->fileName() ) )
			{
				projects.removeAll( cit->project()->fileName() );
			}
		}
	}
	
	foreach ( const QString& fn, projects )
	{
		// open project
		XUPProjectItem* project = new QMakeProjectItem();
		proj->addChild( project );
		
		// remove and delete project if can't open
		if ( !project->open( fn, codec() ) )
		{
			proj->removeChild( project );
			topLevelProject()->setLastError( tr( "Failed to handle subdirs file %1" ).arg( fn ) );
			return false;
		}
	}
	
	return true;
}

QString QMakeProjectItem::getVariableContent( const QString& variableName )
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
	// qmake value
	else if ( variableName.startsWith( "$$[" ) )
	{
		QMakeProjectItem* proj = qobject_cast<QMakeProjectItem*>(rootIncludeProject());
		
		if ( proj->mVariableCache.contains( name ) )
		{
			return proj->mVariableCache.value( name );
		}
		
		QString result;
		QtVersionManager* manager = QMake::versionManager();
		const QtVersion version = manager->version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( version.isValid() )
		{
			QProcess query;
			query.start( QString( "%1 -query %2" ).arg( version.qmake() ).arg( name ) );
			query.waitForFinished();
			QString result = QString::fromLocal8Bit( query.readAll() ).trimmed();
			
			if ( result == "**Unknown**" )
			{
				result.clear();
			}
		}
		
		//proj->mVariableCache[ name ] = result;
		return result;
	}
	// variable value
	else
	{
		if ( name == "PWD" )
		{
			return project()->path();
		}
		else if ( name == "_PRO_FILE_" )
		{
			return rootIncludeProject()->fileName();
		}
		else if ( name == "_PRO_FILE_PWD_" )
		{
			return rootIncludeProject()->path();
		}
		else
		{
			return qobject_cast<QMakeProjectItem*>( rootIncludeProject() )->mVariableCache.value( name );
			Q_ASSERT( 0 );
		}
	}
	
	return QString::null;
}

bool QMakeProjectItem::analyze( XUPItem* item )
{
	QStringList values;
	XUPProjectItem* project = item->project();
	QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>( rootIncludeProject() );
	
	foreach ( XUPItem* cItem, item->childrenList() )
	{
		switch ( cItem->type() )
		{
			case XUPItem::Value:
			case XUPItem::File:
			case XUPItem::Path:
			{
				QString content = interpretContent( cItem->content() );
				
				if ( cItem->type() != XUPItem::Value )
				{
					QString fn = project->filePath( content );
					
					if ( QFile::exists( fn ) )
					{
						fn = riProject->relativeFilePath( fn );
					}
					
					content = fn;
				}
				
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
		
		if ( op == "=" )
		{
			riProject->mVariableCache[ name ] = values.join( " " );
		}
		else if ( op == "-=" )
		{
			foreach ( const QString& value, values )
			{
				riProject->mVariableCache[ name ].replace( QRegExp( QString( "\\b%1\\b" ).arg( value ) ), QString::null );
			}
		}
		else if ( op == "+=" )
		{
			riProject->mVariableCache[ name ] += " " +values.join( " " );
		}
		else if ( op == "*=" )
		{
			//if ( !riProject->mVariableCache[ name ].contains( content ) )
			{
				riProject->mVariableCache[ name ] += " " +values.join( " " );
			}
		}
		else if ( op == "~=" )
		{
			topLevelProject()->setLastError( tr( "Don't know how to interpret ~= operator" ) );
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
	
	// handle sub projects
	if ( item->attribute( "name" ) == "SUBDIRS" )
	{
		if ( !handleSubdirs( item ) )
		{
			return false;
		}
	}
	
	return true;
}

void QMakeProjectItem::rebuildCache()
{
	QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>(rootIncludeProject());
	riProject->mVariableCache.clear();
	analyze( riProject );
}

QString QMakeProjectItem::interpretContent( const QString& content )
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

bool QMakeProjectItem::handleIncludeFile( XUPItem* function )
{
	const QString parameters = function->attribute( "parameters" );
	const QString fn = filePath( interpretContent( parameters ) );
	QStringList projects;

	foreach ( XUPItem* cit, function->childrenList() )
	{
		if ( cit->type() == XUPItem::Project )
		{
			projects << cit->project()->fileName();
		}
	}
	
	/*qWarning() << "---" << projects << parameters << fn << function->xmlContent();
	qWarning() << "BOOM";*/

	// check if project is already handled
	if ( projects.contains( fn ) )
	{
		return true;
	}

	// open project
	XUPProjectItem* project = new QMakeProjectItem();
	function->addChild( project );

	// remove and delete project if can't open
	if ( !project->open( fn, codec() ) )
	{
		function->removeChild( project );
		topLevelProject()->setLastError( tr( "Failed to handle include file %1" ).arg( fn ) );
		return false;
	}

	return true;
}

bool QMakeProjectItem::open( const QString& fileName, const QString& codec )
{
	QString buffer = QMake2XUP::convertFromPro( fileName, codec );
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
		topLevelProject()->setLastError("no project node" );
		return false;
	}
	
	// all is ok
	mCodec = codec;
	mFileName = fileName;
	topLevelProject()->setLastError( QString::null );
	
	//qWarning() << mDocument.toString( 4 );
	
	return analyze( this );
}

bool QMakeProjectItem::save()
{
	return XUPProjectItem::save();
}

QString QMakeProjectItem::targetFilePath( bool allowToAskUser, XUPProjectItem::TargetType targetType)
{
	QString targetTypeString;
	
	switch ( targetType )
	{
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
	QString target = tlProject->filePath( interpretContent( projectSettingsValue( key ) ) );
	QFileInfo targetInfo( target );
	
	if ( !targetInfo.exists() || ( !targetInfo.isExecutable() && !QLibrary::isLibrary( target ) ) )
	{
		if ( allowToAskUser )
		{
			QString type;
			
			switch ( targetType )
			{
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
			
			if ( !userTarget.isEmpty() )
			{
				target = userTarget;
			}
			
			if ( targetInfo.exists() )
			{
				setProjectSettingsValue( key, tlProject->relativeFilePath( userTarget ) );
				save();
			}
		}
	}
	
	return target;
}

QStringList QMakeProjectItem::sourceFiles() const
{
/* For PasNox this code is too QMake specific for be in XUPProjectItem. Moved to QMake
   Not all projects has multiline variables, not all multiline variables must
   be splitted, not all projects store source files list in the variables!!
   Not all fileVariables() are source files 
*/
	QStringList files;

	// get all variable that represent files
	foreach ( const QString& variable, fileVariables() )
	{
		const QStringList values = splitMultiLineValue( mVariableCache.value( variable ) );

		foreach ( const QString& value, values )
		{
			const QString file = filePath( value ); // no need content interpretation as its cached values
			const QFileInfo fi( file );

			if ( fi.isDir() )
			{
				continue;
			}

			files << file;
		}
	}
	
	// get dynamic files
	/*XUPItem* dynamicFolderItem = XUPProjectItemHelper::projectDynamicFolderItem( const_cast<XUPProjectItem*>( this ), false );
	
	if ( dynamicFolderItem )
	{
		foreach ( XUPItem* valueItem, dynamicFolderItem->childrenList() )
		{
			if ( valueItem->type() == XUPItem::File )
			{
				files << valueItem->attribute( "content" );
			}
		}
	}*/
	
	return files;
}

void QMakeProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	QHash <QString, QString> varNameForSuffix;
	const QStringList cSuffixtes = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	// HEADERS filters
	foreach ( QString f, cSuffixtes )
		if ( f.startsWith( "*.h", Qt::CaseInsensitive ) )
			varNameForSuffix[f] = "HEADERS";
	// SOURCES filters
	foreach ( QString f, cSuffixtes )
		if ( f.startsWith( "*.c", Qt::CaseInsensitive ) )
			varNameForSuffix[f] = "SOURCES";
	// YACC filters
	foreach ( QString s, cSuffixtes )
		if ( !varNameForSuffix.contains( s.replace( "c", "y", Qt::CaseInsensitive ) ) )
			varNameForSuffix[s.replace( "c", "y", Qt::CaseInsensitive )] = "YACCSOURCES";
	// LEX filters
	QStringList lexSuffixes;
	foreach ( QString s, cSuffixtes )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !varNameForSuffix.contains( s.replace( "c", "l", Qt::CaseInsensitive ) ) )
			varNameForSuffix[s.replace( "c", "l", Qt::CaseInsensitive )] = "LEXSOURCES";
	// PROJECT filters
	varNameForSuffix["*.pro"] = "SUBDIRS";
	varNameForSuffix["*.m"] = "OBJECTIVE_SOURCES";
	varNameForSuffix["*.mm"] = "OBJECTIVE_SOURCES";
	varNameForSuffix["*.pro"] = "SUBDIRS";
	varNameForSuffix["*.ui"] = "FORMS"; // FORMS3 ignored. Let's user edit his pro by text editor. It makes this code simpler
	varNameForSuffix["*.ts"] = "TRANSLATIONS";
	varNameForSuffix["*.qrc"] = "RESOURCES";
	varNameForSuffix["*.def"] = "DEF_FILE";
	varNameForSuffix["*.rc"] = "RC_FILE";
	varNameForSuffix["*.res"] = "RES_FILE";
	
	
	foreach ( const QString& file, files )
	{
		QString varName;
		foreach(QString suff, varNameForSuffix.keys())
		{
			if ( QDir::match( suff, file ) )
			{
				varName = varNameForSuffix[suff];
			}
		}
		if (! varName.isNull())
		{
			XUPItem* variable = getVariable( scope, varName );
			if (NULL == variable)
			{
				variable = scope->addChild( XUPItem::Variable );
				variable->setAttribute( "name", varName );
				
			}
			
			if (variable->attribute( "operator").isNull())
			{
				variable->setAttribute( "operator", "+=" );
			}
			variable->setAttribute( "multiline", "true" );
			XUPItem* value = variable->addChild( XUPItem::File );
			value->setContent( relativeFilePath( file ) );
		}
		else
		{
			setLastError("Don't know how to add file " + file);
			return;
		}
	}
	
	// rebuild cache
	rebuildCache();
	qobject_cast<QMakeProjectItem*>(topLevelProject())->rebuildCache();
}

void QMakeProjectItem::removeItem( XUPItem* item )
{
	XUPItem* variable = item->parent();
	variable->removeChild(item);
	if (0 == variable->childCount())
	{
		variable->parent()->removeChild(variable);
	}
	
	// rebuild cache
	rebuildCache();
	qobject_cast<QMakeProjectItem*>(topLevelProject())->rebuildCache();
}

QStringList QMakeProjectItem::autoActivatePlugins() const
{
	QStringList res;

	QtVersionManager* manager = QMake::versionManager();
	const QtVersion version = manager->version( projectSettingsValue( "QT_VERSION" ) );
	if ( version.isValid() &&
		 version.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ))
	{
		res << "MSVCMake";
	}
	else
	{
		res << "GNUMake";
	}
	
	res << "BeaverDebugger";
	
	return res;
}

void QMakeProjectItem::installCommands()
{
	// get plugins
	CLIToolPlugin* bp = builder();
		
	// config variable
	QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>(rootIncludeProject());
	QStringList config = splitMultiLineValue( riProject->mVariableCache.value( "CONFIG" ) );
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
	const QtVersion version = manager->version( projectSettingsValue( "QT_VERSION" ) );
	
	// evaluate some variables
	QString s;
	s = riProject->mVariableCache.value( "TARGET" );
	
	if ( s.isEmpty() )
	{
		s = QFileInfo( fileName() ).baseName();
	}
	
	const QString target = s;
	s = riProject->mVariableCache.value( "DESTDIR" );
	
	if ( s.isEmpty() )
	{
		s = riProject->mVariableCache.value( "DLLDESTDIR" );
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
			addCommand( cmd, "mBuilder/mBuild" );
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
			addCommand( cmd, "mBuilder/mBuild" );
			cmdBuildRelease = cmd;
		}
		
		// build all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build All" ) );
			cmd.setArguments( "all" );
			addCommand( cmd, "mBuilder/mBuild" );
		}
		
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
			addCommand( cmd, "mBuilder/mClean" );
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
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// clean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean All" ) );
			cmd.setArguments( "clean" );
			addCommand( cmd, "mBuilder/mClean" );
		}

		pCommand cmdDistcleanRelease;
		pCommand cmdDistcleanDebug;
		pCommand cmdDistclean;
		
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
			addCommand( cmd, "mBuilder/mClean" );
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
			addCommand( cmd, "mBuilder/mClean" );
			cmdDistcleanRelease = cmd;
		}
		
		// distclean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean All" ) );
			cmd.setArguments( "distclean" );
			addCommand( cmd, "mBuilder/mClean" );
			cmdDistclean = cmd;
		}
		
		pCommand cmdQmake;
		
		// add qt commands only if possible
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
			addCommand( cmd, "mBuilder" );
			cmdQmake = cmd;
			
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
		
			// rebuild debug
			if ( haveDebug || haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistcleanDebug << cmdQmake << cmdBuildDebug;
				addCommands( "mBuilder/mRebuild", tr( "Rebuild Debug" ), cmds );
			}
			
			// rebuild release
			if ( haveRelease || haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistcleanRelease << cmdQmake << cmdBuildRelease;
				addCommands( "mBuilder/mRebuild", tr( "Rebuild Release" ), cmds );
			}
			
			// rebuild all
			if ( haveDebugRelease )
			{
				pCommandList cmds;
				cmds << cmdDistclean << cmdQmake << cmdBuild;
				addCommands( "mBuilder/mRebuild", tr( "Rebuild All" ), cmds );
			}
		}
		else if ( projectSettingsValue( "SHOW_QT_VERSION_WARNING", "1" ) == "1" )
		{
			setProjectSettingsValue( "SHOW_QT_VERSION_WARNING", "0" );
			topLevelProject()->save();
			MonkeyCore::messageManager()->appendMessage( tr( "Some actions can't be created, because there is no default Qt version setted, please go in your project settings ( %1 ) to fix this." ).arg( displayText() ) );
		}
		
		// execute debug
		if ( haveDebug || haveDebugRelease )
		{
			const QString debugTarget = targetFilePath( false, XUPProjectItem::DebugTarget );
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Debug" ) );
			cmd.setCommand( debugTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QFileInfo( debugTarget ).absolutePath() );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder/mExecute" );
		}
		
		// execute release
		if ( haveRelease || haveDebugRelease )
		{
			const QString releaseTarget = targetFilePath( false, XUPProjectItem::ReleaseTarget );
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Release" ) );
			cmd.setCommand( releaseTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QFileInfo( releaseTarget ).absolutePath() );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder/mExecute" );
		}
		
		if ( !( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease ) )
		{
			const QString defaultTarget = targetFilePath( false, XUPProjectItem::DefaultTarget);
			
			cmd = cmdBuild;
			cmd.setText( tr( "Execute" ) );
			cmd.setCommand( defaultTarget );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( QFileInfo( defaultTarget ).absolutePath() );
			cmd.setParsers( QStringList() );
			cmd.setTryAllParsers( false );
			cmd.setExecutableCheckingEnabled( true );
			addCommand( cmd, "mBuilder/mExecute" );
		}
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

XUPItem* QMakeProjectItem::projectSettingsScope( bool create ) const
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
				return child;
			}
		}

		if ( create )
		{
			XUPItem* scope = project->addChild( XUPItem::Scope, 0 );
			scope->setAttribute( "name", mScopeName );

			XUPItem* emptyline = project->addChild( XUPItem::EmptyLine, 1 );
			emptyline->setAttribute( "count", "1" );

			return scope;
		}
	}

	return 0;
}

QStringList QMakeProjectItem::projectSettingsValues( const QString& variableName, const QStringList& defaultValues ) const
{
	QStringList values;
	XUPProjectItem* project = topLevelProject();

	if ( project )
	{
		XUPItem* scope = projectSettingsScope( false );

		if ( scope )
		{
			XUPItemList variables = getVariables( scope, variableName, false );

			foreach ( const XUPItem* variable, variables )
			{
				foreach ( const XUPItem* child, variable->childrenList() )
				{
					if ( child->type() == XUPItem::Value )
					{
						values << child->content();
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

QString QMakeProjectItem::projectSettingsValue( const QString& variableName, const QString& defaultValue ) const 
{
	const QStringList dvalue = defaultValue.isEmpty() ? QStringList() : QStringList( defaultValue );
	return projectSettingsValues(variableName, dvalue ).join( " " );
}

void QMakeProjectItem::setProjectSettingsValues( const QString& variableName, const QStringList& values )
{
	XUPProjectItem* project = topLevelProject();

	if ( project )
	{
		XUPItem* scope = projectSettingsScope( !values.isEmpty() );

		if ( !scope )
		{
			return;
		}

		XUPItemList variables = getVariables( scope, variableName, false );
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
				QString value = child->content();
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
			valueItem->setContent( value );
		}
	}
}

void QMakeProjectItem::setProjectSettingsValue( const QString& variable, const QString& value ) 
{
	setProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) );
}

void QMakeProjectItem::addProjectSettingsValues( const QString& variableName, const QStringList& values )
{
	XUPProjectItem* project = topLevelProject();

	if ( project )
	{
		XUPItem* scope = projectSettingsScope( !values.isEmpty() );

		if ( !scope )
		{
			return;
		}

		XUPItemList variables = getVariables( scope, variableName, false );
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
				QString value = child->content();
				if ( cleanValues.contains( value ) )
				{
					cleanValues.removeAll( value );
				}
			}
		}

		foreach ( const QString& value, cleanValues )
		{
			XUPItem* valueItem = variable->addChild( XUPItem::Value );
			valueItem->setContent( value );
		}
	}
}

void QMakeProjectItem::addProjectSettingsValue( const QString& variable, const QString& value )
{
	addProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) );
}

QStringList QMakeProjectItem::splitMultiLineValue( const QString& value )
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

bool QMakeProjectItem::edit()
{
	bool ret = UISimpleQMakeEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
	
	if ( ret )
	{
		// rebuild cache
		rebuildCache();
		qobject_cast<QMakeProjectItem*>(topLevelProject())->rebuildCache();
	}
	return ret;
}

CLIToolPlugin* QMakeProjectItem::builder() const
{
	QString name;
	
	QtVersionManager* manager = QMake::versionManager();
	const QtVersion version = manager->version( projectSettingsValue( "QT_VERSION" ) );
	if ( version.isValid() &&
		 version.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ))
	{
		name = "MSVCMake";
	}
	else
	{
		name = "GNUMake";
	}
	
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, name );
}

Pair_String_StringList_List QMakeProjectItem::sourceFileNamePatterns() const
{
	const Pair_String_StringList_List mSuffixes = Pair_String_StringList_List()
		<< qMakePair( tr( "Qt Project" ), QStringList( "*.pro" ) )
		<< qMakePair( tr( "Qt Include Project" ), QStringList( "*.pri" ) );

	const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	// HEADERS filters
	QStringList hf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.h", Qt::CaseInsensitive ) )
			hf << f;
	// SOURCES filters
	QStringList sf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.c", Qt::CaseInsensitive ) )
			sf << f;
	// YACC filters
	QStringList yf;
	foreach ( QString s, sf )
		if ( !yf.contains( s.replace( "c", "y", Qt::CaseInsensitive ) ) )
			yf << s;
	// LEX filters
	QStringList lf;
	foreach ( QString s, sf )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( s.replace( "c", "l", Qt::CaseInsensitive ) ) )
			lf << s;
	// PROJECT filters
	QStringList pjf;
	foreach ( const Pair_String_StringList& p, mSuffixes )
		pjf << p.second;
	// Variable suffixes
	const Pair_String_StringList_List sourceFileNamePatterns = Pair_String_StringList_List()
		<< qMakePair( tr( "Headers" ), hf )
		<< qMakePair( tr( "Sources" ), sf )
		<< qMakePair( tr( "YACC sources" ), yf )
		<< qMakePair( tr( "LEX sources" ), lf )
		<< qMakePair( tr( "Objective sources" ), QStringList( "*.m" ) << "*.mm" )
		<< qMakePair( tr( "Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( tr( "Qt3 Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( tr( "Translations" ), QStringList( "*.ts" ) )
		<< qMakePair( tr( "Resources" ), QStringList( "*.qrc" ) )
		<< qMakePair( tr( "Definition files" ), QStringList( "*.def" ) )
		<< qMakePair( tr( "Resources files" ), QStringList( "*.rc" ) )
		<< qMakePair( tr( "Compiled resources files" ), QStringList( "*.res" ) )
		<< qMakePair( tr( "Projects" ), pjf );
	
	return sourceFileNamePatterns;
}

QStringList QMakeProjectItem::fileVariables()
{
	return mFileVariables;
}

QStringList QMakeProjectItem::pathVariables()
{
	return mPathVariables;
}

QStringList QMakeProjectItem::filteredVariables() const
{
	return mFilteredVariables;
}

QString QMakeProjectItem::iconsPath() const
{
	return ":/qmakeitems";
}

QString QMakeProjectItem::variableDisplayText( const QString& variableName ) const
{
	return mVariableLabels.value( variableName, XUPProjectItem::variableDisplayText( variableName ) );
}

QString QMakeProjectItem::variableDisplayIcon( const QString& variableName ) const
{
	const QString iconFilePath = mVariableIcons.value( variableName );
	
	if ( iconFilePath.isEmpty() ) {
		return XUPProjectItem::variableDisplayIcon( variableName );
	}
	
	return QDir::cleanPath( QString( "%1/%2" ).arg( iconsPath() ).arg( iconFilePath ) );
}
