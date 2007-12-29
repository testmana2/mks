/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspace.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a workspace that can at run time be switched in SDI / MDI or Top Level
 ********************************************************************************************************/
#include "pTabbedWorkspace.h"

#include <QBoxLayout>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QIcon>
#include <QCloseEvent>
#include <QFile>
#include <QMenu>
#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>


pTabbedWorkspace::pTabbedWorkspace( QMainWindow* w, pTabbedWorkspace::DocumentMode m )
	: QWidget( w ), mMainWindow (w)
{
	// tab widget
	mTabLayout = new QBoxLayout( QBoxLayout::LeftToRight );
	mTabLayout->setSpacing( 3 );
	mTabLayout->setMargin( 0 );
	//mTabBar = new pTabBar (this);
	//mTabLayout->addWidget( ( mTabBar ) );

	mFilesList = new pFilesListWidget (tr("Files list"), w, this);
	mMainWindow->addDockWidget (Qt::LeftDockWidgetArea, mFilesList);
	
	// document widget
	mStackedLayout = new QStackedLayout;
	mStackedLayout->addWidget( ( mStackedWidget = new QStackedWidget ) );
	mStackedLayout->addWidget( ( mMdiAreaWidget = new QMdiArea ) );

	// main layout
	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	mLayout->setSpacing( 0 );
	mLayout->setMargin( 0 );
	mLayout->addLayout( mTabLayout );
    mLayout->addLayout( mStackedLayout );

	connect( mMdiAreaWidget, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( setCurrentDocument( QMdiSubWindow* ) ) );
	
	// init view
	setAttribute( Qt::WA_DeleteOnClose );
	//mTabBar->setDrawBase( false );
	//mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );
	
	mDocMode = (DocumentMode)-1; //for avoid return on start method setDocMode (m)
	setDocMode (m);
}

pTabbedWorkspace::~pTabbedWorkspace()
{
	// close all document
	closeAllDocuments();

	// delete container
	delete mMdiAreaWidget;
}

bool pTabbedWorkspace::eventFilter( QObject* o, QEvent* e )
{
	// get event type
	QEvent::Type t = e->type();
	
	// get document
	QWidget* td = qobject_cast<QWidget*>( o );
	
	// child modified state
	if (t == QEvent::ModifiedChange && td && (indexOf (td) != -1))
		emit modifiedChanged (indexOf (td), td->isWindowModified());

	// remove document from workspace
	else if ( t == QEvent::Close && td && (indexOf (td) != -1))
	{
		closeDocument (td);
		return true;
	}
	else if (mDocMode == dmTopLevel && 
			 t == QEvent::WindowActivate && 
			 td &&
             (indexOf (td) != -1))
		setCurrentDocument (td);

	// return default event filter
	return QWidget::eventFilter( o, e );
}

pTabBar* pTabbedWorkspace::tabBar() const
{ return NULL;}//mTabBar; }

pFilesListWidget* pTabbedWorkspace::listWidget() const
{ return mFilesList; }

pTabbedWorkspace::DocumentMode pTabbedWorkspace::docMode() const
{ return mDocMode; }

QTabBar::Shape pTabbedWorkspace::tabShape () const
{ return (QTabBar::Shape)0;}//mTabBar->shape(); }

QWidgetList pTabbedWorkspace::documents() const
{ return mDocuments; }

QWidget* pTabbedWorkspace::document(int i) const
{ return mDocuments[i]; }

int pTabbedWorkspace::count() const
{ return mDocuments.size(); }

int pTabbedWorkspace::currentIndex() const
{ return mCurrIndex; }

QWidget* pTabbedWorkspace::currentDocument() const
{
	if (count ())
    	return document (mCurrIndex);
	else
		return NULL;
}

int pTabbedWorkspace::indexOf(QWidget* w) const
{ return mDocuments.indexOf (w); }

void pTabbedWorkspace::setBackground( const QPixmap& p )
{ mMdiAreaWidget->setBackground( QBrush( p ) ); }

void pTabbedWorkspace::setBackground( const QString& s )
{ mMdiAreaWidget->setBackground( QBrush( QPixmap( s ) ) ); }

int pTabbedWorkspace::addDocument(QWidget* td, const QString& s,  const QIcon& i)
{
	return insertDocument (count(), td, s, i);
}

int pTabbedWorkspace::insertDocument(int pos, QWidget* td, const QString& s,  const QIcon& i)
{
	// filter the document
	td->installEventFilter( this );
	td->setAttribute (Qt::WA_DeleteOnClose, true);

	// append to document list
	mDocuments.insert( pos, td );
	
	switch ( mDocMode )
	{
	case dmSDI:
		mStackedLayout->setCurrentWidget( mStackedWidget );
		mStackedWidget->addWidget( td );
		//mStackedWidget->setCurrentWidget (td);
		break;
	case dmMDI:
		mMdiAreaWidget->addSubWindow( td )->showNormal();
		break;
	case dmTopLevel:
		td->setParent( 0 );
		td->addActions (mMainWindow->actions());//not working !!!! FIXME
		td->showNormal();
		break;
	}	
	
	// emit tab inserted
	emit documentInserted( pos, s, i );

	// emit tab current changed
	emit currentChanged( pos );
	
	return pos;
}

