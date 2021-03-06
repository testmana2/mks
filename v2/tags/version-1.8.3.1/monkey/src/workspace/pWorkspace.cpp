/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pWorkspace.cpp
** Date      : 2008-01-14T00:37:21
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include <QToolButton>
#include <QCloseEvent>
#include <QMainWindow>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QDir>

#include <QDebug>

#include "pWorkspace.h"
#include "pAbstractChild.h"
#include "../recentsmanager/pRecentsManager.h"
#include "pFileManager.h"
#include "../maininterface/ui/UISettings.h"
#include "../maininterface/ui/UITranslator.h"
#include "UISaveFiles.h"
#include "../maininterface/ui/UIAbout.h"
#include "../templatesmanager/ui/UITemplatesWizard.h"
#include "../abbreviationsmanager/pAbbreviationsManager.h"
#include "../pMonkeyStudio.h"
#include "../templatesmanager/pTemplatesManager.h"
#include "../xupmanager/gui/XUPProjectManager.h"
#include "../xupmanager/core/XUPProjectItem.h"
#include "../pluginsmanager/PluginsManager.h"
#include "../coremanager/MonkeyCore.h"
#include "../maininterface/UIMain.h"
#include "../queuedstatusbar/QueuedStatusBar.h"
#include "MkSFileDialog.h"

#include "pChild.h"
#include "../qscintillamanager/pEditor.h"

using namespace pMonkeyStudio;

