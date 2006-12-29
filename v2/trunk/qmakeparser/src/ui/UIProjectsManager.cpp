#include "UIProjectsManager.h"
#include "AbstractProjectProxy.h"
#include "AbstractProjectItemModel.h"
#include "QMakeProjectProxy.h"
#include "QMakeProjectItemModel.h"
//
#include <QFileDialog>
#include <QTreeView>
//
QPointer<UIProjectsManager> UIProjectsManager::mSelf = 0L;
//
UIProjectsManager* UIProjectsManager::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIProjectsManager( p );
	return mSelf;
}
//
UIProjectsManager::UIProjectsManager( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
}
//
void UIProjectsManager::setCurrentProxy( AbstractProjectProxy* p )
{
	if ( !p )
		return;
	swProjects->setCurrentWidget( getViewByProxyId( p->id() ) );
	if ( twProjects->currentItem() &&
		twProjects->currentItem()->data( 0, ProxyIdRole ).toInt() != p->id() )
		twProjects->setCurrentItem( getItemByProxyId( p->id() ) );
}
//
AbstractProjectProxy* UIProjectsManager::currentProxy() const
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
QTreeWidgetItem* UIProjectsManager::getItemByProxyId( int i )
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
QAbstractItemView* UIProjectsManager::getViewByProxyId( int i )
{
	QList<QAbstractItemView*> l = swProjects->findChildren<QAbstractItemView*>();
	foreach ( QAbstractItemView* v, l )
		if ( v->model() == AbstractProjectProxy::byId( i ) )
			return v;
	return 0;
}
//
bool UIProjectsManager::openProject( const QString& s, int i )
{
	// create proxy and project
	AbstractProjectProxy* mProxy = new QMakeProjectProxy( new QMakeProjectItemModel( s, this ) );
	// open project
	if ( mProxy->project()->open() )
	{
		// create view and project
		QTreeView* tv = new QTreeView;
		tv->setModel( mProxy );
		if ( mProxy->rowCount() > 0 )
			tv->setRootIndex( mProxy->index( 0, 0 ) );
		// set current view this one
		swProjects->addWidget( tv );
		swProjects->setCurrentWidget( tv );
		// add entry on twProjects
		QTreeWidgetItem* twi;
		if ( i != -1 )
			twi = new QTreeWidgetItem( getItemByProxyId( i ) );
		else
			twi = new QTreeWidgetItem( twProjects );
		twi->setText( 0, mProxy->project()->name() );
		twi->setIcon( 0, QIcon( ":/icons/icons/project.png" ) );
		twi->setData( 0, ProxyIdRole, mProxy->id() ); // proxy id
		twi->setData( 0, ProjectFilePathRole, mProxy->project()->filePath() ); // project file path
		twi->setToolTip( 0, tr( "Proxy Id: %1\nProject Id: %2\nFile Path: %3" ).arg( mProxy->id() ).arg( mProxy->project()->id() ).arg( mProxy->project()->filePath() ) );
		twProjects->setCurrentItem( twi );
		// open subprojects
		foreach ( QString h, mProxy->project()->subProjects() )
			openProject( h, mProxy->id() );
		return true;
	}
	else
		delete mProxy;
	return false;
}
//
void UIProjectsManager::closeProject( QTreeWidgetItem* i )
{
	// if no item do nothing
	if ( !i )
		return;
	// close childs
	while ( i->childCount() )
		closeProject( i->child( 0 ) );
	// delete view and item
	delete getViewByProxyId( i->data( 0, ProxyIdRole ).toInt() );
	delete i;
}
//
void UIProjectsManager::on_tbOpen_clicked()
{
	QString mFilePath = QFileDialog::getOpenFileName( this, tr( "Open Qt Project" ), QDir::homePath().append( "/Desktop" ), tr( "Qt Projects (*.pro)" ) );
	if ( !mFilePath.isNull() )
	{
		// got clean and absolute file path
		mFilePath = QFileInfo( mFilePath ).canonicalFilePath();
		// check if file is already open
		foreach ( AbstractProjectProxy* p, AbstractProjectProxy::all() )
		{
			if ( p->project()->filePath() == mFilePath )
			{
				setCurrentProxy( p );
				return;
			}
		}
		// open project
		if ( openProject( mFilePath ) )
		{
			// add entry to recents projects
		}
	}
}
//
void UIProjectsManager::on_tbComplexCurrent_clicked()
{
	currentProxy()->setComplexModel( tbComplexCurrent->isChecked() );
}
//
void UIProjectsManager::on_tbSaveCurrent_clicked()
{
	currentProxy()->project()->save();
}
//
void UIProjectsManager::on_tbCloseCurrent_clicked()
{
	closeProject( getItemByProxyId( currentProxy()->id() ) );
	if ( !swProjects->count() )
		on_swProjects_currentChanged( -1 );
}
//
void UIProjectsManager::on_twProjects_itemClicked( QTreeWidgetItem* i, int )
{
	if ( !i )
		return;
	setCurrentProxy( AbstractProjectProxy::byId( i->data( 0, ProxyIdRole ).toInt() ) );
}
//
void UIProjectsManager::on_swProjects_currentChanged( int )
{
	AbstractProjectProxy* p = currentProxy();
	if ( p )
	{
		tbComplexCurrent->setChecked( p->isComplexModel() );
		tbSaveCurrent->setEnabled( p->project()->isModified() );
	}
	else
	{
		tbComplexCurrent->setChecked( false );
		tbSaveCurrent->setEnabled( false );
	}
	tbComplexCurrent->setEnabled( p );
	tbCloseCurrent->setEnabled( p );
}
