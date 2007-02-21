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
	//
	setWindowTitle( windowTitle().arg( PROGRAM_NAME ) );
	twAbout->findChild<QTabBar*>()->setCursor( Qt::PointingHandCursor );
	lInformations->setText( QString( "%1 v%2\n%3" ).arg( PROGRAM_NAME, PROGRAM_VERSION, COPYRIGHTS ) );
	QFile file;
	// bugs / wishes
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbBugs->setHtml( file.readAll() );
	file.close();
	// team
	file.setFileName( ":/About/About/authors.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbTeam->setHtml( file.readAll() );
	file.close();
	// donations
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbDonations->setHtml( file.readAll() );
	file.close();
	// license
	file.setFileName( ":/Licenses/Licenses/license.gpl" );
	file.open( QFile::ReadOnly | QFile::Text );
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
	if ( b )
		b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