pWorkspace::pWorkspace( QMainWindow* p )
	: pExtendedWorkspace( p )
{
	Q_ASSERT( p );
	// creaet file watcher
	mFileWatcher = new QFileSystemWatcher( this );
	
	// add dock to main window
	p->addDockWidget( Qt::LeftDockWidgetArea, listWidget() );

	// set background
	//setBackground( ":/application/icons/application/background.png" );

	// set right corner button pixmap
	//cornerWidget()->findChild<QToolButton*>()->defaultAction()->setIcon( QPixmap( ":/application/icons/application/closetab.png" ) );
	
	// load settings
	loadSettings();

	// connections
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( internal_currentChanged( int ) ) );
	/*
	connect( this, SIGNAL( aboutToCloseTab( int, QCloseEvent* ) ), this, SLOT( internal_aboutToCloseTab( int, QCloseEvent* ) ) );
	connect( this, SIGNAL( closeAllRequested() ), this, SLOT( fileCloseAll_triggered() ) );
	connect( tabBar(), SIGNAL( urlsDropped( const QList<QUrl>& ) ), this, SLOT( internal_urlsDropped( const QList<QUrl>& ) ) );
	*/
	connect( listWidget(), SIGNAL( urlsDropped( const QList<QUrl>& ) ), this, SLOT( internal_urlsDropped( const QList<QUrl>& ) ) );
	connect( listWidget(), SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( internal_listWidget_customContextMenuRequested( const QPoint& ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( projectCustomContextMenuRequested( const QPoint& ) ), this, SLOT( internal_projectsManager_customContextMenuRequested( const QPoint& ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ), this, SLOT( internal_currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ) );
	connect( mFileWatcher, SIGNAL( fileChanged( const QString& ) ), this, SLOT( fileWatcher_fileChanged( const QString& ) ) );
	
	QAction* mFocusToEditor = new QAction( this );
	mFocusToEditor->setIcon( QIcon( ":/edit/icons/edit/text.png" ) );
	mFocusToEditor->setText( tr( "Set Focus to Editor" ) );
	pActionsManager::setDefaultShortcut( mFocusToEditor, QKeySequence( "Ctrl+Return" ) );
	
	MonkeyCore::menuBar()->addAction( "mView", mFocusToEditor );
	
	connect( mFocusToEditor, SIGNAL( triggered() ), this, SLOT( focusToEditor_triggered() ) );
}

void pWorkspace::loadSettings()
{
	// restore tabs settings
	/*
	tabBar()->setTabsHaveCloseButton( tabsHaveCloseButton() );
	tabBar()->setTabsHaveShortcut( tabsHaveShortcut() );
	tabBar()->setTabsElided( tabsElided() );
	tabBar()->setTabsColor( tabsTextColor() );
	tabBar()->setCurrentTabColor( currentTabTextColor() );
	*/
	setDocMode( pMonkeyStudio::docMode() );
}

pAbstractChild* pWorkspace::currentChild() const
{ return qobject_cast<pAbstractChild*>( currentDocument() ); }

pAbstractChild* pWorkspace::child( int i ) const
{ return qobject_cast<pAbstractChild*>( document( i ) ); }

QList<pAbstractChild*> pWorkspace::children() const
{
	QList<pAbstractChild*> l;
	foreach ( QWidget* w, documents() )
		l << qobject_cast<pAbstractChild*>( w );
	return l;
}

void pWorkspace::addSearhReplaceWidget (QWidget* widget)
{
	mLayout->addWidget( widget );

}

pAbstractChild* pWorkspace::newTextEditor()
{
	pFileDialogResult result = MkSFileDialog::getNewEditorFile( window() );

	// open open file dialog
	QString fileName = result[ "filename" ].toString();
	
	// return 0 if user cancel
	if ( fileName.isEmpty() )
	{
		return 0;
	}
	
	// close file if already open
	mFileWatcher->removePaths( QStringList( fileName ) );
	closeFile( fileName );

	// create/reset file
	QFile file( fileName );
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Can't create new file '%1'" ).arg( QFileInfo( fileName ).fileName() ), 2000 );
		return 0;
	}
	
	// reset file
	file.resize( 0 );
	file.close();
	
	if ( result.value( "addtoproject", false ).toBool() )
	{
		// add files to scope
		MonkeyCore::projectsManager()->addFilesToScope( result[ "scope" ].value<XUPItem*>(), QStringList( fileName ), result[ "operator" ].toString() );
	}
	
	// open file
	return openFile( fileName, result[ "codec" ].toString() );
}

void pWorkspace::initChildConnections( pAbstractChild* child )
{
	// connections
	connect( child, SIGNAL( currentFileChanged( const QString& ) ), this, SLOT( internal_currentFileChanged( const QString& ) ) );
	// closed file
	connect( child, SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
	// update file menu
	connect( child, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
	// update edit menu
	connect( child, SIGNAL( undoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
	connect( child, SIGNAL( redoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
	connect( child, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
	connect( child, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
	connect( child, SIGNAL( pasteAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
	//connect( child, SIGNAL( searchReplaceAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aSearchReplace" ), SLOT( setEnabled( bool ) ) );
	//connect( child, SIGNAL( goToAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aGoTo" ), SLOT( setEnabled( bool ) ) );
	// update status bar
	connect( child, SIGNAL( cursorPositionChanged( const QPoint& ) ), MonkeyCore::statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
	connect( child, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::statusBar(), SLOT( setModified( bool ) ) );
	//connect( child, SIGNAL( documentModeChanged( AbstractChild::DocumentMode ) ), statusBar(), SLOT( setDocumentMode( AbstractChild::DocumentMode ) ) );
	//connect( child, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), statusBar(), SLOT( setLayoutMode( AbstractChild::LayoutMode ) ) );
	//connect( child, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
}

pAbstractChild* pWorkspace::openFile( const QString& s, const QString& codec )
{
	// if it not exists
	if ( !QFile::exists( s ) || !QFileInfo( s ).isFile() )
		return 0;
	
	// check if file is already opened
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				setCurrentDocument( c );
				c->showFile( f );
				return c;
			}
		}
	}
	
	// track file
	mFileWatcher->addPath( s );

	// try opening file with child plugins
	pAbstractChild* c = MonkeyCore::pluginsManager()->openChildFile( s ); // TODO: repalce by a childForFileName member witch will return a child that will open file
	
	// open it with pChild instance if no c
	if ( !c )
		c = new pChild;
	
	// made connection if worksapce don t contains this child
	bool wasIn = children().contains( c );
	if ( !wasIn )
	{
		// setups connections
		initChildConnections( c );
		// add to workspace
		addDocument( c, QString() );
	}

	// open file
	if ( !c->openFile( s, codec ) )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "An error occur while opening this file: '%1'" ).arg( QFileInfo( s ).fileName() ), -1 );
		
		if ( !wasIn )
		{
			closeDocument( c );
			c->deleteLater();
		}
		
		return 0;
	}
	
	// set correct document if needed ( sdi hack )
	if ( currentDocument() != c )
	{
		setCurrentDocument( c );
		c->showFile( s );
	}
	
	// temporary hack
	internal_currentChanged( indexOf( c ) );

	// emit file open
	emit fileOpened( s );

	// return child instance
	return c;
}

void pWorkspace::closeFile( const QString& s )
{
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				c->closeFile( f );
				if ( c->files().isEmpty() )
					closeDocument( c );
				return;
			}
		}
	}
}

void pWorkspace::goToLine( const QString& s, const QPoint& p, bool b, const QString& codec )
{
	if ( b )
		openFile( s, codec );
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				setCurrentDocument( c );
				c->goTo( s, p, b );
				return;
			}
		}
	}
}

