#include "XUPProjectManager.h"
#include "UIXUPFindFiles.h"
#include "pIconManager.h"
#include "XUPProjectModel.h"
#include "XUPFilteredProjectModel.h"
#include "MkSFileDialog.h"
#include "pMonkeyStudio.h"
#include "MonkeyCore.h"
#include "PluginsManager.h"
#include "pWorkspace.h"
#include "XUPPlugin.h"
#include "UITemplatesWizard.h"
#include "pRecentsManager.h"

#include <pDockWidgetTitleBar.h>
#include <pQueuedMessageToolBar.h>
#include <pMenuBar.h>

#include <QTextCodec>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QDebug>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );
	setActionsManager( MonkeyCore::actionsManager() );
	
	titleBar()->addAction( action( atNew ), 0 );
	titleBar()->addAction( action( atOpen ), 1 );
	titleBar()->addAction( action( atClose ), 2 );
	titleBar()->addAction( action( atCloseAll ), 3 );
	titleBar()->addAction( action( atEdit ), 4 );
	titleBar()->addAction( action( atAddFiles ), 5 );
	titleBar()->addAction( action( atRemoveFiles ), 6 );
	titleBar()->addSeparator( 7 );
	
	mFilteredModel = new XUPFilteredProjectModel( tvFiltered );
	tvFiltered->setModel( mFilteredModel );
	
	connect( tvFiltered->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvFiltered_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( tvFiltered, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SIGNAL( projectCustomContextMenuRequested( const QPoint& ) ) );
	
	pMonkeyStudio::showMacFocusRect( wCentral, false, true );
	pMonkeyStudio::setMacSmallSize( wCentral, true, true );
	setCurrentProject( 0, 0 );
}

XUPProjectManager::~XUPProjectManager()
{
}

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* model = cbProjects->itemData( id ).value<XUPProjectModel*>();
	XUPProjectItem* project = model ? model->mRootProject : 0;
	setCurrentProject( project, currentProject() );
}

void XUPProjectManager::tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous )
{
	XUPItem* curItem = mFilteredModel->mapToSource( current );
	XUPItem* preItem = mFilteredModel->mapToSource( previous );
	XUPProjectItem* curProject = curItem ? curItem->project() : 0;
	XUPProjectItem* preProject = preItem ? preItem->project() : 0;
	
	if ( !previous.isValid() )
	{
		preProject = curProject->topLevelProject();
	}
	
	setCurrentProject( curProject, preProject );
}

void XUPProjectManager::on_tvFiltered_activated( const QModelIndex& index )
{
	XUPItem* item = mFilteredModel->mapToSource( index );
	if ( item )
	{
		if ( item->type() == XUPItem::Project )
		{
			emit projectDoubleClicked( item->project() );
		}
		else if ( item->type() == XUPItem::File )
		{
			XUPProjectItem* project = item->project();
			XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
			QString fn = rootIncludeProject->filePath( item->content() );
			
			if ( !QFile::exists( fn ) )
			{
				QString findFile = item->content().remove( '"' );
				QFileInfoList files = rootIncludeProject->findFile( findFile );
				switch ( files.count() )
				{
					case 0:
						fn.clear();
						break;
					case 1:
						fn = files.at( 0 ).absoluteFilePath();
						break;
					default:
					{
						UIXUPFindFiles dlg( findFile, this );
						dlg.setFiles( files, rootIncludeProject->path() );
						fn.clear();
						if ( dlg.exec() == QDialog::Accepted )
						{
							fn = dlg.selectedFile();
						}
						break;
					}
				}
			}
			
			if ( QFile::exists( fn ) )
			{
				emit fileDoubleClicked( project, fn, project->codec() );
				emit fileDoubleClicked( fn, project->codec() );
			}
		}
	}
}

