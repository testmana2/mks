#include "XUPProjectItem.h"
#include "XUPProjectModel.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "UIMain.h"
#include "XUPPlugin.h"
#include "XUPProjectItemHelper.h"
#include "XUPDynamicFolderItem.h"
#include "XUPProjectItemCache.h"
#include "pFileManager.h"

#include <pQueuedMessageToolBar.h>

#include <QTextCodec>
#include <QDir>
#include <QRegExp>
#include <QProcess>
#include <QFileDialog>
#include <QLibrary>
#include <QMessageBox>

#include <pVersion.h>

#include <QDebug>

XUPProjectItemCache XUPProjectItem::mProjectsCache;
XUPProjectItemCacheBackend XUPProjectItem::mProjectsCacheBackend = XUPProjectItemCacheBackend( &XUPProjectItem::mProjectsCache );

XUPProjectItem::XUPProjectItem()
	: XUPItem( QDomElement(), 0 )
{
}

XUPProjectItem::~XUPProjectItem()
{
	XUPProjectItem::cache()->clear( this );
}

void XUPProjectItem::setLastError( const QString& error )
{
	mLastError = error;
	
	if ( !error.trimmed().isEmpty() ) {
		MonkeyCore::messageManager()->appendMessage( error );
	}
}

QString XUPProjectItem::lastError() const
{
	return mLastError;
}

QString XUPProjectItem::fileName() const
{
	return mFileName;
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

QStringList XUPProjectItem::sourceFiles() const
{
	QSet<QString> entries;
	
	foreach ( const QString& name, documentFilters().fileVariables() ) {
		XUPItemList variables = getVariables( this, name );
		
		foreach ( XUPItem* variable, variables ) {
			foreach( XUPItem* item, variable->childrenList() ) {
				if ( item->type() == XUPItem::File ) {
					entries << filePath( item->cacheValue( "content" ) );
				}
			}
		}
	}
	
	// get dynamic files
	const XUPDynamicFolderSettings settings = XUPProjectItemHelper::projectDynamicFolderSettings( const_cast<XUPProjectItem*>( this ) );
	
	if ( settings.Active && !settings.AbsolutePath.isEmpty() && QFile::exists( settings.AbsolutePath ) ) {
		QDir dir( settings.AbsolutePath );
		QFileInfoList files = pMonkeyStudio::getFiles( dir, settings.FilesPatterns, true );
		
		foreach ( const QFileInfo& fi, files ) {
			entries << fi.absoluteFilePath();
		}
	}
	
	return entries.toList();
}

QStringList XUPProjectItem::topLevelProjectSourceFiles() const
{
	QStringList files;

	foreach ( XUPProjectItem* project, childrenProjects( true ) ) {
		files << project->sourceFiles();
	}

	// remove duplicates
	return files.toSet().toList();
}

XUPPlugin* XUPProjectItem::driver() const
{
	return MonkeyCore::pluginsManager()->plugin<XUPPlugin*>( PluginsManager::stAll, projectType() );
}

QStringList XUPProjectItem::autoActivatePlugins() const
{
	return driver()->infos().dependencies;
}

void XUPProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	const DocumentFilterMap& filters = documentFilters();
	QStringList notImported;
	XUPProjectItem* project = scope ? scope->project() : this;
	XUPItem* parent = scope ? scope : this;
	
	foreach ( const QString& file, files ) {
		bool found = false;
		
		foreach ( const QString& name, filters.keys() ) {
			const DocumentFilter& filter = filters[ name ];
			
			if ( QDir::match( filter.filters, file ) ) {
				XUPItem* variable = project->getVariable( parent, name );
				bool exists = false;
				
				if ( variable ) {
					foreach ( XUPItem* item, variable->childrenList() ) {
						switch ( item->type() ) {
							case XUPItem::Value:
							case XUPItem::File:
							case XUPItem::Path:
								exists = pMonkeyStudio::isSameFile( file, filePath( item->content() ) );
								break;
							
							default:
								break;
						}
						
						if ( exists ) {
							break;
						}
					}
				}
				
				if ( !exists ) {
					if ( !variable ) {
						variable = parent->addChild( XUPItem::Variable );
						variable->setAttribute( "name", name );
					}
					
					XUPItem* value = variable->addChild( XUPItem::File );
					value->setContent( project->relativeFilePath( file ) );
				}
				
				found = true;
				break;
			}
		}
		
		if ( !found ) {
			notImported << file;
		}
	}
	
	if ( !notImported.isEmpty() ) {
		setLastError( tr( "Don't know how to add files:\n" ).arg( notImported.join( "\n" ) ) );
	}
	
	XUPProjectItem::cache()->build( this );
}

