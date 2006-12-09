#include "pTabToolBarManager.h"
//
#include <QMainWindow>
#include <QDockWidget>
#include <QSettings>
//
pTabToolBarManager::pTabToolBarManager( QMainWindow* p )
	: QObject( p ), mMain( p )
{
	Q_ASSERT( p != 0 );
}
//
QMainWindow* pTabToolBarManager::mainWindow() const
{
	return mMain;
}
//
pTabToolBar* pTabToolBarManager::bar( pTabToolBarManager::Align a )
{
	if ( !mBars.contains( a ) )
	{
		switch ( a )
		{
		case Top:
			mBars[a] = new pTabToolBar( this, KMultiTabBar::Horizontal );
			mBars[a]->setObjectName( "pTabToolBarTop" );
			mBars[a]->setWindowTitle( tr( "Top TabToolBar" ) );
			mMain->addToolBar( Qt::TopToolBarArea, mBars[a] );
			break;
		case Bottom:
			mBars[a] = new pTabToolBar( this, KMultiTabBar::Horizontal );
			mBars[a]->setObjectName( "pTabToolBarBottom" );
			mBars[a]->setWindowTitle( tr( "Bottom TabToolBar" ) );
			mMain->addToolBar( Qt::BottomToolBarArea, mBars[a] );
			break;
		case Left:
			mBars[a] = new pTabToolBar( this, KMultiTabBar::Vertical );
			mBars[a]->setObjectName( "pTabToolBarLeft" );
			mBars[a]->setWindowTitle( tr( "Left TabToolBar" ) );
			mMain->addToolBar( Qt::LeftToolBarArea, mBars[a] );
			break;
		case Right:
			mBars[a] = new pTabToolBar( this, KMultiTabBar::Vertical );
			mBars[a]->setObjectName( "pTabToolBarRight" );
			mBars[a]->setWindowTitle( tr( "Right TabToolBar" ) );
			mMain->addToolBar( Qt::RightToolBarArea, mBars[a] );
			break;
		}
		mBars.value( a )->setMovable( false );
		connect( mBars.value( a ), SIGNAL( dockWidgetTabToolBarChanged( QDockWidget*, pTabToolBar* ) ), this, SLOT( dockWidgetTabToolBarChanged( QDockWidget*, pTabToolBar* ) ) );
	}
	return mBars.value( a );
}
//
pTabToolBarManager::Align pTabToolBarManager::dockWidgetAreaToAlign( Qt::DockWidgetArea a ) const
{
	switch ( a )
	{
	case Qt::LeftDockWidgetArea:
		return pTabToolBarManager::Left;
		break;
	case Qt::RightDockWidgetArea:
		return pTabToolBarManager::Right;
		break;
	case Qt::TopDockWidgetArea:
		return pTabToolBarManager::Top;
		break;
	case Qt::BottomDockWidgetArea:
		return pTabToolBarManager::Bottom;
		break;
	default:
		return pTabToolBarManager::Bottom;
		break;
	}
}
//
void pTabToolBarManager::restoreState( QSettings* s )
{
	if ( !s )
		return;
	QVariantList mList;
	for ( int i = Left; i < Bottom +1; i++ )
	{
		mList = s->value( QString( "TabToolBar/Tabs/%1" ).arg( i ) ).toList();
		foreach ( QVariant v, mList )
			if ( bar( (pTabToolBarManager::Align)i )->tab( v.toInt() ) )
				bar( (pTabToolBarManager::Align)i )->setTab( v.toInt(), true );
	}
}
//
void pTabToolBarManager::saveState( QSettings* s )
{
	if ( !s )
		return;
	QVariantList mList;
	for ( int i = Left; i < Bottom +1; i++ )
	{
		mList.clear();
		for ( int j = 0; j < bar( (pTabToolBarManager::Align)i )->tabs()->count(); j++ )
			if ( bar( (pTabToolBarManager::Align)i )->isTabRaised( j ) )
				mList << j;
		s->setValue( QString( "TabToolBar/Tabs/%1" ).arg( i ), mList );
	}
}
//
void pTabToolBarManager::dockWidgetTabToolBarChanged( QDockWidget* d, pTabToolBar* f )
{
	KMultiTabBarTab* tf = f->tabOf( d );
	if ( !tf )
		return;
	QPixmap p = tf->icon().pixmap( QSize( 24, 24 ), QIcon::Normal, QIcon::On );
	QString s = tf->text();
	f->removeTab( d );
	bar( dockWidgetAreaToAlign( mMain->dockWidgetArea( d ) ) )->appendTab( d, p, s );
}