void pWorkspace::internal_currentFileChanged( const QString& file )
{
	QDir::setCurrent( QFileInfo( file ).absolutePath() );
}

void pWorkspace::internal_currentChanged( int i )
{
	// get child
	pAbstractChild* c = child( i );
	pEditor* editor = c ? c->currentEditor() : 0;
	bool hasChild = c;
	bool hasEditor = editor;
	bool modified = hasChild ? c->isModified() : false;
	bool print = hasChild ? c->isPrintAvailable() : false;
	bool undo = hasChild ? c->isUndoAvailable() : false;
	bool redo = hasChild ? c->isRedoAvailable() : false;
	bool copy = hasChild ? c->isCopyAvailable() : false;
	bool paste = hasChild ? c->isPasteAvailable() : false;
	bool go = hasChild ? c->isGoToAvailable() : false;
	bool moreThanOneChild = count() > 1;

	// update file menu
	MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( modified );
	MonkeyCore::menuBar()->action( "mFile/mSave/aAll" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/mClose/aAll" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/aSaveAsBackup" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/aQuickPrint" )->setEnabled( print );
	MonkeyCore::menuBar()->action( "mFile/aPrint" )->setEnabled( print );

	// update edit menu
	MonkeyCore::menuBar()->action( "mEdit/aUndo" )->setEnabled( undo );
	MonkeyCore::menuBar()->action( "mEdit/aRedo" )->setEnabled( redo );
	MonkeyCore::menuBar()->action( "mEdit/aCut" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aCopy" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aPaste" )->setEnabled( paste );
	
	MonkeyCore::menuBar()->action( "mEdit/aGoTo" )->setEnabled( go );
	MonkeyCore::menuBar()->action( "mEdit/aExpandAbbreviation" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->setMenuEnabled( MonkeyCore::menuBar()->menu( "mEdit/mAllCommands" ), hasEditor );
	MonkeyCore::menuBar()->setMenuEnabled( MonkeyCore::menuBar()->menu( "mEdit/mBookmarks" ), hasEditor );

	// update view menu
	MonkeyCore::menuBar()->action( "mView/aNext" )->setEnabled( moreThanOneChild );
	MonkeyCore::menuBar()->action( "mView/aPrevious" )->setEnabled( moreThanOneChild );

	// update status bar
	MonkeyCore::statusBar()->setModified( c ? c->isModified() : false );
	MonkeyCore::statusBar()->setEOLMode( editor ? editor->eolMode() : (QsciScintilla::EolMode)-1 );
	MonkeyCore::statusBar()->setIndentMode( editor ? ( editor->indentationsUseTabs() ? 1 : 0 ) : -1 );
	MonkeyCore::statusBar()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );

	// update item tooltip
	if ( hasChild )
		listWidget()->setItemToolTip( i, c->currentFile() );
	
	// internal update
	internal_currentFileChanged( hasChild ? c->currentFile() : QString() );
	
	// emit file changed
	emit currentFileChanged( c, hasChild ? c->currentFile() : QString() );
}
/*
void pWorkspace::internal_aboutToCloseTab( int i, QCloseEvent* e )
{
	if ( UISaveFiles::saveDocument( window(), child( i ), false ) == UISaveFiles::bCancelClose )
		e->ignore();
}
*/
void pWorkspace::internal_urlsDropped( const QList<QUrl>& l )
{
	// create menu
	QMenu m( this );
	QAction* aof = m.addAction( tr( "Open As &File" ) );
	QAction* aop = m.addAction( tr( "Open As &Project" ) );
	m.addSeparator();
	m.addAction( tr( "Cancel" ) );
	
	// execute menu
	QAction* a = m.exec( QCursor::pos() );
	
	// check triggered action
	if ( a == aof )
	{
		foreach ( QUrl u, l )
			if ( !u.toLocalFile().trimmed().isEmpty() )
				openFile( u.toLocalFile(), pMonkeyStudio::defaultCodec() );
	}
	else if ( a == aop )
	{
		foreach ( QUrl u, l )
			if ( !u.toLocalFile().trimmed().isEmpty() )
				MonkeyCore::projectsManager()->openProject( u.toLocalFile(), pMonkeyStudio::defaultCodec() );
	}
}

void pWorkspace::internal_listWidget_customContextMenuRequested( const QPoint& p )
{
	QMenu m;
	m.addAction( MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" ) );
	m.addAction( MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ) );
	m.exec( listWidget()->mapToGlobal( p ) );
}

void pWorkspace::internal_projectsManager_customContextMenuRequested( const QPoint& p )
{
	if ( MonkeyCore::projectsManager()->currentProject() )
	{
		// get menubar
		pMenuBar* mb = MonkeyCore::menuBar();
		// create menu
		QMenu m( this );
		// add menu commands
		m.addActions( mb->menu( "mProject" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mBuilder" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mDebugger" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mInterpreter" )->actions() );
		// show menu
		m.exec( MonkeyCore::projectsManager()->tvFiltered->mapToGlobal( p ) );
	}
}

void pWorkspace::internal_currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject )
{
	// uninstall old commands
	if ( previousProject )
	{
		previousProject->uninstallCommands();
		
		disconnect( previousProject, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ), this, SLOT( internal_projectInstallCommandRequested( const pCommand&, const QString& ) ) );
		disconnect( previousProject, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ), this, SLOT( internal_projectUninstallCommandRequested( const pCommand&, const QString& ) ) );
	}
	// get pluginsmanager
	PluginsManager* pm = MonkeyCore::pluginsManager();
	// set compiler, debugger and interpreter
	BuilderPlugin* bp = currentProject ? currentProject->builder() : 0;
	CompilerPlugin* cp = currentProject ? currentProject->compiler() : 0;
	DebuggerPlugin* dp = currentProject ? currentProject->debugger() : 0;
	InterpreterPlugin* ip = currentProject ? currentProject->interpreter() : 0;
	pm->setCurrentBuilder( bp && !bp->neverEnable() ? bp : 0 );
	pm->setCurrentCompiler( cp && !cp->neverEnable() ? cp : 0 );
	pm->setCurrentDebugger( dp && !dp->neverEnable() ? dp : 0 );
	pm->setCurrentInterpreter( ip && !ip->neverEnable() ? ip : 0 );
	
	// install new commands
	if ( currentProject )
	{
		connect( currentProject, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ), this, SLOT( internal_projectInstallCommandRequested( const pCommand&, const QString& ) ) );
		connect( currentProject, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ), this, SLOT( internal_projectUninstallCommandRequested( const pCommand&, const QString& ) ) );
		
		currentProject->installCommands();
	}
	// update menu visibility
	MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
}

