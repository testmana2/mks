#include "DebugDockWidget.h"

#include <MonkeyCore.h>
#include <XUPProjectManager.h>
#include <XUPFilteredProjectModel.h>
#include <pDockWidgetTitleBar.h>

#include <QTreeView>
#include <QPlainTextEdit>

DebugDockWidget::DebugDockWidget( QWidget* parent )
	: pDockWidget( parent )
{
	tvProjects = new QTreeView( this );
	setWidget( tvProjects );
	
	QAction* aXml = new QAction( this );
	aXml->setText( "Show Xml" );
	titleBar()->addAction( aXml );
	
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SLOT( currentProjectChanged() ) );
	connect( aXml, SIGNAL( triggered() ), this, SLOT( showXml() ) );
}

DebugDockWidget::~DebugDockWidget()
{
}

void DebugDockWidget::currentProjectChanged()
{
	tvProjects->setModel( MonkeyCore::projectsManager()->currentProjectModel() );
}

void DebugDockWidget::showXml()
{
	XUPProjectItem* project = MonkeyCore::projectsManager()->currentProject();
	
	QPlainTextEdit* pte = new QPlainTextEdit( this );
	pte->setWindowFlags( Qt::Window );
	pte->setAttribute( Qt::WA_DeleteOnClose );
	pte->setPlainText( project->toString() );
	pte->show();
}
