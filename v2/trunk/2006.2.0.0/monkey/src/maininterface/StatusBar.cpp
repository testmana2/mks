#include "StatusBar.h"
//
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QFontMetrics>
//
QPointer<StatusBar> StatusBar::mSelf = 0L;
//
StatusBar* StatusBar::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new StatusBar( p );
	return mSelf;
}
//
StatusBar::StatusBar( QWidget* p )
	: QStatusBar( p )
{
	setSizeGripEnabled( true );
	for ( int i = tStatusTip; i < tFilePath +1; i++ )
	{
		mLabels[ i ] = new QLabel( this );
		mLabels[ i ]->setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );
		addPermanentWidget( mLabels[ i ], i != tFilePath ? 0 : 100 );
	}
	connect( this, SIGNAL( messageChanged( const QString& ) ), label( tStatusTip ), SLOT( setText( const QString& ) ) );
	initialize();
}
//
void StatusBar::initialize()
{
	setCursorPosition( QPoint( -1, -1 ) );
	setModified( false );
	setMode( AbstractChild::mNone );
	setFileName( QString::null );
}
//
QLabel* StatusBar::label( Type t )
{
	return mLabels[ t ];
}
//
void StatusBar::setText( Type t, const QString& s, int i )
{
	label( t )->setText( s );
	if ( i > 0 )
		QTimer::singleShot( i, label( t ), SLOT( clear() ) );
}
//
void StatusBar::setPixmap( Type t, const QPixmap& p, int i )
{
	label( t )->setPixmap( p );
	if ( i > 0 )
		QTimer::singleShot( i, label( t ), SLOT( clear() ) );
}
//
void StatusBar::setCursorPosition( const QPoint& p )
{
	if ( p == QPoint( -1, -1 ) )
		setText( tCursor, tr( "Line : - Col : -" ) );
	else
		setText( tCursor, tr( "Line : %1 Col : %2" ).arg( p.y() ).arg( p.x() ) );
}
//
void StatusBar::setModified( bool b )
{
	setPixmap( tState, QIcon( ":/Icons/Icons/filesave.png" ).pixmap( 16, (QIcon::Mode)!b ) );
}
//
void StatusBar::setMode( AbstractChild::Mode m )
{
	switch ( m )
	{
	case AbstractChild::mInsert:
		setText( tMode, tr( "INSERT" ) );
		break;
	case AbstractChild::mOverwrite:
		setText( tMode,  tr( "OVERWRITE" ) );
		break;
	default:
		setText( tMode,  tr( "N/A" ) );
		break;
	}
}
//
void StatusBar::setFileName( const QString& s )
{
	setText( tFilePath, s.isEmpty() ? tr( "No File" ) : s );
}
//
