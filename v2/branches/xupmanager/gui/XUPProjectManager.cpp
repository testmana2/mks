#include "XUPProjectManager.h"
#include "../iconmanager/pIconManager.h"
#include "../core/XUPProjectModel.h"
#include "../core/XUPProjectItem.h"

#include <QFileDialog>

#include <QDebug>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	tbOpen->setDefaultAction( action( atOpen ) );
	tbClose->setDefaultAction( action( atClose ) );
}

XUPProjectManager::~XUPProjectManager()
{
	delete XUPProjectItem::projectInfos();
}

void XUPProjectManager::on_lwOpenedProjects_itemSelectionChanged()
{
	QListWidgetItem* item = lwOpenedProjects->selectedItems().value( 0 );
	XUPProjectModel* project = mProjects.value( item );
	setCurrentProject( project );
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
		case XUPProjectManager::atOpen:
			action = new QAction( pIconManager::icon( "open.png" ), tr( "Open" ), this );
			action->setToolTip( tr( "Open a project" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( openProject() ) );
			break;
		case XUPProjectManager::atClose:
			action = new QAction( pIconManager::icon( "close.png" ), tr( "Close" ), this );
			action->setToolTip( tr( "Close the current project" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeProject() ) );
			break;
	}
	
	if ( action )
	{
		mActions[ type ] = action;
	}
	
	return action;
}

XUPProjectItem* XUPProjectManager::newProjectItem( const QString& fileName ) const
{
	int projectType = XUPProjectItem::projectInfos()->projectTypeForFileName( fileName );
	if ( mRegisteredProjectItems.value ( projectType ) )
		return mRegisteredProjectItems[ projectType ]->newItem();
	return 0;
}

void XUPProjectManager::openProject()
{
	const QString fn = QFileDialog::getOpenFileName( this, tr( "Choose a project to open" ), QLatin1String( "." ), XUPProjectItem::projectInfos()->projectsFilter() );
	if ( fn.isNull() )
	{
		return;
	}
	
	XUPProjectItem* projectItem = newProjectItem( fn );
	if ( !projectItem )
	{
		return;
	}
	
	XUPProjectModel* project = new XUPProjectModel( this );
	if ( project->open( projectItem, fn ) )
	{
		QListWidgetItem* item = new QListWidgetItem( project->rootProjectName(), lwOpenedProjects );
		mProjects[ item ] = project;
		setCurrentProject( project );
	}
	else
	{
		pteLog->appendPlainText( project->lastError() );
		delete project;
	}
}

void XUPProjectManager::XUPProjectManager::closeProject()
{
	XUPProjectModel* project = currentProject();
	if ( project )
	{
		if ( !project->save() )
		{
			pteLog->appendPlainText( project->lastError() );
		}
		project->close();
		QListWidgetItem* item = mProjects.key( project );
		mProjects.remove( item );
		delete item;
		delete project;
	}
}

XUPProjectModel* XUPProjectManager::currentProject() const
{
	return qobject_cast<XUPProjectModel*>( tvCurrentProject->model() );
}

void XUPProjectManager::setCurrentProject( XUPProjectModel* project )
{
	tvCurrentProject->setModel( project );
	tvNative->setModel( project );
	QListWidgetItem* item = mProjects.key( project );
	if ( lwOpenedProjects->selectedItems().value( 0 ) != item )
	{
		lwOpenedProjects->setCurrentItem( item );
	}
}

void XUPProjectManager::registerProjectItem( XUPProjectItem* item )
{
	int pType = item->projectType();
	if ( mRegisteredProjectItems.keys().contains( pType ) )
		return;
	item->registerProjectType();
	mRegisteredProjectItems[ pType ] = item;
}

void XUPProjectManager::unRegisterProjectType( int projectType )
{
	if ( mRegisteredProjectItems.keys().contains( projectType ) )
	{
		XUPProjectItem::projectInfos()->unRegisterType( projectType );
		delete mRegisteredProjectItems.take( projectType );
	}
}
