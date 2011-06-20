#include "DebugDockWidget.h"

#include <MonkeyCore.h>
#include <XUPProjectManager.h>
#include <XUPFilteredProjectModel.h>
#include <pDockWidgetTitleBar.h>
#include <pEditor.h>
#include <pChild.h>
#include <pFileManager.h>

#include <QMakeProjectItem.h>

#include <QTreeView>
#include <QPlainTextEdit>

DebugDockWidget::DebugDockWidget( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );

	titleBar()->addAction( aShowXml );
	titleBar()->addAction( aShowNativeString );
	titleBar()->addAction( aGenerateFakeProject );
	
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SLOT( currentProjectChanged() ) );
	connect( aShowXml, SIGNAL( triggered() ), this, SLOT( showXml() ) );
	connect( aShowNativeString, SIGNAL( triggered() ), this, SLOT( showNativeString() ) );
	connect( aGenerateFakeProject, SIGNAL( triggered() ), this, SLOT( generateFakeProject() ) );
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

void DebugDockWidget::generateFakeProject()
{
	QMakeProjectItem* project = new QMakeProjectItem;
	XUPItem* item;
	XUPItem* variable;
	XUPItem* value;
	XUPItem* scope;
	XUPItem* function;
	
	project->mDocument = QDomDocument( "XUPProject" );
	QDomElement projectElement = project->mDocument.createElement( "project" );
	project->mDocument.appendChild( projectElement );
	project->mDomElement = projectElement;
	
	// set project name
	project->setAttribute( "name", "My project" );
	
	// a comment
	item = project->addChild( XUPItem::Comment );
	item->setAttribute( "value", "#This is my comment" );
	
	// an empty line
	item = project->addChild( XUPItem::EmptyLine );
	item->setAttribute( "count", "4" );
	
	// a variable
	variable = project->addChild( XUPItem::Variable );
	variable->setAttribute( "operator", "*=" );
	variable->setAttribute( "name", "LIBS" );
	
	// values
	value = variable->addChild( XUPItem::Value );
	value->setContent( "-L/usr/include" );
	
	value = variable->addChild( XUPItem::Value );
	value->setContent( "-lpng" );
	
	value = variable->addChild( XUPItem::Value );
	value->setContent( "-lz" );
	
	// a function
	function = project->addChild( XUPItem::Function );
	function->setAttribute( "name", "isEqual" );
	function->setAttribute( "parameters", "TOTO = TITI" );
	
	// a variable
	variable = function->addChild( XUPItem::Variable );
	variable->setAttribute( "operator", "-=" );
	variable->setAttribute( "name", "LIBS" );
	
	value = variable->addChild( XUPItem::Value );
	value->setContent( "-lz" );
	
	// finish project
	project->mCodec = "UTF-8";
	project->mFileName = QString( "Fake project %1" ).arg( qrand() % INT_MAX );
	
	MonkeyCore::projectsManager()->openProject( project );
	
	// scope : name
	// folder : name
	// file : content
	// path : content
	
	/*XUPItem* addChild( XUPItem::Type type, int row = -1 );
	
	Unknow = -1,
	Project, // a project node
	Comment, // a comment node
	EmptyLine, // a empty line node
	Variable, // a variabel node
	Value, // a value node
	Function, // a function node
	Scope, // a scope node
	//
	DynamicFolder, // a dynamic folder node
	Folder, // a folder node
	File, // a value that is a file node
	Path // a value that is a path node*/
}
