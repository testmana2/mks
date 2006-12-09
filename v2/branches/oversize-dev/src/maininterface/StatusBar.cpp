#include "StatusBar.h"
//
#include <QLabel>
#include <QTimer>
#include <QIcon>
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
	for ( int i = ltStatusTip; i < ltFilePath +1; i++ )
	{
		mLabels[ i ] = new QLabel( this );
		mLabels[ i ]->setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );
		addPermanentWidget( mLabels[ i ], i != ltFilePath ? 0 : 100 );
	}
	connect( this, SIGNAL( messageChanged( const QString& ) ), label( ltStatusTip ), SLOT( setText( const QString& ) ) );
}
//
void StatusBar::initialize()
{
	setCursorPosition( QPoint( -1, -1 ) );
	setModified( false );
	setMode( lmNone );
	setFileName( QString::null );
}
//
QLabel* StatusBar::label( LabelType t )
{
	return mLabels[ t ];
}
//
void StatusBar::setText( LabelType t, const QString& s, int i )
{
	label( t )->setText( s );
	if ( i > 0 )
		QTimer::singleShot( i, label( t ), SLOT( clear() ) );
}
//
void StatusBar::setCursorPosition( const QPoint& p )
{
	if ( p == QPoint( -1, -1 ) )
		label( ltCursor )->setText( tr( "Line : - Col : -" ) );
	else
		label( ltCursor )->setText( tr( "Line : %1 Col : %2" ).arg( p.y() ).arg( p.x() ) );
}
//
void StatusBar::setModified( bool b )
{
	label( ltState )->setPixmap( QIcon( ":/Icons/Icons/filesave.png" ).pixmap( 16, (QIcon::Mode)!b ) );
}
//
void StatusBar::setMode( LabelMode l )
{
	switch ( l )
	{
	case lmNone:
		label( ltMode )->setText( tr( "N/A" ) );
		break;
	case lmInsert:
		label( ltMode )->setText( tr( "INSERT" ) );
		break;
	case lmOverwrite:
		label( ltMode )->setText( tr( "OVERWRITE" ) );
		break;
	}
}
//
void StatusBar::setFileName( const QString& s )
{
	label( ltFilePath )->setText( s.isEmpty() ? tr( "No File" ) : s );
}
//