void pWorkspace::internal_projectInstallCommandRequested( const pCommand& cmd, const QString& mnu )
{
	// create action
	QAction* a = MonkeyCore::menuBar()->action( QString( "%1/%2" ).arg( mnu, cmd.text() ) , cmd.text() );
	a->setStatusTip( cmd.text() );
	// set action custom data contain the command to execute
	a->setData( QVariant::fromValue( cmd ) );
	// connect to signal
	connect( a, SIGNAL( triggered() ), this, SLOT( projectCustomActionTriggered() ) );
}

void pWorkspace::internal_projectUninstallCommandRequested( const pCommand& cmd, const QString& mnu )
{
	QMenu* menu = MonkeyCore::menuBar()->menu( mnu );
	foreach ( QAction* a, menu->actions() )
	{
		if ( a->menu() )
			internal_projectUninstallCommandRequested( cmd, QString( "%1/%2" ).arg( mnu ).arg( a->menu()->objectName() ) );
		else if ( !a->isSeparator() && a->data().value<pCommand>() == cmd )
			delete a;
	}
}

void pWorkspace::projectCustomActionTriggered()
{
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
	{
		pConsoleManager* cm = MonkeyCore::consoleManager();
		pCommand cmd = a->data().value<pCommand>();
		pCommandMap* cmdsHash = cmd.userData().value<pCommandMap*>();
		const pCommandList cmds = cmdsHash ? cmdsHash->values() : pCommandList();
		// save project if needed
		if ( saveProjectsOnCustomAction() )
		{
			// TODO: completly remove the save, save all proejct way: project must be save each time they have been edited
			//MonkeyCore::projectsManager()->action( UIXUPManager::SaveAll )->trigger();
		}
		// save project files
		if ( saveFilesOnCustomAction() )
		{
			fileSaveAll_triggered();
		}
		// check that command to execute exists, else ask to user if he want to choose another one
		if ( cmd.project() && a->text().contains( "execute", Qt::CaseInsensitive ) )
		{
			cmd = cm->processCommand( cm->getCommand( cmds, cmd.text() ) );
			QString s = QString( "%1/%2" ).arg( cmd.workingDirectory() ).arg( cmd.command() );
			if ( !QFile::exists( s ) )
			{
				XUPProjectItem* project = cmd.project();
				XUPProjectItem* topLevelProject = project->topLevelProject();
				// try reading already saved binary
				s = topLevelProject->projectSettingsValue( a->text().replace( ' ', '_' ).toUpper() );
				if ( !s.isEmpty() )
				{
					s = topLevelProject->filePath( s );
				}
				// if not exists ask user to select one
				if ( !QFile::exists( s ) && question( a->text().append( "..." ), tr( "Can't find your executable file, do you want to choose the file ?" ) ) )
				{
					s = getOpenFileName( a->text().append( "..." ), cmd.workingDirectory() );
				}
				// if file exists execut it
				if ( QFile::exists( s ) )
				{
					QFileInfo fi( s );
					QString f = fi.fileName().prepend( "./" );
					QString p = fi.absolutePath();
					if ( p.endsWith( '/' ) )
					{
						p.chop( 1 );
					}
					// correct command
					cmd.setCommand( cm->quotedString( cm->nativeSeparators( s ) ) );
					cmd.setWorkingDirectory( cm->nativeSeparators( p ) );
					// write in project
					topLevelProject->setProjectSettingsValue( a->text().replace( ' ', '_' ).toUpper(), topLevelProject->relativeFilePath( s ) );
					topLevelProject->save();
					// add command to console manager
					cm->addCommand( cmd );
				}
			}
			// return
			return;
		}
		// generate commands list
		pCommandList mCmds = cm->recursiveCommandList( cmds, cm->getCommand( cmds, cmd.text() ) );
		// the first one must not be skipped on last error
		if ( mCmds.count() > 0 )
			mCmds.first().setSkipOnError( false );
		// send command to consolemanager
		cm->addCommands( mCmds );
	}
}