void XUPProjectItem::removeValue( XUPItem* item, bool deleteFiles, const QString& quoteString )
{
	switch ( item->type() ) {
		case XUPItem::Variable: {
			if ( deleteFiles ) {
				foreach ( XUPItem* value, item->childrenList() ) {
					removeValue( value, deleteFiles, quoteString );
				}
			}
			
			break;
		}
		case XUPItem::File: {
			if ( deleteFiles ) {
				const QString content = item->cacheValue( "content" ).remove( quoteString );
				const QString filePath = this->filePath( content );
				QFile file( filePath );
				
				if ( file.exists() ) {
					MonkeyCore::fileManager()->closeFile( filePath );
					
					if ( !file.remove() ) {
						MonkeyCore::messageManager()->appendMessage( tr( "Can't delete file '%1': %2" ).arg( filePath ).arg( file.errorString() ) );
					}
				}
			}
			
			break;
		}
		case XUPItem::Path:
		case XUPItem::Value: {
			break;
		}
		default:
			qWarning() << Q_FUNC_INFO << "Ask to remove bad item" << item->type() << item->displayText();
			Q_ASSERT( 0 );
			break;
	}
	
	item->parent()->removeChild( item );
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
	const QString searchFileName = QFileInfo( partialFilePath ).fileName();
	QFileInfoList files;
	
	foreach( XUPProjectItem* project, childrenProjects( true ) ) {
		foreach( const QString& file, project->sourceFiles() ) {
			const QFileInfo fileInfo( project->filePath( file ) );
			
			if ( fileInfo.exists() && fileInfo.fileName() == searchFileName ) {
				files << fileInfo;
			}
		}
	}
	
	return files;
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

XUPProjectItemList XUPProjectItem::childrenProjects( bool recursive ) const
{
	XUPProjectModel* model = this->model();
	XUPProjectItem* thisProject = project();
	QMap<QString, XUPProjectItem*> projects;

	if ( model )
	{
		const QModelIndexList projectIndexes = model->match( thisProject->index(), XUPProjectModel::TypeRole, XUPItem::Project, -1, Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive );

		foreach ( const QModelIndex& index, projectIndexes )
		{
			XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
			XUPProjectItem* project = item->project();

			if ( recursive || project->parentProject() == thisProject )
			{
				projects[ project->fileName() ] = project;
			}
		}
	}

	return projects.values();
}

XUPItemList XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, bool recursive, const XUPItem* caller ) const
{
	XUPItemList variables;

	foreach(XUPItem* item, root->childrenList())
	{
		if ( XUPItem::Variable ==  item->type() && variableName == item->attribute( "name" ) )
		{
			variables << item;
		}
		else if ( XUPItem::Project == item->type() )
		{
			if ( recursive )
			{
				XUPItem* pItem = item->parent();
				if ( pItem->type() == XUPItem::Function)
				{
					variables << getVariables( item, variableName );
				}
			}
		}
		else if ( recursive )
		{
			variables << getVariables( item, variableName );
		}
		
		if ( item == caller ) {
			break;
		}
	}

	return variables;
}

XUPItem* XUPProjectItem::getVariable( const XUPItem* root, const QString& variableName ) const
{
	foreach( XUPItem* item, root->childrenList() )
	{
		if ( XUPItem::Variable == item->type() && item->attribute( "name" ) == variableName )
		{
			return item;
		}
	}

	return 0;
}

QString XUPProjectItem::toXml() const
{
	return XUPProjectItemHelper::stripDynamicFolderFiles( mDocument ).toString( 4 );
	//return mDocument.toString( 4 );
}

QString XUPProjectItem::toNativeString() const
{
	return toXml();
}

bool XUPProjectItem::open( const QString& fileName, const QString& codec )
{
	// get QFile
	QFile file( fileName );

	// check existence
	if ( !file.exists() ) {
		topLevelProject()->setLastError( tr( "file not exists" ) );
		return false;
	}

	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) ) {
		topLevelProject()->setLastError( tr( "can't open file for reading" ) );
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
		topLevelProject()->setLastError(
			tr( "%1 on line: %2, column: %3" )
				.arg( errorMsg )
				.arg( errorLine )
				.arg( errorColumn )
		);
		return false;
	}

	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	
	if ( mDomElement.isNull() ) {
		topLevelProject()->setLastError( tr( "no project node" ) );
		return false;
	}
	
	// check xup version
	const QString docVersion = mDomElement.attribute( "version" );
	
	if ( pVersion( docVersion ) < XUP_VERSION ) {
		topLevelProject()->setLastError(
			tr( "The document format is too old, current version is '%1', your document is '%2'" )
				.arg( XUP_VERSION ).arg( docVersion )
		);
		return false;
	}

	// all is ok
	mCodec = codec;
	mFileName = fileName;
	topLevelProject()->setLastError( QString::null );
	file.close();
	
	const XUPDynamicFolderSettings dynamicFolderSettings = XUPProjectItemHelper::projectDynamicFolderSettings( this );
	
	if ( dynamicFolderSettings.Active && QFile::exists( dynamicFolderSettings.AbsolutePath ) ) {
		XUPDynamicFolderItem* dynamicFolderItem = XUPProjectItemHelper::projectDynamicFolderItem( this, true );
		dynamicFolderItem->setRootPath( dynamicFolderSettings.AbsolutePath, dynamicFolderSettings.FilesPatterns );
	}

	return true;
}

