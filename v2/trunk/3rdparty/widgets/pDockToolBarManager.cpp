#include "pDockToolBarManager.h"
#include "pDockToolBar.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QSettings>
#include <QAbstractButton>

pDockToolBarManager::pDockToolBarManager( QMainWindow* w )
	: QObject( w ), mMain( w )
{
	Q_ASSERT( w != 0 );
}

QMainWindow* pDockToolBarManager::mainWindow() const
{
	return mMain;
}

pDockToolBar* pDockToolBarManager::bar( Qt::ToolBarArea a )
{
	// if toolbar not exists, create it
	if ( !mBars.contains( a ) )
	{
		switch ( a )
		{
		case Qt::TopToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Horizontal );
			mBars[a]->setObjectName( "pDockToolBarTop" );
			mBars[a]->setWindowTitle( tr( "Top DockToolBar" ) );
			break;
		case Qt::BottomToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Horizontal );
			mBars[a]->setObjectName( "pDockToolBarBottom" );
			mBars[a]->setWindowTitle( tr( "Bottom DockToolBar" ) );
			break;
		case Qt::LeftToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Vertical );
			mBars[a]->setObjectName( "pDockToolBarLeft" );
			mBars[a]->setWindowTitle( tr( "Left DockToolBar" ) );
			break;
		case Qt::RightToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Vertical );
			mBars[a]->setObjectName( "pDockToolBarRight" );
			mBars[a]->setWindowTitle( tr( "Right DockToolBar" ) );
			break;
		default:
			return 0;
			break;
		}

		// add toolbar to mainwindow
		mMain->addToolBar( a, mBars[a] );

		// toolbar is not movable
		mBars[a]->setMovable( false );

		// hide
		mBars[a]->hide();

		// track dock bar changed
		connect( mBars[a], SIGNAL( dockWidgetAreaChanged( QDockWidget*, pDockToolBar* ) ), this, SLOT( dockWidgetAreaChanged( QDockWidget*, pDockToolBar* ) ) );
	}

	// return existings toolbar
	return mBars[a];
}

Qt::ToolBarArea pDockToolBarManager::dockWidgetAreaToToolBarArea( Qt::DockWidgetArea a )
{
	// convert dock area to toolbar area
	switch ( a )
	{
	case Qt::LeftDockWidgetArea:
		return Qt::LeftToolBarArea;
		break;
	case Qt::RightDockWidgetArea:
		return Qt::RightToolBarArea;
		break;
	case Qt::TopDockWidgetArea:
		return Qt::TopToolBarArea;
		break;
	case Qt::BottomDockWidgetArea:
		return Qt::BottomToolBarArea;
		break;
	default:
		return Qt::BottomToolBarArea;
		break;
	}
}

Qt::DockWidgetArea pDockToolBarManager::ToolBarAreaToDockWidgetArea( Qt::ToolBarArea a )
{
	// convert toolbar area to dock widget area
	switch ( a )
	{
	case Qt::LeftToolBarArea:
		return Qt::LeftDockWidgetArea;
		break;
	case Qt::RightToolBarArea:
		return Qt::RightDockWidgetArea;
		break;
	case Qt::TopToolBarArea:
		return Qt::TopDockWidgetArea;
		break;
	case Qt::BottomToolBarArea:
		return Qt::BottomDockWidgetArea;
		break;
	default:
		return Qt::BottomDockWidgetArea;
		break;
	}
}

void pDockToolBarManager::restoreState( QSettings* s )
{
	// if no settings cancel
	if ( !s )
		return;

	QVariantList mList;

	// for each docktoolbar
	for ( int i = Qt::LeftToolBarArea; i < Qt::AllToolBarAreas; i++ )
	{
		// get datas
		mList = s->value( QString( "DockToolBar/Tabs/%1" ).arg( i ) ).toList();

		// for each datas
		foreach ( QVariant v, mList )
		{
			foreach ( QDockWidget* d, bar( (Qt::ToolBarArea)i )->docks() )
			{
				if ( d->objectName() == v.toString().section( '|', 0, 0 ) )
					bar( (Qt::ToolBarArea)i )->button( d )->setChecked( v.toString().section( '|', 1, 1 ).toInt() );
			}
		}
	}
}

void pDockToolBarManager::saveState( QSettings* s )
{
	// cancel if no settings
	if ( !s )
		return;

	// list to stock checked button
	QVariantList mList;

	// for each docktoolbar
	for ( int i = Qt::LeftToolBarArea; i < Qt::AllToolBarAreas; i++ )
	{
		// clear variant list
		mList.clear();

		// for each button in docktoolbar
		foreach ( QAbstractButton* b, bar( (Qt::ToolBarArea)i )->buttons() )
			mList << QString( "%1|%2" ).arg( bar( (Qt::ToolBarArea)i )->dock( b )->objectName(), b->isChecked() );

		// write datas
		s->setValue( QString( "DockToolBar/Tabs/%1" ).arg( i ), mList );
	}
}

void pDockToolBarManager::dockWidgetAreaChanged( QDockWidget* d, pDockToolBar* f )
{
	// remove dock from old toolbar
	f->removeDock( d );

	// add dock to new toolbar
	bar( dockWidgetAreaToToolBarArea( mMain->dockWidgetArea( d ) ) )->addDock( d, d->windowTitle(), d->windowIcon().pixmap( QSize( 24, 24 ), QIcon::Normal, QIcon::On ) );
}
