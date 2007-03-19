#include "UIAbout.h"
#include "main.h"
//
#include <QFile>
#include <QDesktopServices>
#include <QTabBar>
//
QPointer<UIAbout> UIAbout::mSelf = 0L;
//
UIAbout* UIAbout::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIAbout( p );
	return mSelf;
}
//
UIAbout::UIAbout( QWidget* p )
	: QDialog( p )
{
	setupUi( this );	
	setAttribute( Qt::WA_DeleteOnClose );
	// window title
	setWindowTitle( windowTitle().arg( PROGRAM_NAME ) );
	// mouse cursor
	twAbout->findChild<QTabBar*>()->setCursor( Qt::PointingHandCursor );	
	// change label color
	QPalette lp( lInformations->palette() );
	lp.setColor( lInformations->backgroundRole(), Qt::white );
	lInformations->setPalette( lp );
	//
	QFile file;
	// show informations table
	file.setFileName( ":/About/About/informations.html" );
	file.open( QFile::ReadOnly );
	lInformations->setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard );
	lInformations->setText( QString::fromLocal8Bit( file.readAll() ).arg( ":/Icons/Icons/icon.png" ).arg( PROGRAM_NAME ).arg( PROGRAM_VERSION ).arg( COPYRIGHTS ).arg( COPYRIGHTS_OTHERS ).arg( HOMEPAGE ) );
	file.close();
	// bugs / wishes
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly );
	tbBugs->setHtml( file.readAll() );
	file.close();
	// team
	file.setFileName( ":/About/About/authors.html" );
	file.open( QFile::ReadOnly );
	tbTeam->setHtml( file.readAll() );
	file.close();
	// donations
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly );
	tbDonations->setHtml( file.readAll() );
	file.close();
	// license
	file.setFileName( ":/Licenses/Licenses/license.gpl" );
	file.open( QFile::ReadOnly );
	tbLicense->setPlainText( file.readAll() );
	file.close();
	//
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}
//
void UIAbout::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
