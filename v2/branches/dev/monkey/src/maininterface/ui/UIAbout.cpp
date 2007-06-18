#include "UIAbout.h"

#include <QFile>
#include <QDesktopServices>
#include <QTabBar>

QString mInformations =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
"	<tr>"
"		<td align=\"center\"><br /><img src=\":/application/icons/application/monkey2.png\" width=\"32\" height=\"32\"></td>"
"		<td>%2 v%3<br />%4<br />The Monkey Studio Team<br /><a href=\"%6\">Home page</a></td>"
"	</tr>"
"</table>";

QList<pAuthorInformations> mAuthors = QList<pAuthorInformations>()
	<< pAuthorInformations( "Filipe Azevedo", "Nox P@sNox", QObject::tr( "France" ), "pasnox@gmail.com", QObject::tr( "Creator && Principal Developer" ) )
	<< pAuthorInformations( "Manuel Schmidt", "oversize", QObject::tr( "Germany" ), "manuel@schmidtman.de", QObject::tr( "Web Developer & Web Designer" ) )
	<< pAuthorInformations( "Yannick", "xiantia", QObject::tr( "France" ), "xiantia@gmail.com", QObject::tr( "GDB Driver Integration" ) )
	<< pAuthorInformations( "Julien Decologne", "Judd", QObject::tr( "France" ), "judd@hotmail.com", QObject::tr( "Splashscreen & Icons Designer" ) );

QString mAuthorMask =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
"	<tr>"
"		<td>"
"			<b>%1</b><br />"
"			%2 aka %3<br />"
"			%4 (<a href=\"%5\">%5</a>)<br />"
"		</td>"
"	</tr>"
"</table>";

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

	// show informations table
	lInformations->setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard );
	lInformations->setText( mInformations.arg( PROGRAM_NAME ).arg( PROGRAM_VERSION ).arg( PROGRAM_COPYRIGHTS ).arg( PROGRAM_DOMAIN ) );

	foreach ( pAuthorInformations i, mAuthors )
		tbTeam->append( mAuthorMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( i.Email ) );

	QFile file;

	// about team license donations bugs

	// bugs / wishes
	file.setFileName( ":/About/About/greetings.html" );
	file.open( QFile::ReadOnly );
	tbBugs->setHtml( file.readAll() );
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

	// connections
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}

void UIAbout::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
