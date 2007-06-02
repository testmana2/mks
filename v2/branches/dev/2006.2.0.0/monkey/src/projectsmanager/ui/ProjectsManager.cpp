#include "ProjectsManager.h"
#include "MenuBar.h"
#include "AbstractProjectProxy.h"
#include "AbstractProjectModel.h"
#include "Settings.h"
#include "PluginsManager.h"
#include "RecentsManager.h"
#include "ProjectPlugin.h"
//
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QTreeView>
#include <QHeaderView>
//
QPointer<ProjectsManager> ProjectsManager::mSelf = 0L;
//
ProjectsManager* ProjectsManager::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new ProjectsManager( p );
	return mSelf;
}
//
ProjectsManager::ProjectsManager( QWidget* p )
	: QDockWidget( p )
{
	setupUi( this );
	setMinimumWidth( 225 );
	//
	QAction* a = MenuBar::self()->action( "mView/aFilteredView" );
	a->setCheckable( true );
	//
	tbActions->layout()->setAlignment( Qt::AlignRight );
	tbActions->addAction( MenuBar::self()->action( "mProject/aOpen" ) );
	tbActions->addAction( a );
	tbActions->addActions( MenuBar::self()->menu( "mProject/mSave" )->actions() );
	tbActions->addActions( MenuBar::self()->menu( "mProject/mClose" )->actions() );
	tbActions->addSeparator();
	tbActions->addAction( MenuBar::self()->action( "mProject/aSettings" ) );
	// connections
	connect( a, SIGNAL( triggered( bool ) ), this, SLOT( setFiltering( bool ) ) );
	// set current project to nothing
	setCurrentProject( QModelIndex() );
}
//
QStringList ProjectsManager::projectsFilters() const
{
	QStringList l;
	foreach ( BasePlugin* bp, PluginsManager::self()->plugins() )
	{
		if ( bp->infos().Type == BasePlugin::iProject )
		{
			ProjectPlugin* pp = (ProjectPlugin*)bp;
			if ( pp && pp->infos().Installed )
				l << pp->filters();
		}
	}
	return l;
}
//
void ProjectsManager::newProject()
{
	qWarning( "ProjectsManager::newProject : not yet available" );
}
//
bool ProjectsManager::openProject( const QString& s )
{
	bool b = false;
	QString mFilePath = s;
	// if no file ask to open one
	if ( mFilePath.isNull() )
	{
		const QString mPath = Settings::current()->value( "Recents/ProjectOpenPath" ).toString();
		QString mFilters = projectsFilters().join( ";;" );
		mFilePath = QFileDialog::getOpenFileName( this, tr( "Choose the project to open" ), mPath, mFilters );
	}
	// if file, open it
	if ( !mFilePath.isNull() )
	{
		const QFileInfo f( mFilePath );
		mFilePath = f.canonicalFilePath();
		if ( !f.exists() )
		{
			// remove it from recents projects
			RecentsManager::self()->removeRecentProject( mFilePath );
			return false;
		}
		else
		{
			QString mExtension = f.completeSuffix();
			foreach ( BasePlugin* bp, PluginsManager::self()->plugins() )
			{
				if ( bp->infos().Type == BasePlugin::iProject )
				{
					ProjectPlugin* pp = (ProjectPlugin*)bp;
					if ( pp && pp->infos().Installed && pp->extensions().contains( mExtension, Qt::CaseInsensitive ) )
					{
						b = pp->openProject( mFilePath );
						break;
					}
				}
			}
			// if success
			if ( b )
			{
				// save recent project open path
				Settings::current()->setValue( "Recents/ProjectOpenPath", f.canonicalPath() );
				// append it to recents
				RecentsManager::self()->addRecentProject( mFilePath );
			}
		}
	}
	return b;
}
//
void ProjectsManager::saveCurrent()
{
	AbstractProjectModel* m = currentModel();
	if ( m )
		m->save( currentProject() );
}
//
void ProjectsManager::saveAll()
{
	AbstractProjectModel* m = currentModel();
	if ( m )
		m->saveAll();
}
//
void ProjectsManager::closeCurrent()
{
	AbstractProjectModel* m = currentModel();
	if ( m )
		m->close( currentProject() );
}
//
void ProjectsManager::closeAll()
{
	AbstractProjectModel* m = currentModel();
	if ( m )
		m->close( m->rootProject() );
}
//
void ProjectsManager::projectSettings()
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
	{
		saveCurrent();
		currentProxy()->projectSettings( currentProject() );
	}
}
//
void ProjectsManager::addProxy( AbstractProjectProxy* p, QAbstractItemView* v )
{
	if ( !p )
		return;
	// create a view for proxy if require
	if ( !v )
	{
		v = new QTreeView;
		qobject_cast<QTreeView*>( v )->header()->hide();
		// set view propreties
		v->setEditTriggers( QAbstractItemView::NoEditTriggers );
		v->setContextMenuPolicy( Qt::CustomContextMenu );
		v->setModel( p );
		// connect
		connect( v, SIGNAL( doubleClicked( const QModelIndex& ) ), p, SLOT( doubleClicked( const QModelIndex& ) ) );
 		connect( v, SIGNAL( clicked (const QModelIndex)),p, SLOT ( clicked (const QModelIndex)));
		connect( v, SIGNAL( customContextMenuRequested( const QPoint& ) ), p, SLOT( customContextMenuRequested( const QPoint& ) ) );
	}
	// connect
	connect( p->project(), SIGNAL( isModifiedChanged( bool, const QModelIndex& ) ), this, SLOT( projectIsModified( bool, const QModelIndex& ) ) );
	connect( p->project(), SIGNAL( aboutToClose( const QModelIndex& ) ), this, SLOT( projectAboutToClose( const QModelIndex& ) ) );
	connect( v, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( view_clicked( const QModelIndex& ) ) );
	connect( p, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ), this, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ) );
	// add to project list
	tbProjects->addItem( v, p->project()->rootProject().data( Qt::DecorationRole ).value<QPixmap>(), p->project()->rootProject().data().toString() );
	setCurrentProject( p->project()->rootProject() );
	// emit signal
	emit proxyAdded( p );
}
//
AbstractProjectModel* ProjectsManager::modelByIndex( const QModelIndex& i ) const
{
	return qobject_cast<AbstractProjectModel*>( const_cast<QAbstractItemModel*>( i.model() ) );
}
//
AbstractProjectProxy* ProjectsManager::proxyByIndex( const QModelIndex& i ) const
{
	return qobject_cast<AbstractProjectProxy*>( const_cast<QAbstractItemModel*>( i.model() ) );
}
//
QAbstractItemView* ProjectsManager::viewByProxy( AbstractProjectProxy* p ) const
{
	foreach ( QAbstractItemView* v, tbProjects->findChildren<QAbstractItemView*>() )
		if ( v->model() == p )
			return v;
	return 0;
}
//
AbstractProjectProxy* ProjectsManager::currentProxy() const
{
	QAbstractItemView* v = qobject_cast<QAbstractItemView*>( tbProjects->currentWidget() );
	return v ? qobject_cast<AbstractProjectProxy*>( v->model() ) : 0;
}
//
AbstractProjectModel* ProjectsManager::currentModel() const
{
    AbstractProjectProxy* p = currentProxy();
	return p ? p->project() : 0;
}
//
QAbstractItemView* ProjectsManager::currentView() const
{
	return qobject_cast<QAbstractItemView*>( tbProjects->currentWidget() );
}
//
void ProjectsManager::setCurrentProject( const QModelIndex& i ) // index is not proxied
{
	// get proxy
	AbstractProjectProxy* p = modelByIndex( i )->findChild<AbstractProjectProxy*>();
	if ( p )
	{
		QAbstractItemView* v = viewByProxy( p );
		// if current proxy is different, change it
		if ( currentView() != v )
			tbProjects->setCurrentWidget( v );
		v->clearSelection();
		// set correct project
		v->setCurrentIndex( p->mapFromSource( i ) );
	}
	// update actions
	updateProjectActions( i );
}
//
QModelIndex ProjectsManager::currentProject() const // return index is not proxied
{
	AbstractProjectModel* m = currentModel();	
	if ( m )
	{
		AbstractProjectProxy* p = m->findChild<AbstractProjectProxy*>();
		QModelIndex i = m->project( p->mapToSource( viewByProxy( p )->currentIndex() ) );
		if ( !i.isValid() )
			i = m->rootProject();
		return i;
	}
	return QModelIndex();
}
//
void ProjectsManager::setFiltering( bool b )
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
		p->setFiltering( b );
}
//
void ProjectsManager::projectIsModified( bool b, const QModelIndex& i )
{
	MenuBar::self()->action( "mProject/mSave/aCurrent" )->setEnabled( b );
}
//
void ProjectsManager::projectAboutToClose( const QModelIndex& i )
{
	// get model
	AbstractProjectModel* m = modelByIndex( i );
	// delete view if needed
	if ( m && m->rootProject() == i )
		delete viewByProxy( m->findChild<AbstractProjectProxy*>() );
	updateProjectActions( currentProject() );
}
//
void ProjectsManager::updateProjectActions( const QModelIndex& i ) // index is not proxied
{
	AbstractProjectProxy* p = modelByIndex( i )->findChild<AbstractProjectProxy*>();
	// enable / disable, check / uncheck action according to proxy
	QAction* a = MenuBar::self()->action( "mView/aFilteredView" );
	a->setEnabled( p );
	a->setChecked( p ? p->isFiltering() : false );
	MenuBar::self()->action( "mProject/mSave/aCurrent" )->setEnabled( p ? p->project()->isModified( i ) : false );
	MenuBar::self()->action( "mProject/mSave/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aCurrent" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/aSettings" )->setEnabled( p );
}
//
void ProjectsManager::view_clicked( const QModelIndex& i )
{
	if ( i.isValid() )
		setCurrentProject( currentProxy()->mapToSource( i ) );
}
/*
	AbstractProjectModel* m = modelByIndex( i );
	QFont f( i.data( Qt::FontRole ).value<QFont>() );
	f.setBold( b );
	m->setData( i, f, Qt::FontRole );
	m->setData( i, b ? Qt::red : Qt::black, Qt::ForegroundRole );
*/