bool XUPProjectItem::save()
{
	// try open file for writing
	QFile file( mFileName );

	if ( !file.open( QIODevice::WriteOnly ) ) {
		return false;
	}

	// erase file content
	file.resize( 0 );

	// set xup version
	setAttribute( "version", XUP_VERSION );

	// encode content
	QTextCodec* codecObj = QTextCodec::codecForName( codec().toAscii ()  );
	QByteArray content = codecObj->fromUnicode( toNativeString() );

	// write content
	bool result = file.write( content ) != -1;
	file.close();

	// set error message if needed
	if ( result ) {
		topLevelProject()->setLastError( QString::null );
		XUPProjectItem::cache()->build( rootIncludeProject() );
	}
	else {
		topLevelProject()->setLastError( tr( "Can't write content" ) );
	}

	return result;
}

QString XUPProjectItem::targetFilePath( bool, XUPProjectItem::TargetType )
{
	return QString::null;
}

void XUPProjectItem::addCommands( const QString& mnu, const QString& text, pCommandList& cmds )
{
	// create action
	QAction* action = MonkeyCore::menuBar()->action( QString( "%1/%2" ).arg( mnu ).arg( text ) , text );
	action->setStatusTip( text );
	
	// set action custom data contain the command to execute
	action->setData( QVariant::fromValue( cmds ) );
	
	// connect to signal
	connect( action, SIGNAL( triggered() ), this, SLOT( internal_projectCustomActionTriggered() ) );
	
	// update menu visibility
	MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
	
	mInstalledActions << action;
}

void XUPProjectItem::addSeparator( const QString& mnu )
{
	static int i = 0;
	QAction* action = MonkeyCore::menuBar()->action( QString( "%1/%2" ).arg( mnu ).arg( QString( "*separator%1*" ).arg( i++ ) ) );
	action->setSeparator( true );
	mInstalledActions << action;
}

void XUPProjectItem::addCommand( pCommand& cmd, const QString& mnu )
{
	pCommandList cmds;
	cmds << cmd;
	return addCommands( mnu, cmd.text(), cmds);
}

void XUPProjectItem::installCommands()
{
	const MenuCommandListMap commands = XUPProjectItemHelper::projectCommands( this );
	
	foreach ( const QString& menu, commands.keys() ) {
		foreach ( pCommand command, commands[ menu ] ) {
			if ( !menu.isEmpty() ) {
				addCommand( command, menu );
			}
		}
	}
}

void XUPProjectItem::uninstallCommands()
{
	qDeleteAll( mInstalledActions );
	mInstalledActions.clear();

	// update menu visibility
	MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
}

QString XUPProjectItem::codec() const
{
	if (! mCodec.isNull())
		return mCodec;
	else
		return pMonkeyStudio::defaultCodec();
}

const DocumentFilterMap& XUPProjectItem::documentFilters() const
{
	return MonkeyCore::projectTypesIndex()->documentFilters( projectType() );
}

XUPProjectItemCacheBackend* XUPProjectItem::cacheBackend() const
{
	return &XUPProjectItem::mProjectsCacheBackend;
}

QStringList XUPProjectItem::cachedVariableValues( const QString& variableName ) const
{
    return XUPProjectItem::cache()->values( rootIncludeProject(), variableName );
}

QString XUPProjectItem::cachedVariableValue( const QString& variableName ) const
{
    return cachedVariableValues( variableName ).join( " " );
}

XUPProjectItemCache* XUPProjectItem::cache()
{
	return &XUPProjectItem::mProjectsCache;
}

void XUPProjectItem::internal_projectCustomActionTriggered()
{
	QAction* action = qobject_cast<QAction*>( sender() );
	Q_ASSERT(action);
	
	if ( action )
	{
		// save project files
		if ( pMonkeyStudio::saveFilesOnCustomAction() )
		{
			MonkeyCore::workspace()->fileSaveAll_triggered();
		}
		
		pCommandList cmds = action->data().value<pCommandList>();
		
		foreach ( pCommand cmd, cmds )
		{
			cmd = MonkeyCore::consoleManager()->processCommand( cmd );
			
			if (cmd.executableCheckingEnabled())
			{
				QString fileName = cmd.project()->filePath( cmd.command() );
				QString workDir = cmd.workingDirectory();
				const QFileInfo fileInfo( fileName );
				
				// if not exists ask user to select one
				if ( !fileInfo.exists() )
				{
					QMessageBox::critical( MonkeyCore::mainWindow(), tr( "Executable file not found" ), tr( "Target '%1' does not exists" ).arg( fileName ) );
					return;
				}
				
				if ( !fileInfo.isExecutable() )
				{
					QMessageBox::critical( MonkeyCore::mainWindow(), tr( "Can't execute target" ), tr( "Target '%1' is not an executable" ).arg( fileName ) );
					return;
				}
			}
			
			MonkeyCore::consoleManager()->addCommand( cmd );
		
		}
	}
}