void pWorkspace::fileWatcher_ecmNothing( const QString& filename )
{
	MonkeyCore::statusBar()->appendMessage( tr( "File externally modified: '%1'" ).arg( QFileInfo( filename ).fileName() ), 2000 );
}

void pWorkspace::fileWatcher_ecmReload( const QString& filename, bool force )
{
	// try reload
	pAbstractChild* ac = MonkeyCore::fileManager()->childForFile (filename);
	if ( ac && ( !ac->isModified(filename) || force ) )
	{
		ac->closeFile(filename);
		ac->openFile(filename, ac->textCodec());
		MonkeyCore::statusBar()->appendMessage( tr( "Reloaded externally modified file: '%1'" ).arg( QFileInfo( filename ).fileName() ), 2000 );
		return;;
	}
	// ask user
	fileWatcher_ecmAlert( filename );
}

void pWorkspace::fileWatcher_ecmAlert( const QString& filename )
{
	pQueuedMessage m;
	m.Message = tr( "The file '%1' has been modified externally, what you do ?" ).arg( QFileInfo( filename ).fileName() );
	m.MilliSeconds = 0;
	m.Pixmap = QPixmap();
	m.Background = QBrush( QColor( 255, 0, 0, 20 ) );
	m.Foreground = QBrush();
	m.Buttons[ QDialogButtonBox::Ignore ] = QString();
	m.Buttons[ QDialogButtonBox::Reset ] = tr( "Reload" );
	m.Object = this;
	m.Slot = "fileWatcher_alertClicked";
	m.UserData = filename;
	MonkeyCore::statusBar()->appendMessage( m );
}

