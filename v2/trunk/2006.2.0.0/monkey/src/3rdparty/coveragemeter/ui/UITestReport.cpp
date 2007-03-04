#include "UITestReport.h"
#include "PluginsManager.h"
#include "main.h"
//
#include <QFile>
#include <QDesktopServices>
#include <stdlib.h>
#include <QDateTime>
//
QPointer<UITestReport> UITestReport::mSelf = 0L;
//
UITestReport* UITestReport::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UITestReport( p );
	return mSelf;
}
//
UITestReport::UITestReport( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	QString descriptionText;
	descriptionText = "<HTML><BODY>" 
                      "<H1>Test Report Generation for MonkeyStudio</H1>"
                      "<P>"
                      "Process as follows:"
                      "<UL>"
                        "<LI>Do some test operations.</LI>"
                        "<LI>Press the <TT>PAUSE</TT> key or exit MonkeyStudio. This dialog appears.</LI>"
                        "<LI>Enter a title for your test and its state (Passed or Failed).</LI>"
                        "<LI>Press OK to save the report.</LI>"
                      "</UL>"
                      "</P><P>"
                      "The report contains code coverage data generated using "
                      "<I>CoverageMeter</I>&nbsp;(<TT>http://www.coveragemeter.com</TT>). "
                      "This report does not contain any kind of information about "
                      "the project developped using MonkeyStudio."
                      "</P><P>"
                      "When the testing phase is finished, send the file "
                      "'<TT>"+QCoreApplication::applicationFilePath ()+".csexe</TT>' "
                      "to xxxx@xxx.xx" 
                      "</P>"
                      "</BODY></HTML>";
	description->setText(descriptionText);
	//
	connect( ok, SIGNAL( clicked( ) ), this, SLOT( okClicked( ) ) );
	connect( cancel, SIGNAL( clicked( ) ), this, SLOT( cancelClicked( ) ) );
}
//
void UITestReport::okClicked( )
{
#ifdef __COVERAGESCANNER__
	setTestTitle( title->text() );
	switch ( status->currentIndex() )
	{
	case 1:
		__coveragescanner_teststate( "PASSED" );
		break;
        case 2:
		__coveragescanner_teststate( "FAILED" );
		break;
        default:
		__coveragescanner_teststate( "UNKNOWN" );
		break;
	}
	__coveragescanner_save();
	setTestTitle( QString() );
#endif
	accept();
}

void UITestReport::cancelClicked( )
{
	reject();
}

void UITestReport::setTestTitle( const QString& s )
{
	QString userName = QString( getenv( "USERNAME" ) );
	QString user = QString( getenv( "USER" ) );
	QString testName;
	testName = user;
	if ( !userName.isEmpty() )
		testName += "(" +userName +")";
	//
	testName += "/";
	if ( s.isEmpty() )
		testName += QString( "unnamed" );
	else
		testName += s;
#ifdef __COVERAGESCANNER__
    QString monkeyTestName=testName+"/monkey";
	__coveragescanner_testname( monkeyTestName.toAscii() );
#endif
	for ( int i = 0; i < PluginsManager::self()->plugins().count(); i++ )
	{
      BasePlugin *plg=PluginsManager::self()->plugins().at(i);
      plg->saveCodeCoverage(testName);
    }
}