void XUPProjectManager::on_tvFiltered_customContextMenuRequested( const QPoint& pos )
{
	if ( currentProject() )
	{
		// get menubar
		pMenuBar* mb = MonkeyCore::menuBar();
		QMenu menu;
		
		// add menu commands
		menu.addActions( mb->menu( "mProject" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mBuilder" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mDebugger" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mInterpreter" )->actions() );
		
		// show menu
		menu.exec( tvFiltered->mapToGlobal( pos ) );
	}
}

QAction* XUPProjectManager::action( XUPProjectManager::ActionType type )
{
	if ( mActions.contains( type ) )
	{
		return mActions[ type ];
	}
	
	QAction* action = 0;
	switch ( type )
	{
		case XUPProjectManager::atNew:
			action = new QAction( pIconManager::icon( "new.png", ":/project" ), tr( "New project..." ), this );
			mActionsManager->setDefaultShortcut( action, tr( "Ctrl+Shift+N" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( newProject() ) );
			break;
		case XUPProjectManager::atOpen:
			action = new QAction( pIconManager::icon( "open.png", ":/project" ), tr( "Open project..." ), this );
			mActionsManager->setDefaultShortcut( action, tr( "Ctrl+Shift+O" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( openProject() ) );
			break;
		case XUPProjectManager::atClose:
			action = new QAction( pIconManager::icon( "close.png", ":/project" ), tr( "Close current project" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeProject() ) );
			break;
		case XUPProjectManager::atCloseAll:
			action = new QAction( pIconManager::icon( "closeall.png", ":/project" ), tr( "Close all projects" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeAllProjects() ) );
			break;
		case XUPProjectManager::atEdit:
			action = new QAction( pIconManager::icon( "settings.png", ":/project" ), tr( "Edit current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( editProject() ) );
			break;
		case XUPProjectManager::atAddFiles:
			action = new QAction( pIconManager::icon( "add.png", ":/project" ), tr( "Add existing files to current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( addFiles() ) );
			break;
		case XUPProjectManager::atRemoveFiles:
			action = new QAction( pIconManager::icon( "remove.png", ":/project" ), tr( "Remove files from current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( removeFiles() ) );
			break;
	}
	
	if ( action )
	{
		action->setToolTip( action->text() );
		action->setStatusTip( action->text() );
		mActions[ type ] = action;
	}
	
	return action;
}

void XUPProjectManager::newProject()
{
	UITemplatesWizard wizard( this );
	wizard.setType( "Projects" );
	wizard.exec();
}

bool XUPProjectManager::openProject( const QString& fileName, const QString& codec )
{
	// check that project is not yet open
	foreach ( XUPProjectItem* project, topLevelProjects() )
	{
		if ( pMonkeyStudio::isSameFile( project->fileName(), fileName ) )
		{
			setCurrentProject( project, currentProject() );
			return true;
		}
	}
	
	QFileInfo fi( fileName );
	
	if ( fi.exists() && fi.isFile() )
	{
		XUPProjectModel* model = new XUPProjectModel( this );
		if ( model->open( fileName, codec ) )
		{
			// append file to recents project
			MonkeyCore::recentsManager()->addRecentProject( fileName );
			
			int id = cbProjects->count();
			cbProjects->addItem( model->headerData( 0, Qt::Horizontal, Qt::DisplayRole ).toString(), QVariant::fromValue<XUPProjectModel*>( model ) );
			cbProjects->setItemIcon( id, model->headerData( 0, Qt::Horizontal, Qt::DecorationRole ).value<QIcon>() );
			setCurrentProject( model->rootProject(), currentProject() );
			emit projectOpened( currentProject() );
			
			return true;
		}
		else
		{
			// remove from recents
			MonkeyCore::recentsManager()->removeRecentProject( fileName );
			// inform user about error
			MonkeyCore::messageManager()->appendMessage( tr( "An error occur while opening project '%1': %2" ).arg( fileName ).arg( model->lastError() ) );
			// free
			delete model;
		}
	}
	
	return false;
}

bool XUPProjectManager::openProject()
{
	pFileDialogResult result = MkSFileDialog::getOpenProjects( window() );
	
	const QStringList files = result[ "filenames" ].toStringList();
	if ( !files.isEmpty() )
	{
		const QString codec = result[ "codec" ].toString();
		foreach ( const QString file, files )
		{
			if ( !openProject( file, codec ) )
			{
				return false;
			}
		}
		
		return true;
	}
	
	return false;
}

void XUPProjectManager::closeProject()
{
	XUPProjectModel* curModel = currentProjectModel();
	
	if ( curModel )
	{
		XUPProjectItem* preProject = currentProject();
		
		bool blocked = cbProjects->blockSignals( true );
		int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( curModel ) );
		cbProjects->removeItem( id );
		XUPProjectModel* model = cbProjects->itemData( cbProjects->currentIndex() ).value<XUPProjectModel*>();
		setCurrentProjectModel( model );
		cbProjects->blockSignals( blocked );
		
		XUPProjectItem* curProject = currentProject();
		
		setCurrentProject( curProject, preProject );
		
		emit projectAboutToClose( preProject );
		
		curModel->close();
		delete curModel;
	}
}

void XUPProjectManager::closeAllProjects()
{
	while ( cbProjects->count() > 0 )
	{
		closeProject();
	}
}

void XUPProjectManager::editProject()
{
	XUPProjectItem* project = currentProject();
	
	if ( !project )
	{
		return;
	}
	
	XUPProjectItem* topLevelProject = project->topLevelProject();
		
	XUPProjectModel* model = currentProjectModel();
	QFileSystemWatcher* watcher = MonkeyCore::workspace()->fileWatcher();
	
	// edit project and save it if needed
	if ( project->edit() )
	{
		if ( project->save() )
		{
			// need save topLevelProject ( for XUPProejctSettings scope  )
			if ( !topLevelProject->save() )
			{
				MonkeyCore::messageManager()->appendMessage( tr( "An error occur while saving project '%1': %2" ).arg( topLevelProject->fileName() ).arg( topLevelProject->lastError() ) );
			}
		}
		else
		{
			MonkeyCore::messageManager()->appendMessage( tr( "An error occur while saving project '%1': %2" ).arg( project->fileName() ).arg( project->lastError() ) );
		}
		
		// update menu actions
		project->uninstallCommands();
		project->installCommands();
	}
}

void XUPProjectManager::addFiles( const QStringList& files, XUPItem* scope )
{
	XUPProjectItem* project = scope->project();
	//TODO check if files already added!
	project->addFiles(files, scope);
	
	// rebuild cache
	project->rebuildCache();
	project->topLevelProject()->rebuildCache();
	
	// save project
	if ( !project->save() )
	{
		MonkeyCore::messageManager()->appendMessage( tr( "An error occur while saving project '%1': %2" ).arg( project->fileName() ).arg( project->lastError() ) );
	}
}

void XUPProjectManager::addFiles()
{
	pFileDialogResult result = MkSFileDialog::getProjectAddFiles( window() );
	
	if ( !result.isEmpty() )
	{
		QStringList files = result[ "filenames" ].toStringList();
		XUPItem* scope = result[ "scope" ].value<XUPItem*>();
		
		// import files if needed
		if ( result[ "import" ].toBool() )
		{
			const QString projectPath = scope->project()->path();
			const QString importPath = result[ "importpath" ].toString();
			const QString importRootPath = result[ "directory" ].toString();
			QDir dir( importRootPath );
			
			for ( int i = 0; i < files.count(); i++ )
			{
				if ( !files.at( i ).startsWith( projectPath ) )
				{
					QString fn = QString( files.at( i ) ).remove( importRootPath ).replace( "\\", "/" );
					fn = QDir::cleanPath( QString( "%1/%2/%3" ).arg( projectPath ).arg( importPath ).arg( fn ) );
					
					if ( dir.mkpath( QFileInfo( fn ).absolutePath() ) && QFile::copy( files.at( i ), fn ) )
					{
						files[ i ] = fn;
					}
				}
			}
		}
		
		// add files to scope
		addFiles( files, scope );
	}
}

void XUPProjectManager::removeFiles()
{
	XUPItem* curItem = currentItem();
	
	
	if ( !curItem || !( curItem->type() == XUPItem::Value || curItem->type() == XUPItem::File || curItem->type() == XUPItem::Path ) )
	{
		return;
	}
	
	if ( QMessageBox::question( window(), tr( "Remove Value..." ), tr( "Are you sur you want to remove this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
	{
		XUPProjectItem* project = curItem->project();
		
		// if file item
		if ( curItem->type() == XUPItem::File )
		{
			XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
			const QString fp = rootIncludeProject->filePath( curItem->content() );
			
			// ask removing file
			if ( QFile::exists( fp ) && QMessageBox::question( window(), tr( "Delete associations..." ), tr( "Do you want to delete the associate file ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
			{
				if ( !QFile::remove( fp ) )
				{
					QMessageBox::warning( window(), tr( "Error..." ), tr( "Can't delete file: %1" ).arg( fp ) );
				}
			}
		}
		
		// remove value & variable if needed
		XUPItem* variable = curItem->parent();
		XUPItem* variableParent = variable->parent();
		
		variable->removeChild( curItem );
		
		if ( variable->childCount() == 0 )
		{
			variableParent->removeChild( variable );
		}
		
		// rebuild cache
		project->rebuildCache();
		project->topLevelProject()->rebuildCache();
		
		// save project
		if ( !project->save() )
		{
			MonkeyCore::messageManager()->appendMessage( tr( "An error occur while saving project '%1': %2" ).arg( project->fileName() ).arg( project->lastError() ) );
		}
	}
}

XUPProjectModel* XUPProjectManager::currentProjectModel() const
{
	return mFilteredModel->sourceModel();
}

XUPProjectItem* XUPProjectManager::currentProject() const
{
	XUPProjectModel* curModel = currentProjectModel();
	XUPItem* curItem = currentItem();
	
	if ( curItem )
	{
		return curItem->project();
	}
	
	if ( !curItem && curModel )
	{
		return curModel->mRootProject;
	}
	
	return 0;
}

XUPItem* XUPProjectManager::currentItem() const
{
	const QModelIndex index = tvFiltered->currentIndex();
	return mFilteredModel->mapToSource( index );
}

XUPProjectItemList XUPProjectManager::topLevelProjects() const
{
	XUPProjectItemList projects;
	
	for ( int i = 0; i < cbProjects->count(); i++ )
	{
		XUPProjectItem* project = cbProjects->itemData( i ).value<XUPProjectModel*>()->mRootProject;
		projects << project;
	}
	
	return projects;
}

void XUPProjectManager::setCurrentProjectModel( XUPProjectModel* model )
{
	mFilteredModel->setSourceModel( model );
	int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( model ) );
	cbProjects->setCurrentIndex( id );
}

void XUPProjectManager::setCurrentProject( XUPProjectItem* curProject, XUPProjectItem* preProject )
{
	// update project actions
	action( atClose )->setEnabled( curProject );
	action( atCloseAll )->setEnabled( curProject );
	action( atEdit )->setEnabled( curProject );
	action( atAddFiles )->setEnabled( curProject );
	action( atRemoveFiles )->setEnabled( curProject );
	
	if ( !curProject )
	{
		setCurrentProjectModel( 0 );
	}
	else if ( curProject->model() != currentProjectModel() )
	{
		setCurrentProjectModel( curProject->model() );
	}
	
	// if new project != old update gui
	if ( curProject != preProject )
	{
		emit currentProjectChanged( curProject, preProject );
		emit currentProjectChanged( curProject );
	}
}