void pWorkspace::fileWatcher_alertClicked( QDialogButtonBox::StandardButton button, const pQueuedMessage& message )
{
	switch ( button )
	{
		case QDialogButtonBox::Ignore:
			fileWatcher_ecmNothing( message.UserData.toString() );
			break;
		case QDialogButtonBox::Reset:
			fileWatcher_ecmReload( message.UserData.toString(), true );
			break;
		default:
			break;
	}
}

void pWorkspace::fileWatcher_fileChanged( const QString& filename )
{
	switch ( pMonkeyStudio::externalchanges() )
	{
		// silently inform user
		case pMonkeyStudio::ecmNothing:
			fileWatcher_ecmNothing( filename );
			break;
		// check if file is open, if open and modified goto alert, else reload it
		case pMonkeyStudio::ecmReload:
			fileWatcher_ecmReload( filename );
			break;
		// ask user what to do
		case pMonkeyStudio::ecmAlert:
			fileWatcher_ecmAlert( filename );
			break;
	}
}

// file menu
void pWorkspace::fileNew_triggered()
{
	UITemplatesWizard* d = UITemplatesWizard::instance( this );
	d->setType( "Files" );
	d->exec();
}

void pWorkspace::fileOpen_triggered()
{
	// get available filters
	QString mFilters = availableFilesFilters();
	
	// path to show
	QString path = MonkeyCore::fileManager()->currentChildFile();
	if ( path.isEmpty() )
	{
		path = QDir::current().absolutePath();
	}
	
	
	// show filedialog to user
	pFileDialogResult result = MkSFileDialog::getOpenFileNames( window(), tr( "Choose the file(s) to open" ), path, mFilters, true, false );

	// open open file dialog
	QStringList fileNames = result[ "filenames" ].toStringList();
	
	// return 0 if user cancel
	if ( fileNames.isEmpty() )
	{
		return;
	}

	// for each entry, open file
	foreach ( QString file, fileNames )
	{
		if ( openFile( file, result[ "codec" ].toString() ) )
		{
			// append file to recents
			MonkeyCore::recentsManager()->addRecentFile( file );
		}
		else
		{
			// remove it from recents files
			MonkeyCore::recentsManager()->removeRecentFile( file );
		}
	}
}

void pWorkspace::fileSessionSave_triggered()
{
	QStringList l;
	// saves opened files
	foreach ( pAbstractChild* c, children() )
	{
		l << c->files();
	}
	MonkeyCore::settings()->setValue( "Session/Files", l );
	// saves opened projects
	l.clear();
	foreach ( XUPProjectItem* p, MonkeyCore::projectsManager()->topLevelProjects() )
	{
		l << p->fileName();
	}
	MonkeyCore::settings()->setValue( "Session/Projects", l );
}

void pWorkspace::fileSessionRestore_triggered()
{
	// restore files
	foreach ( QString s, MonkeyCore::settings()->value( "Session/Files", QStringList() ).toStringList() )
		if ( !openFile( s, pMonkeyStudio::defaultCodec() ) ) // remove it from recents files
			MonkeyCore::recentsManager()->removeRecentFile( s );
	// restore projects
	foreach ( QString s, MonkeyCore::settings()->value( "Session/Projects", QStringList() ).toStringList() )
		if ( !MonkeyCore::projectsManager()->openProject( s, pMonkeyStudio::defaultCodec() ) ) // remove it from recents projects
			MonkeyCore::recentsManager()->removeRecentProject( s );
}

