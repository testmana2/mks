#include "ProjectsManager.h"
#include "MenuBar.h"
#include "AbstractProjectProxy.h"
#include "AbstractProjectModel.h"
#include "Settings.h"
#include "PluginsManager.h"
#include "RecentsManager.h"
#include "ProjectPlugin.h"
//
#include <QFileDialog>
#include <QTreeView>
//
Q_DECLARE_METATYPE( QModelIndex )
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
	sProjects->setStretchFactor( sProjects->indexOf( swProjects ), 1 );
	twProjects->setMinimumHeight( 50 );
	sProjects->setSizes( QList<int>() << twProjects->minimumHeight() );
	//
	aProjectsList = new QAction( QIcon( ":/Icons/Icons/helpgreettings.png" ), tr( "Projects list" ), this );
	aFilteredView = new QAction( QIcon( ":/Icons/Icons/helptesters.png" ), tr( "Filter project" ), this );
	aProjectsList->setCheckable( true );
	aProjectsList->setChecked( true );
	aProjectsList->setToolTip( tr( "Show/Hide the projects list" ) );
	aFilteredView->setCheckable( true );
	aFilteredView->setToolTip( tr( "Filtered project view" ) );
	//
	tbActions->layout()->setAlignment( Qt::AlignRight );
	tbActions->addAction( aProjectsList );
	tbActions->addAction( MenuBar::self()->action( "mProject/aOpen" ) );
	tbActions->addAction( aFilteredView );
	tbActions->addActions( MenuBar::self()->menu( "mProject/mSave" )->actions() );
	tbActions->addActions( MenuBar::self()->menu( "mProject/mClose" )->actions() );
	tbActions->addSeparator();
	tbActions->addAction( MenuBar::self()->action( "mProject/aSettings" ) );
	// connections
	connect( aProjectsList, SIGNAL( triggered( bool ) ), twProjects, SLOT( setVisible( bool ) ) );
	connect( aFilteredView, SIGNAL( triggered( bool ) ), this, SLOT( setFilteredModel( bool ) ) );
	// initialize actions
	setCurrentProject( QModelIndex() );
}
//
void ProjectsManager::showEvent( QShowEvent* e )
{
	// restore spliter state
	QDockWidget::showEvent( e );
	sProjects->restoreState( Settings::current()->value( "ProjectsManager/State" ).toByteArray() );
}
//
void ProjectsManager::closeEvent( QCloseEvent* e )
{
	// save spliter state
	Settings::current()->setValue( "ProjectsManager/State", sProjects->saveState() );
	QDockWidget::closeEvent( e );
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
						b =  pp->openProject( mFilePath );
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
	foreach ( AbstractProjectModel* p, AbstractProjectModel::all() )
		p->saveAll();
}
//
void ProjectsManager::closeCurrent()
{
	closeProject( currentProject() );
}
//
void ProjectsManager::closeAll()
{
	foreach ( AbstractProjectProxy* p, AbstractProjectProxy::all() )
		closeProject( p->project()->rootProject() );
}
//
void ProjectsManager::projectSettings()
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
		currentProxy()->projectSettings( currentProject() );
}
//
void ProjectsManager::addProxy( AbstractProjectProxy* p )
{
	if ( !p )
		return;
	// build project tree list
	buildProjectTreeItems( p, p->project()->rootProject() );
	// create a view for proxy
	QTreeView* tv = new QTreeView;
	tv->setEditTriggers( QAbstractItemView::NoEditTriggers );
	tv->setContextMenuPolicy( Qt::CustomContextMenu );
	tv->setModel( p );
	swProjects->addWidget( tv );
	setCurrentProject( p->project()->rootProject() );
	// if there is only one top project, hide projects list
	bool b = twProjects->topLevelItemCount() > 1 || ( twProjects->topLevelItemCount() == 1 && twProjects->topLevelItem( 0 )->childCount() );
	if ( twProjects->isVisible() != b )
		aProjectsList->trigger();
	// connect
	connect( p->project(), SIGNAL( isModifiedChanged( bool, const QModelIndex& ) ), this, SLOT( projectIsModified( bool, const QModelIndex& ) ) );
	connect( p->project(), SIGNAL( aboutToClose( const QModelIndex& ) ), this, SLOT( projectAboutToClose( const QModelIndex& ) ) );
	//connect( tv, SIGNAL( doubleClicked( const QModelIndex& ) ), p, SLOT( doubleClicked( const QModelIndex& ) ) );
	//connect( tv, SIGNAL( customContextMenuRequested( const QPoint& ) ), p, SLOT( customContextMenuRequested( const QPoint& ) ) );
	//connect( p, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ), this, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ) );
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
	foreach ( QAbstractItemView* v, swProjects->findChildren<QAbstractItemView*>() )
		if ( v->model() == p )
			return v;
	return 0;
}
//
AbstractProjectProxy* ProjectsManager::currentProxy() const
{
	QAbstractItemView* v = qobject_cast<QAbstractItemView*>( swProjects->currentWidget() );
	return v ? qobject_cast<AbstractProjectProxy*>( v->model() ) : 0;
}
//
AbstractProjectModel* ProjectsManager::currentModel() const
{
	AbstractProjectProxy* p = currentProxy();
	return p ? p->project() : 0;
}
//
void ProjectsManager::setCurrentProject( const QModelIndex& i ) // index is not proxied
{
	// get proxy
	AbstractProjectProxy* p = modelByIndex( i )->findChild<AbstractProjectProxy*>();
	if ( p && currentProject() != i )
	{
		QAbstractItemView* v = viewByProxy( p );
		// if current proxy is different, change it
		if ( currentProxy() != p )
			swProjects->setCurrentWidget( v );
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
		return m->project( p->mapToSource( viewByProxy( p )->currentIndex() ) );
	}
	return QModelIndex();
}
//
void ProjectsManager::closeProject( const QModelIndex& i )
{
	AbstractProjectModel* m = i.isValid() ? modelByIndex( i ) : 0;
	if ( m )
		m->close( i );
}
//
void ProjectsManager::projectIsModified( bool b, const QModelIndex& i )
{
	if ( !i.isValid() )
		return;
	MenuBar::self()->action( "mProject/mSave/aCurrent" )->setEnabled( b );
	AbstractProjectModel* m = modelByIndex( i );
	QFont f( i.data( Qt::FontRole ).value<QFont>() );
	f.setBold( b );
	m->setData( i, f, Qt::FontRole );
	m->setData( i, b ? Qt::red : Qt::black, Qt::ForegroundRole );
}
//
void ProjectsManager::projectAboutToClose( const QModelIndex& i )
{
	// get model
	AbstractProjectModel* m = i.isValid() ? modelByIndex( i ) : 0;
	if ( m )
	{
		// remove project item from project list
		removeProjectItem( i );
		// delete view if needed
		if ( m->rootProject() == i )
			delete viewByProxy( m->findChild<AbstractProjectProxy*>() );
		// set new current project
		updateProjectActions( currentProject() );
	}
}
//
#include <QDebug>
void ProjectsManager::buildProjectTreeItems( AbstractProjectProxy* p, const QModelIndex& i, QTreeWidgetItem* twi )
{
	if ( !i.isValid() )
		return;
	twi = twi ? new QTreeWidgetItem( twi ) : new QTreeWidgetItem( twProjects );
	twi->setText( 0, i.data().toString() );
	twi->setIcon( 0, i.data( Qt::DecorationRole ).value<QPixmap>() );
	twi->setData( 0, AbstractProjectModel::ProjectIdRole, p->project()->id() );
	twi->setData( 0, AbstractProjectModel::ProxyIdRole, p->id() );
	twi->setData( 0, AbstractProjectModel::AbsoluteFilePathRole, p->project()->filePath( i ) );
	twi->setData( 0, AbstractProjectModel::FirstRole, QVariant::fromValue( i ) );
	twi->setToolTip( 0, tr( "Proxy Id: %1\nProject Id: %2\nFile Path: %3" ).arg( p->id() ).arg( p->project()->id() ).arg( p->project()->filePath( i ) ) );
	foreach ( QModelIndex j, p->project()->subProjects( i ) )
		buildProjectTreeItems( p, j, twi );
}
//
void ProjectsManager::updateProjectActions( const QModelIndex& i ) // index is not proxied
{
	AbstractProjectProxy* p = modelByIndex( i )->findChild<AbstractProjectProxy*>();
	// enable / disable, check / uncheck action according to proxy
	aFilteredView->setEnabled( p );
	aFilteredView->setChecked( p ? p->isFiltering() : false );
	MenuBar::self()->action( "mProject/mSave/aCurrent" )->setEnabled( p ? p->project()->isModified( i ) : false );
	MenuBar::self()->action( "mProject/mSave/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aCurrent" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/aSettings" )->setEnabled( p );
	// set correct item in project list
	if ( p )
	{
		foreach ( QModelIndex j, twProjects->model()->match( twProjects->model()->index( 0, 0 ), AbstractProjectModel::ProxyIdRole, p->id(), -1, Qt::MatchFixedString | Qt::MatchRecursive ) )
		{
			if ( j.data( AbstractProjectModel::FirstRole ).value<QModelIndex>() == i )
			{
				twProjects->setCurrentIndex( j );
				break;
			}
		}
	}
}
//
void ProjectsManager::removeProjectItem( const QModelIndex& i )
{
	AbstractProjectProxy* p = modelByIndex( i )->findChild<AbstractProjectProxy*>();
	if ( !p )
		return;
	foreach ( QModelIndex j, twProjects->model()->match( twProjects->model()->index( 0, 0 ), AbstractProjectModel::ProxyIdRole, p->id(), -1, Qt::MatchFixedString | Qt::MatchRecursive ) )
	{
		if ( j.data( AbstractProjectModel::FirstRole ).value<QModelIndex>() == i )
		{
			twProjects->model()->removeRow( j.row(), j.parent() );
			return;
		}
	}
}
//
void ProjectsManager::setFilteredModel( bool b )
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
		p->setFiltering( b );
}
//
void ProjectsManager::on_twProjects_itemClicked( QTreeWidgetItem* it, int )
{
	if ( it )
	{
		AbstractProjectProxy* p = AbstractProjectProxy::byId( it->data( 0, AbstractProjectModel::ProxyIdRole ).toInt() );
		swProjects->setCurrentWidget( viewByProxy( p ) );
		setCurrentProject( it->data( 0, AbstractProjectModel::FirstRole ).value<QModelIndex>() );
	}
}
