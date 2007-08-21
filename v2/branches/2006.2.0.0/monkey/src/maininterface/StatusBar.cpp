#include "StatusBar.h"
//
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QFontMetrics>
#include <QUrl>
#include <QSizeGrip>
#include <QLayout>
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
		addPermanentWidget( mLabels[ i ], i != tStatusTip ? 0 : 100 );
	}
	connect( this, SIGNAL( messageChanged( const QString& ) ), this, SLOT( setText( const QString& ) ) );
	initialize();
}
//
void StatusBar::resizeEvent( QResizeEvent* )
{
	updateLabelsSize();
}
//
void StatusBar::initialize()
{
	setCursorPosition( QPoint( -1, -1 ) );
	setModified( false );
	setDocumentMode( AbstractChild::mNone );
	setLayoutMode( AbstractChild::lNone );
	setFileName( QString::null );
}
//
void StatusBar::updateLabelsSize()
{
	// calcul max width for label
	int w = width();
	// remove layout margin and spacing
	w -= ( layout()->margin() *2 +layout()->spacing() *6 );
	// remove all label width
	w -= ( label( tCursor )->width() +label( tState )->width() +label( tDocumentMode )->width() +label( tLayoutMode )->width() +label( tFilePath )->width() );
	// remove sizegrip width
	if ( isSizeGripEnabled() )
		w -= findChild<QSizeGrip*>()->width();
	// apply width
	label( tStatusTip )->setMaximumWidth( w );
}
//
QLabel* StatusBar::label( Type t )
{
	return mLabels[ t ];
}
//
void StatusBar::setText( Type t, const QString& s, int i )
{
	// update labels size
	updateLabelsSize();
	// set text
	label( t )->setText( s );
	// clear message if needed
	if ( i > 0 )
		QTimer::singleShot( i, label( t ), SLOT( clear() ) );
}
//
void StatusBar::setText( const QString& s )
{
	setText( tStatusTip, s );
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
void StatusBar::setDocumentMode( AbstractChild::DocumentMode m )
{
	switch ( m )
	{
	case AbstractChild::mInsert:
		setText( tDocumentMode, tr( "INSERT" ) );
		break;
	case AbstractChild::mOverwrite:
		setText( tDocumentMode,  tr( "OVERWRITE" ) );
		break;
	default:
		setText( tDocumentMode,  tr( "N/A" ) );
		break;
	}
}
//
void StatusBar::setLayoutMode( AbstractChild::LayoutMode m )
{
	switch ( m )
	{
	case AbstractChild::lNormal:
		setText( tLayoutMode, tr( "NORMAL" ) );
		break;
	case AbstractChild::lVertical:
		setText( tLayoutMode,  tr( "VERTICAL" ) );
		break;
	case AbstractChild::lHorizontal:
		setText( tLayoutMode,  tr( "HORIZONTAL" ) );
		break;
	default:
		setText( tLayoutMode,  tr( "N/A" ) );
		break;
	}
}
//
void StatusBar::setFileName( const QString& s )
{
	QFileInfo f( QUrl( s ).toString( QUrl::RemoveScheme ) );
#ifdef Q_WS_WIN
	f.setFile( f.filePath().replace( "///", QString::null ) );
#endif
	setText( tFilePath, f.fileName().length() ? f.fileName() : tr( "No File" ) );
	label( tFilePath )->setToolTip( f.fileName().length() ? s : tr( "No File" ) );
}
