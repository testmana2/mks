#include "QMakeProjectProxy.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
#include "MenuBar.h"
#include "UIProjectSettingsQMake.h"
//
#include <QMenu>
#include <QAbstractItemView>
//
QMakeProjectProxy::QMakeProjectProxy( QMakeProjectItemModel* s )
	: AbstractProjectProxy( s ), mSource( s )
{
	setSourceModel( s );
}
//
AbstractProjectItemModel* QMakeProjectProxy::project() const
{
	return mSource;
}
//
bool QMakeProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !i.isValid() )
		return true;
	// got the needed index
	QModelIndex index = i;
	if ( mSource->hasChildren( i ) )
		index = i.child( r, i.column() );	
	// checking is in settingsView
	if ( isSettingsView() )
	{
		if ( index.data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::Value )
			return false;
		return true;
	}
	// checking is complex model
	else if ( isComplexModel() )
		return true;
	// apply filtering
	switch ( index.data( QMakeProjectItem::TypeRole ).toInt() )
	{
	case QMakeProjectItem::Project:
	case QMakeProjectItem::Folder:
	case QMakeProjectItem::Value:
	case QMakeProjectItem::File:
		return true;
		break;
	case QMakeProjectItem::Scope:
	{
		for ( int j = 0; j < mSource->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
		return false;
		break;
	}
	case QMakeProjectItem::Variable:
		return QMakeProjectItemModel::simpleModelVariables().contains( index.data().toString(), Qt::CaseInsensitive );
		break;
	}
	return false;
}
//
void QMakeProjectProxy::doubleClicked( const QModelIndex& i )
{
	QModelIndex t = mapToSource( i );
	QMakeProjectItem* it = static_cast<QMakeProjectItem*>( mSource->itemFromIndex( t ) );
	if ( it && it->type() == QMakeProjectItem::File )
		emit fileOpenRequested( it->data( QMakeProjectItem::AbsoluteFilePathRole ).toString() , this );
}
//
void QMakeProjectProxy::customContextMenuRequested( const QPoint& p )
{
	QAbstractItemView* v = qobject_cast<QAbstractItemView*>( sender() );
	if ( !v )
		return;
	QMenu* sm;
	QMenu m( v );
	// project menu
	m.addMenu( MenuBar::self()->menu( "mProject" ) );
	m.addSeparator();
	// show menu
	sm = m.addMenu( QIcon( ":/Icons/Icons/projectshow.png" ), tr( "&Show" ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "&Source" ), this, SLOT( showSource() ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "&ToDo" ), this, SLOT( showToDo() ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "&Changes" ), this, SLOT( showChanges() ) );
	// add menu
	sm = m.addMenu( QIcon( ":/Icons/Icons/projectadd.png" ), tr( "&Add" ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectaddform.png" ), tr( "New F&orm..." ), this, SLOT( addNewForm() ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectaddfile.png" ), tr( "New &Files..." ), this, SLOT( addNewFile() ) );
	sm->addAction( QIcon( ":/Icons/Icons/projectaddtemplate.png" ), tr( "New &Templates" ), this, SLOT( addNewTemplate() ) );
	sm->addSeparator();
	sm->addAction( QIcon( ":/Icons/Icons/projectaddfile.png" ), tr( "&Existing Files..." ), this, SLOT( addExistingFile() ) );
	m.addSeparator();
	// actions
	m.addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "Re&name..." ), this, SLOT( rename() ), tr( "Shift+*" ) );
	m.addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "&Move..." ), this, SLOT( move() ), tr( "Shift+/" ) );
	m.addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "Move To &Scope..." ), this, SLOT( moveToScope() ), tr( "Shift++" ) );
	m.addSeparator();
	m.addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "&Remove..." ), this, SLOT( remove() ), tr( "Del" ) );
	m.addAction( QIcon( ":/Icons/Icons/projectshowfile.png" ), tr( "Remove && &Delete..." ), this, SLOT( removeAndDelete() ), tr( "Shift+Del" ) );
	m.exec( v->mapToGlobal( p ) );
	/*
	action( "alupdate", tr( "l&update" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Execute lupdate on current" ) );
	action( "alrelease", tr( "l&release" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Execute lrelease on current" ) );
	*/
}
//
void QMakeProjectProxy::projectSettings()
{
	setSettingsView( true );
	UIProjectSettingsQMake::execute( this );
	setSettingsView( false );
}