void pTabbedWorkspace::closeDocument(int i)
{

	//signal must be processed while widget is exists for avoid crashs
	emit documentClosed( i );
	
	mDocuments[i]->removeEventFilter( this );
	mDocuments[i]->close ();
	
	// remove document
	mDocuments.removeAt(i);
	
	if (i == currentIndex())
	{
        setCurrentIndex (0);
	}
}

void pTabbedWorkspace::closeDocument( QWidget* td )
{
	return closeDocument (indexOf (td));
}

void pTabbedWorkspace::closeAllDocuments()
{
	for (int i = count()-1; i>=0; i--)
		closeDocument (i);
}

void pTabbedWorkspace::closeCurrentDocument()
{
	closeDocument (currentIndex());
}

void pTabbedWorkspace::setDocMode( pTabbedWorkspace::DocumentMode dm )
{
	if ( mDocMode == dm )
		return;
	
	mDocMode = dm;

    if (mDocMode == dmSDI)
        mStackedLayout->setCurrentWidget( mStackedWidget );
    else if (mDocMode ==dmMDI )
		mStackedLayout->setCurrentWidget( mMdiAreaWidget );

	if (!count())
		return;

	// add document to correct workspace
	foreach ( QWidget* td, mDocuments )
	{
		switch ( mDocMode )
		{
		case dmSDI:
            mStackedWidget->addWidget( td );
			//foreach (QAction* act, mMainWindow->actions ()) // not working !!! FIXME
			//	td->removeAction (act);
			break;
		case dmMDI:
			mMdiAreaWidget->addSubWindow( td )->widget()->showNormal();
			//foreach (QAction* act, mMainWindow->actions ())
			//	td->removeAction (act);
			break;
		case dmTopLevel:
			td->setParent( 0 );
			//td->addActions (mMainWindow->actions());
			td->showNormal();
			break;
		}	
	}
	
	//cleanup QMdiArea - remove it's empty QMdiSubWindows
	if ( mDocMode != dmMDI )
		foreach (QMdiSubWindow* sw, mMdiAreaWidget->subWindowList ())
			delete  sw; //if just remove - widget will not be deleted
	// restore current index
	int i = mCurrIndex; //for avoid return from function because index not changed
	mCurrIndex = -1;
	setCurrentIndex( i );
    
	// emit tab mode changed
	emit docModeChanged( mDocMode );
}

void pTabbedWorkspace::setTabShape( QTabBar::Shape s )
{
/*	if ( tabShape() == s )
		return;
	
	// get sizepolicy
	QSizePolicy sp = mTabBar->sizePolicy();
	
	// update view layout
	switch ( s )
	{
	case QTabBar::RoundedNorth:
	case QTabBar::TriangularNorth:
		mLayout->setDirection( QBoxLayout::TopToBottom );
		break;
	case QTabBar::RoundedSouth:
	case QTabBar::TriangularSouth:
		mLayout->setDirection( QBoxLayout::BottomToTop );
		break;
	default:
		Q_ASSERT (0);
	}
	
	// apply tab shape
	mTabBar->setShape( s );
	
	// update corners
	//updateCorners();
	
	// emit shape changed
	emit tabShapeChanged( s );*/
}

void pTabbedWorkspace::setCurrentIndex( int i )
{
	if ( mCurrIndex == i || i < 0)
		return;
	
	mCurrIndex = i;
	
	switch ( mDocMode )
	{
		case dmSDI:
            mStackedWidget->setCurrentWidget (document(mCurrIndex));
		case dmMDI:
			mMdiAreaWidget->setActiveSubWindow( dynamic_cast<QMdiSubWindow*>(document(mCurrIndex)->parent() ));
			break;
		case dmTopLevel:
			document(mCurrIndex)->activateWindow();
			break;
	}
	// emit document change
	emit currentChanged( mCurrIndex );
}

void pTabbedWorkspace::setCurrentDocument( QWidget* d )
{ setCurrentIndex( indexOf( d ) ); }

void pTabbedWorkspace::setCurrentDocument( QMdiSubWindow* d )
{ 
	if (d)
		setCurrentIndex( indexOf( d->widget() ) ); 
}

void pTabbedWorkspace::activateNextDocument()
{
	int currIndex = currentIndex();
	if ( currIndex +1 == count() )
		setCurrentIndex( 0 );
	else
		setCurrentIndex( currIndex +1 );
}

void pTabbedWorkspace::activatePreviousDocument()
{
	int currIndex = currentIndex();
	if ( currIndex -1 == -1 )
		setCurrentIndex( count() -1 );
	else
		setCurrentIndex( currIndex -1 );
}

void pTabbedWorkspace::setSDI ()
{
    setDocMode (dmSDI);
}

void pTabbedWorkspace::setMDI ()
{
    setDocMode (dmMDI);
}

void pTabbedWorkspace::setTopLevel ()
{
    setDocMode (dmTopLevel);
}

void pTabbedWorkspace::cascade ()
{
    setDocMode (dmMDI);
    mMdiAreaWidget->cascadeSubWindows ();
};

void pTabbedWorkspace::tile ()
{
    setDocMode (dmMDI);
    mMdiAreaWidget->tileSubWindows ();
};

void pTabbedWorkspace::minimize ()
{
    foreach (QWidget* w, mDocuments)
        w->showMinimized ();  //WTF ???
};

void pTabbedWorkspace::restore ()
{
    foreach (QWidget* w, mDocuments)
        w->showNormal();
};
