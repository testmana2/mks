#include "XUPProjectManager.h"
#include "../iconmanager/pIconManager.h"
#include "../core/XUPProjectModel.h"
#include "../core/XUPProjectItem.h"

#include <QFileDialog>

#include <QDebug>
#include <QMenu>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	tbOpen->setDefaultAction( action( atOpen ) );
	tbClose->setDefaultAction( action( atClose ) );
	
	verticalLayout_2->parentWidget()->hide();
	
	mDebugMenu = new QMenu( tbDebug );
	tbDebug->setMenu( mDebugMenu );
	tbDebug->setPopupMode( QToolButton::InstantPopup );
	
	mDebugMenu->addAction( "interpretValue" );
	mDebugMenu->addAction( "interpretHandValue" );
	mDebugMenu->addAction( "project" );
	mDebugMenu->addAction( "topLevelProject" );
	mDebugMenu->addAction( "rootIncludeProject" );
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

void XUPProjectManager::on_tbDebug_triggered( QAction* action )
{
	const QModelIndex index = tvCurrentProject->currentIndex();
	if ( !index.isValid() )
		return;
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
	
	pteLog->appendPlainText( "------------------" );
	
	QString attribute;
	
	switch ( item->type() )
	{
		case XUPItem::Project:
			attribute = "name";
			break;
		case XUPItem::Comment:
			attribute = "value";
			break;
		case XUPItem::EmptyLine:
			attribute = "count";
			break;
		case XUPItem::Variable:
			attribute = "name";
			break;
		case XUPItem::Value:
			attribute = "content";
			break;
		case XUPItem::Function:
			attribute = "parameters";
			break;
		case XUPItem::Scope:
			attribute = "name";
			break;
		case XUPItem::Unknow:
			break;
	}
	
	if ( action->text() == "interpretValue" )
	{
		pteLog->appendPlainText( item->attribute( attribute ).prepend( "Content: " ) );
		pteLog->appendPlainText( item->project()->rootIncludeProject()->interpretValue( item, attribute ) );
	}
	else if ( action->text() == "interpretHandValue" )
	{
		pteLog->appendPlainText( "'" +item->attribute( attribute ) +"'" +" content: " +item->project()->rootIncludeProject()->interpretVariable( item->attribute( attribute ), item, "#Null" ) );
	}
	else if ( action->text() == "project" )
	{
		pteLog->appendPlainText( item->project()->displayText() );
	}
	else if ( action->text() == "topLevelProject" )
	{
		pteLog->appendPlainText( item->project()->topLevelProject()->displayText() );
	}
	else if ( action->text() == "rootIncludeProject" )
	{
		pteLog->appendPlainText( item->project()->rootIncludeProject()->displayText() );
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
	return XUPProjectItem::projectInfos()->newProjectItem( fileName );
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
		cbProjects->addItem( project->headerData( 0, Qt::Horizontal, Qt::DisplayRole ).toString(), QVariant::fromValue<XUPProjectModel*>( project ) );
		cbProjects->setItemIcon( id, project->headerData( 0, Qt::Horizontal, Qt::DecorationRole ).value<QIcon>() );
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
