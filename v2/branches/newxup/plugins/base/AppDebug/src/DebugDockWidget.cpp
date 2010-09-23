#include "DebugDockWidget.h"

#include <MonkeyCore.h>
#include <XUPProjectManager.h>
#include <XUPFilteredProjectModel.h>
#include <pDockWidgetTitleBar.h>
#include <pEditor.h>
#include <pChild.h>
#include <pFileManager.h>

#include <QTreeView>
#include <QPlainTextEdit>

DebugDockWidget::DebugDockWidget( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );

	titleBar()->addAction( aShowXml );
	titleBar()->addAction( aShowNativeString );
	
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SLOT( currentProjectChanged() ) );
	connect( aShowXml, SIGNAL( triggered() ), this, SLOT( showXml() ) );
	connect( aShowNativeString, SIGNAL( triggered() ), this, SLOT( showNativeString() ) );
}

DebugDockWidget::~DebugDockWidget()
{
}

void DebugDockWidget::qtMessageHandler( QtMsgType type, const char* msg )
{
	switch ( type ) {
		case QtDebugMsg:
			pteDebug->appendPlainText( QString::fromLocal8Bit( msg ) );
			printf( "*** Debug: %s\n", msg );
			break;
		case QtWarningMsg:
			pteWarning->appendPlainText( QString::fromLocal8Bit( msg ) );
			printf( "*** Warning: %s\n", msg );
			break;
		case QtCriticalMsg:
			pteCritical->appendPlainText( QString::fromLocal8Bit( msg ) );
			printf( "*** Critical: %s\n", msg );
			break;
		case QtFatalMsg:
			pteFatal->appendPlainText( QString::fromLocal8Bit( msg ) );
			printf( "*** Fatal: %s\n", msg );
			//abort();
	}
}

void DebugDockWidget::currentProjectChanged()
{
	tvProjects->setModel( MonkeyCore::projectsManager()->currentProjectModel() );
}

void DebugDockWidget::showXml()
{
	const QModelIndex index = tvProjects->selectionModel()->selectedIndexes().value( 0 );
	XUPItem* item = MonkeyCore::projectsManager()->currentProjectModel()->itemFromIndex( index );
	XUPProjectItem* project = item ? item->project() : 0;
	
	if ( !project ) {
		return;
	}
	
	pWorkspace* workspace = MonkeyCore::workspace();
	static pAbstractChild* document = 0;
	bool exists = document;
	
	if ( !document ) {
		document = new pChild;
	}
	
	pEditor* editor = document->editor();
	
	editor->setText( project->toXml() );
	editor->setModified( false );
	document->setFilePath( "xml content" );
	
	if ( !exists ) {
		workspace->handleDocument( document );
		emit document->fileOpened();
		document->showMaximized();
	}
	
	workspace->setCurrentDocument( document );
}

void DebugDockWidget::showNativeString()
{
	const QModelIndex index = tvProjects->selectionModel()->selectedIndexes().value( 0 );
	XUPItem* item = MonkeyCore::projectsManager()->currentProjectModel()->itemFromIndex( index );
	XUPProjectItem* project = item ? item->project() : 0;
	
	if ( !project ) {
		return;
	}
	
	pWorkspace* workspace = MonkeyCore::workspace();
	static pAbstractChild* document = 0;
	bool exists = document;
	
	if ( !document ) {
		document = new pChild;
	}
	
	pEditor* editor = document->editor();
	
	editor->setText( project->toNativeString() );
	editor->setModified( false );
	document->setFilePath( "native content" );
	
	if ( !exists ) {
		workspace->handleDocument( document );
		emit document->fileOpened();
		document->showMaximized();
	}
	
	workspace->setCurrentDocument( document );
}
