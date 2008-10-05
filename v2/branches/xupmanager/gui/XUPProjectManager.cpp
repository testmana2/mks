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

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* project = cbProjects->itemData( id ).value<XUPProjectModel*>();
	setCurrentProject( project );
}

void XUPProjectManager::on_tbDebug_clicked()
{
	const QModelIndex index = tvCurrentProject->currentIndex();
	if ( !index.isValid() )
		return;
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
	pteLog->appendPlainText( "------------------" );
	pteLog->appendPlainText( item->attribute( "content" ).prepend( "Content: " ) );
	pteLog->appendPlainText( item->project()->interpretValue( item ) );
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
	if ( mRegisteredProjectItems.value( projectType ) )
	{
		return mRegisteredProjectItems[ projectType ]->newItem();
	}
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
		int id = cbProjects->count();
		cbProjects->addItem( project->rootProjectName(), QVariant::fromValue<XUPProjectModel*>( project ) );
		cbProjects->setItemIcon( id, project->rootProjectIcon() );
		cbProjects->setCurrentIndex( id );
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
		int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
		cbProjects->removeItem( id );
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
	
	int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
	if ( cbProjects->currentIndex() != id )
	{
		cbProjects->setCurrentIndex( id );
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
