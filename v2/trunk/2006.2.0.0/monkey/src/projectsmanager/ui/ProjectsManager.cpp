#include "ProjectsManager.h"
#include "MenuBar.h"
#include "AbstractProjectProxy.h"
#include "AbstractProjectItemModel.h"
#include "Settings.h"
//
#include <QFileDialog>
#include <QTreeView>
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
	sProjects->setStretchFactor( sProjects->indexOf( swProjects ), 1 );
	twProjects->setMinimumHeight( 50 );
	sProjects->setSizes( QList<int>() << twProjects->minimumHeight() );
	tbList->setDefaultAction( MenuBar::self()->action( "mView/aProjectsList" ) );
	tbOpen->setDefaultAction( MenuBar::self()->action( "mProject/aOpen" ) );
	tbComplex->setDefaultAction( MenuBar::self()->action( "mView/aComplexProject" ) );
	tbSave->setDefaultAction( MenuBar::self()->menu( "mProject/mSave" )->menuAction() );
	tbClose->setDefaultAction( MenuBar::self()->menu( "mProject/mClose" )->menuAction() );
	// update actions state
	on_swProjects_currentChanged( -1 );
	MenuBar::self()->action( "mView/aProjectsList" )->setChecked( true );
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
QProxyList ProjectsManager::rootProxies() const
{
	QProxyList l;
	for ( int i = 0; i < twProjects->topLevelItemCount(); i++ )
		l << AbstractProjectProxy::byId( twProjects->topLevelItem( i )->data( 0, ProxyIdRole ).toInt() );
	return l;
}
//
QProjectList ProjectsManager::rootProjects() const
{
	QProjectList l;
	foreach ( AbstractProjectProxy* p, rootProxies() )
		l << p->project();
	return l;
}
//
void ProjectsManager::setCurrentProxy( AbstractProjectProxy* p )
{
	if ( !p )
		return;
	swProjects->setCurrentWidget( getViewByProxyId( p->id() ) );
	if ( twProjects->currentItem() &&
		twProjects->currentItem()->data( 0, ProxyIdRole ).toInt() != p->id() )
		twProjects->setCurrentItem( getItemByProxyId( p->id() ) );
}
//
AbstractProjectProxy* ProjectsManager::currentProxy() const
{
	if ( swProjects->count() )
	{
		QAbstractItemView* v = qobject_cast<QAbstractItemView*>( swProjects->currentWidget() );
		if ( v )
			return qobject_cast<AbstractProjectProxy*>( v->model() );
	}
	return 0;
}
//
void ProjectsManager::setCurrentProject( AbstractProjectItemModel* p )
{
	foreach ( AbstractProjectProxy* py, AbstractProjectProxy::all() )
	{
		if ( py->project() == p )
		{
			setCurrentProxy( py );
			return;
		}
	}
}
//
AbstractProjectItemModel* ProjectsManager::currentProject() const
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
		return p->project();
	return 0;
}
//
QTreeWidgetItem* ProjectsManager::getItemByProxyId( int i )
{
	AbstractProjectProxy* p = AbstractProjectProxy::byId( i );
	if ( !p )
		return 0;
	QList<QTreeWidgetItem*> l = twProjects->findItems( p->project()->name(), Qt::MatchFixedString | Qt::MatchRecursive );
	foreach ( QTreeWidgetItem* i, l )
		if ( i->data( 0, ProxyIdRole ).toInt() == p->id() )
			return i;
	return 0;
}
//
QAbstractItemView* ProjectsManager::getViewByProxyId( int i )
{
	QList<QAbstractItemView*> l = swProjects->findChildren<QAbstractItemView*>();
	foreach ( QAbstractItemView* v, l )
		if ( v->model() == AbstractProjectProxy::byId( i ) )
			return v;
	return 0;
}
//
void ProjectsManager::closeProxy( AbstractProjectProxy* p )
{
	QTreeWidgetItem* twi;
	if ( !p || !( twi = getItemByProxyId( p->id() ) ) )
		return;
	const QString s = p->project()->name();
	// close childs
	while ( twi->childCount() )
		closeProxy( AbstractProjectProxy::byId( twi->child( 0 )->data( 0, ProxyIdRole ).toInt() ) );
	// delete view, project, proxy and item
	delete getViewByProxyId( p->id() );
	delete p->sourceModel(); // as project is the sourceModel ( child ) of proxy, it delete proxy too
	delete twi;
	// update tool button if there is no project
	if ( !swProjects->count() )
		on_swProjects_currentChanged( -1 );
}
//
void ProjectsManager::closeProject( AbstractProjectItemModel* p )
{
	if ( p )
		closeProxy( AbstractProjectProxy::getProxyByProject( p ) );
}
//
void ProjectsManager::addProxy( AbstractProjectProxy* p, AbstractProjectProxy* pParent )
{
	if ( !p )
		return;
	// connect close signal with deletion 
	connect( p->project(), SIGNAL( isModifiedChanged( bool ) ), MenuBar::self()->action( "mProject/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
	// create a view for proxy
	QTreeView* tv = new QTreeView;
	tv->setEditTriggers( QAbstractItemView::NoEditTriggers );
	tv->setContextMenuPolicy( Qt::CustomContextMenu );
	// connect view
	connect( tv, SIGNAL( doubleClicked( const QModelIndex& ) ), p, SLOT( doubleClicked( const QModelIndex& ) ) );
	connect( tv, SIGNAL( customContextMenuRequested( const QPoint& ) ), p, SLOT( customContextMenuRequested( const QPoint& ) ) );
	connect( p, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ), this, SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ) );
	// set model
	tv->setModel( p );
	if ( p->rowCount() > 0 )
		tv->setRootIndex( p->index( 0, 0 ) );
	// set current view this one
	swProjects->addWidget( tv );
	swProjects->setCurrentWidget( tv );
	// add entry on twProjects
	QTreeWidgetItem* twi;
	if ( pParent )
		twi = new QTreeWidgetItem( getItemByProxyId( pParent->id() ) );
	else
		twi = new QTreeWidgetItem( twProjects );
	twi->setText( 0, p->project()->name() );
	twi->setIcon( 0, QIcon( ":/icons/icons/project.png" ) );
	twi->setData( 0, ProxyIdRole, p->id() ); // proxy id
	twi->setData( 0, ProjectFilePathRole, p->project()->filePath() ); // project file path
	twi->setToolTip( 0, tr( "Proxy Id: %1\nProject Id: %2\nFile Path: %3" ).arg( p->id() ).arg( p->project()->id() ).arg( p->project()->filePath() ) );
	twProjects->setCurrentItem( twi );
	// emit signal
	emit proxyAdded( p );
}
//
void ProjectsManager::setTreeProjectsVisible( bool b )
{
	twProjects->setVisible( b );
}
//
void ProjectsManager::setComplexModel( bool b )
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
		p->setComplexModel( b );
}
//
void ProjectsManager::on_tbSave_clicked()
{
	QAction* a = MenuBar::self()->action( "mProject/mSave/aCurrent" );
	if ( a->isEnabled() )
		a->trigger();
}
//
void ProjectsManager::on_tbClose_clicked()
{
	QAction* a = MenuBar::self()->action( "mProject/mClose/aCurrent" );
	if ( a->isEnabled() )
		a->trigger();
}
//
void ProjectsManager::on_twProjects_itemClicked( QTreeWidgetItem* i, int )
{
	if ( !i )
		return;
	AbstractProjectProxy* p = AbstractProjectProxy::byId( i->data( 0, ProxyIdRole ).toInt() );
	setCurrentProxy( p );
	emit currentProxyChanged( p );
}
//
void ProjectsManager::on_swProjects_currentChanged( int )
{
	AbstractProjectProxy* p = currentProxy();
	// enable / disable, check / uncheck action according to proxy
	MenuBar::self()->action( "mView/aComplexProject" )->setEnabled( p );
	MenuBar::self()->action( "mView/aComplexProject" )->setChecked( p ? p->isComplexModel() : false );
	MenuBar::self()->action( "mProject/mSave/aCurrent" )->setEnabled( p ? p->project()->isModified() : false );
	MenuBar::self()->action( "mProject/mSave/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aCurrent" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mClose/aAll" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mShow/aSource" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mShow/aToDo" )->setEnabled( p );
	MenuBar::self()->action( "mProject/mShow/aChanges" )->setEnabled( p );
	MenuBar::self()->action( "mProject/aSettings" )->setEnabled( p );
}
