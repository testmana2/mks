#include <QMenu>

#include "pFilesListWidget.h"
#include "pTabbedWorkspace.h"

pFilesListWidget::pFilesListWidget(QString s, QWidget* w, pTabbedWorkspace* p) : QDockWidget (s, w), mWorkspace (p)
{
	setObjectName ("FilesListWidget");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setAcceptDrops( true );
	setContextMenuPolicy (Qt::CustomContextMenu);
	
	setWidget (&list);
	
	//this
	connect (this, SIGNAL (customContextMenuRequested ( const QPoint & )), this, SLOT (showMenu (const QPoint & )));
	
    //this -> workspace
	connect ( &list, SIGNAL (currentRowChanged (int)), mWorkspace, SLOT( setCurrentIndex( int ) ) );
	
    //workspace ->this
	connect( mWorkspace, SIGNAL( currentChanged( int ) ), this, SLOT( setCurrentIndex( int ) ) );
    connect( mWorkspace, SIGNAL( modifiedChanged( int, bool ) ), this, SLOT( modifiedChanged( int, bool ) ) );
    connect (mWorkspace, SIGNAL (documentInserted (int, QString, QIcon)), this, SLOT (documentInserted( int, QString, QIcon )));
    connect( mWorkspace, SIGNAL( documentClosed( int ) ), this, SLOT( documentClosed( int ) ) );
}

pFilesListWidget::~pFilesListWidget()
{
}

void pFilesListWidget::dragEnterEvent( QDragEnterEvent* e )
{
	// if correct mime and same tabbar
	if ( e->mimeData()->hasUrls() )
	{
		// accept drag
		e->acceptProposedAction();
	}
	
	// default event
	QDockWidget::dragEnterEvent( e );
}

void pFilesListWidget::dropEvent (QDropEvent* e)
{
	if ( e->mimeData()->hasUrls() )
		emit urlsDropped( e->mimeData()->urls () );
	// default event
	QDockWidget::dropEvent( e );
}

void pFilesListWidget::showMenu( const QPoint & pos )
{
	QMenu menu;
	mWorkspace->addFileActions (&menu);
	menu.exec (mapToGlobal (pos));
}

void pFilesListWidget::setCurrentIndex (int i)
{
	list.setCurrentRow (i);
}

void pFilesListWidget::modifiedChanged (int i, bool b)
{
    list.item(i)->setIcon ( b ? QIcon (":/file/icons/file/save.png") : QIcon (":/file/icons/file/transparent.png"));
}

void pFilesListWidget::documentInserted( int pos, QString s, QIcon )
{
    list.insertItem (pos, s);
}

void pFilesListWidget::documentClosed( int i )
{
    delete list.item ( i );
}
