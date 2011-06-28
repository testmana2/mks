#include "XUPProjectItem.h"
#include "XUPProjectModel.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "UIMain.h"
#include "XUPPlugin.h"

#include <QTextCodec>
#include <QDir>
#include <QRegExp>
#include <QProcess>
#include <QFileDialog>
#include <QLibrary>
#include <QMessageBox>

#include <pVersion.h>

#include <QDebug>

XUPProjectItem::XUPProjectItem()
	: XUPItem( QDomElement(), 0 )
{
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
	QStringList entries;
	
	foreach ( const QString& name, sourceFileNamePatterns().keys() ) {
		XUPItem* variable = getVariable( this, name );
		
		if ( variable ) {
			foreach( XUPItem* item, variable->childrenList() ) {
				if ( item->type() == XUPItem::File ) {
					entries << filePath( item->content() );
				}
			}
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
	
	return entries;
}

QStringList XUPProjectItem::topLevelProjectSourceFiles() const
{
	QSet<QString> files;

	XUPProjectItemList projects = childrenProjects( true );

	foreach ( XUPProjectItem* project, projects ) {
		const QStringList sources = project->sourceFiles();

		foreach ( const QString& source, sources ) {
			files << source;
		}
	}

	return files.toList();
}

XUPPlugin* XUPProjectItem::driver() const
{
	return MonkeyCore::pluginsManager()->plugin<XUPPlugin*>( PluginsManager::stAll, projectType() );
}

QStringList XUPProjectItem::autoActivatePlugins() const
{
	return driver()->infos().dependencies;
}

QString XUPProjectItem::defaultIconsPath() const
{
	return ":/items";
}

QString XUPProjectItem::iconsPath() const
{
	return driver()->infos().iconsPath;
}

void XUPProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	const DocumentFilterMap filters = sourceFileNamePatterns();
	QStringList notImported;
	XUPProjectItem* project = scope->project();
	
	foreach ( const QString& file, files ) {
		bool found = false;
		
		foreach ( const QString& name, filters.keys() ) {
			const DocumentFilter& filter = filters[ name ];
			
			if ( QDir::match( filter.filters, file ) ) {
				XUPItem* variable = project->getVariable( scope, name );
				
				if ( !variable ) {
					variable = scope->addChild( XUPItem::Variable );
					variable->setAttribute( "name", name );
					
				}
				
				XUPItem* value = variable->addChild( XUPItem::File );
				value->setContent( project->relativeFilePath( file ) );
				
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
}

void XUPProjectItem::removeItem( XUPItem* item )
{
	switch ( item->type() ) {
		case XUPItem::File:
			#warning may ask to delete file here
			break;
		case XUPItem::Variable:
			#warning may ask to delete files here
			break;
		case XUPItem::Path:
		case XUPItem::Value:
			break;
		default:
			return;
	}
	
	item->parent()->removeChild( item );
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
	QFileInfoList files;
	QString searchFileName = QFileInfo( partialFilePath ).fileName();
	
	foreach(XUPProjectItem* project, childrenProjects(true))
	{
		foreach(QString file, project->sourceFiles())
		{
			if (QFileInfo( file ).fileName() == searchFileName)
			{
				files << QFileInfo(project->filePath(file)); // absolute path
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

QString XUPProjectItem::variableDisplayText( const QString& variableName ) const
{
	const QString text = sourceFileNamePatterns().value( variableName ).label;
	return text.isEmpty() ? variableName : text;
}

QString XUPProjectItem::variableDisplayIcon( const QString& variableName ) const
{
	QString icon = sourceFileNamePatterns().value( variableName ).icon;
	QString filePath;
	
	if ( icon.isEmpty() ) {
		icon = QString( "%1.png" ).arg( variableName.toLower() );
	}
	
	filePath = QString( "%1/%2" ).arg( iconsPath() ).arg( icon );
	
	if ( !QFile::exists( filePath ) ) {
		filePath = QString( "%1/%2" ).arg( defaultIconsPath() ).arg( icon );
	}
	
	return QDir::cleanPath( filePath );
}

XUPItemList XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, bool recursive ) const
{
	XUPItemList variables;

	foreach(XUPItem* item, root->childrenList())
	{
		if ( XUPItem::Variable ==  item->type() &&
			 variableName == item->attribute( "name" ))
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
	}

	return variables;
}

XUPItem* XUPProjectItem::getVariable( const XUPItem* root, const QString& variableName) const
{
	foreach(XUPItem* item, root->childrenList())
	{
		if (XUPItem::Variable == item->type() &&
			item->attribute( "name" ) == variableName )
		{
			return item;
		}
	}

	return 0;
}

QString XUPProjectItem::toXml() const
{
	return mDocument.toString( 4 );
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
	file.close();

	return true;
//	return analyze( this );
}

bool XUPProjectItem::save()
{
	// try open file for writing
	QFile file( mFileName );

	if ( !file.open( QIODevice::WriteOnly ) )
	{
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

void XUPProjectItem::addCommand( pCommand& cmd, const QString& mnu )
{
	pCommandList cmds;
	cmds << cmd;
	return addCommands( mnu, cmd.text(), cmds);
}

void XUPProjectItem::installCommands()
{
}

void XUPProjectItem::uninstallCommands()
{
	foreach( QAction* action, mInstalledActions )
	{
		delete action;
	}
	
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

QString XUPProjectItem::sourceFileNameFilter() const
{
	return pMonkeyStudio::buildFileDialogFilter( sourceFileNamePatterns(), true, true );
}

QStringList XUPProjectItem::filteredVariables() const
{
	const DocumentFilterMap filters = sourceFileNamePatterns();
	QMap<int, QString> variables;
	
	foreach ( const QString& name, filters.keys() ) {
		const DocumentFilter& filter = filters[ name ];
		variables.insertMulti( filter.weight, name );
	}
	
	return variables.values();
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