void pWorkspace::fileSaveCurrent_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
	{
		const QString fn = c->currentFile();
		mFileWatcher->removePath( fn );
		c->saveCurrentFile();
		mFileWatcher->addPath( fn );
	}
}

void pWorkspace::fileSaveAll_triggered()
{
	foreach ( pAbstractChild* c, children() )
	{
		const QStringList fns = c->files();
		mFileWatcher->removePaths( fns );
		c->saveFiles();
		mFileWatcher->addPaths( fns );
	}
}

void pWorkspace::fileCloseCurrent_triggered()
{ closeCurrentDocument(); }

void pWorkspace::fileCloseAll_triggered()
{ closeAllDocuments(); }

void pWorkspace::fileSaveAsBackup_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
	{
		const QString s = getSaveFileName( tr( "Choose a filename to backup your file" ), QFileInfo( c->currentFile() ).fileName(), QString::null, this );
		if ( !s.isEmpty() )
			c->backupCurrentFile( s );
	}
}

void pWorkspace::fileQuickPrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->quickPrintCurrentFile();
}

void pWorkspace::filePrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->printCurrentFile();
}

void pWorkspace::fileExit_triggered()
{ window()->close(); }

// edit menu
void pWorkspace::editSettings_triggered()
{ UISettings::instance( this )->exec(); }

void pWorkspace::editTranslations_triggered()
{
	if ( UITranslator::instance( this )->exec() )
		loadTranslations();
}

void pWorkspace::editUndo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->undo();
}

void pWorkspace::editRedo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->redo();
}

void pWorkspace::editCut_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->cut();
}

void pWorkspace::editCopy_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->copy();
}

void pWorkspace::editPaste_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->paste();
}

void pWorkspace::editSearch_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->invokeSearch();
}

void pWorkspace::editGoTo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->goTo();
}

void pWorkspace::editExpandAbbreviation_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		pAbbreviationsManager::expandAbbreviation( c->currentEditor() );
}

void pWorkspace::editPrepareAPIs_triggered()
{ prepareAPIs(); }

// view menu
void pWorkspace::agStyles_triggered( QAction* a )
{
	qApp->setStyle( a->text() );
	qApp->setPalette( qApp->style()->standardPalette() );
	MonkeyCore::settings()->setValue( "MainWindow/Style", a->text() );
}

// help menu
void pWorkspace::helpAboutApplication_triggered()
{ UIAbout::instance( this )->exec(); }

void pWorkspace::helpAboutQt_triggered()
{ qApp->aboutQt(); }

#ifdef __COVERAGESCANNER__
void pWorkspace::helpTestReport_triggered()
{ UITestReport::instance( this )->exec(); }
#endif

void pWorkspace::focusToEditor_triggered ()
{
	if (currentChild() && currentChild()->currentEditor())
		currentChild()->currentEditor()->setFocus();
}

void pWorkspace::closeCurrentDocument()
{
	// close document
	if ( pAbstractChild* ac = currentChild() )
		closeDocument( ac );
}

bool pWorkspace::closeAllDocuments()
{
	// try save documents
	UISaveFiles::Buttons cb = UISaveFiles::saveDocuments( window(), children(), false );
	// close all object, disconnecting them
	if ( cb != UISaveFiles::bCancelClose )
	{
		// stop watching files
		foreach ( pAbstractChild* ac, children() )
			mFileWatcher->removePaths( ac->files() );
		// close all documents
		pExtendedWorkspace::closeAllDocuments();
		return true;
	}
	else
		return false; //not close IDE
}

void pWorkspace::closeDocument( QWidget* document )
{
	pAbstractChild* child = qobject_cast<pAbstractChild*>( document );
	if ( UISaveFiles::saveDocument( window(), child, false ) == UISaveFiles::bCancelClose )
		return;
	// stop watching files
	mFileWatcher->removePaths( child->files() );
	// close document
	pExtendedWorkspace::closeDocument( child );
}
