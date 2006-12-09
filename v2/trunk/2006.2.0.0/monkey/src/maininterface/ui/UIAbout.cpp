#include "UIAbout.h"
#include "main.h"
//
#include <QFile>
#include <QDesktopServices>
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
	lInformations->setText( QString( "%1 v%2\n%3" ).arg( PROGRAM_NAME, PROGRAM_VERSION, COPYRIGHTS ) );
	QFile file;
	file.setFileName( ":/About/About/authors.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbAuthors->setHtml( file.readAll() );
	file.close();
	file.setFileName( ":/About/About/testers.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbTesters->setHtml( file.readAll() );
	file.close();
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbGreetings->setHtml( file.readAll() );
	file.close();
	file.setFileName( ":/Licenses/Licenses/license.gpl" );
	file.open( QFile::ReadOnly | QFile::Text );
	tbLicence->setPlainText( file.readAll() );
	file.close();
	//
	connect( tbAuthors, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
	connect( tbTesters, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
	connect( tbGreetings, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}
//
void UIAbout::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( b )
		b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
