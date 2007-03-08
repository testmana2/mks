#include "Console.h"
#include "ConsoleCommandParser.h"
#include "UISettingsConsole.h"
#include "Settings.h"
//
#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>
//
Console::~Console()
{
	if ( isInstalled() )
		uninstall();
}
//
void Console::initialize( Workspace* w )
{
	BasePlugin::initialize( w );
	mProcess = 0L;
	mParser = 0L;
	mStop = false;
	mCodec = 0;
	// plugin infos
	mPluginInfos.Caption = tr( "Console Bridge" );
	mPluginInfos.Description = tr( "This plugin provide a qprocess console bridge" );
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name ="Console";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Installed = false;
}
//
bool Console::install()
{
	// create process
	mProcess = new QProcess( this );
	// mixes channels
	mProcess->setReadChannelMode( QProcess::MergedChannels );
	// remove lang to ouput in english only
	QStringList l = mProcess->systemEnvironment();
	l.removeAt( l.indexOf( QRegExp( "^LANG.*" ) ) );
	mProcess->setEnvironment( l );
	// set codec
	updateCodec();
	// connections
	connect( mProcess, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
	connect( mProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( finished( int, QProcess::ExitStatus ) ) );
	connect( mProcess, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect( mProcess, SIGNAL( started() ), this, SLOT( started() ) );
	connect( mProcess, SIGNAL( stateChanged( QProcess::ProcessState ) ), this, SLOT( stateChanged( QProcess::ProcessState ) ) );
	// plugin is installed
	mPluginInfos.Installed = true;
	return true;
}
//
bool Console::uninstall()
{
	// disconnections
	disconnect( mProcess, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
	disconnect( mProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( finished( int, QProcess::ExitStatus ) ) );
	disconnect( mProcess, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	disconnect( mProcess, SIGNAL( started() ), this, SLOT( started() ) );
	disconnect( mProcess, SIGNAL( stateChanged( QProcess::ProcessState ) ), this, SLOT( stateChanged( QProcess::ProcessState ) ) );
	// delete process
	delete mProcess;
	mProcess = 0L;
	// plugin is not installed
	mPluginInfos.Installed = false;
	return true;
}
//
QWidget* Console::settingsWidget()
{
	UISettingsConsole*  w = new UISettingsConsole;
	connect(  w, SIGNAL ( updateCodec() ), this, SLOT ( updateCodec() ) );
	return w;
}
//
void Console::setEnvironment( const QStringList& l )
{
	if ( isInstalled() )
		mProcess->setEnvironment( l );
}
//
void Console::getEnvironment( QStringList& l )
{
	if ( &l )
		l = ( isInstalled() ? mProcess->environment() : QProcess::systemEnvironment() );
}
//
void Console::addCommands( const ConsoleCommands& c )
{
	if ( isInstalled() )
		mConsoleCommandsList << c;
}
//
void Console::removeCommands( const ConsoleCommands& c )
{
	if ( isInstalled() )
		mConsoleCommandsList.removeAll( c );
}
//
void Console::getCommandsList( ConsoleCommandsList& l )
{
	if ( &l )
		l = ( isInstalled() ? mConsoleCommandsList : ConsoleCommandsList() );
}
//
void Console::isRunning( bool& b )
{
	if ( &b )
		b = ( isInstalled() ? !mProcess->state() == QProcess::NotRunning : false );
}
//
void Console::run()
{
	bool b;
	isRunning( b );
	if ( !isInstalled() || mConsoleCommandsList.isEmpty() || b )
		return;
	// got the current command list
	ConsoleCommands& cc = mConsoleCommandsList.first();
	if ( cc.isEmpty() )
	{
		// remove the entry
		mConsoleCommandsList.removeFirst();
		// call next if not empty
		if ( !mConsoleCommandsList.isEmpty() )
			run();
	}
	else
	{
		// got command
		ConsoleCommand& c = cc.first();
		QString command = c.command();
		QString workingdir = c.workingFolder();
		// parser
		mParser = c.parser();
		if ( mParser )
			connect( mParser, SIGNAL( newErrorAvailable( const ConsoleCommandParser::Message& ) ), this, SIGNAL( newErrorAvailable( const ConsoleCommandParser::Message& ) ) );
		// remove command from list
		cc.removeFirst();
		// remove the entry if list is empty
		if ( cc.isEmpty() )
			mConsoleCommandsList.removeFirst();
		// execute command or run next
		if ( command.isEmpty() )
			run();
		else
		{
			emit messageBox( "<font color=\"green\"><b>************************************************************</b></font>" );
			// assign and run process
			if ( !workingdir.isEmpty() )
			{
				mProcess->setWorkingDirectory( workingdir );
				emit messageBox( tr( "<font color=\"green\">From: <font color=\"red\"><b>%1</b></font></font>" ).arg( workingdir ) );
			}
			emit messageBox( tr( "<font color=\"green\">Executing: <font color=\"red\"><b>%1</b></font></font>" ).arg( command ) );
			emit messageBox( "<font color=\"green\"><b>************************************************************</b></font>" );
			mProcess->start( command );
		}
	}
}
//
void Console::runConsoleCommands( const ConsoleCommands& c )
{
	addCommands( c );
	run();
}
//
void Console::stopConsole()
{
	bool b;
	isRunning( b );
	if ( isInstalled() && b )
	{
		mStop = true;
		mProcess->terminate();
		mProcess->waitForFinished( 5000 );
		mProcess->kill();
		emit messageBox( tr( "<font color=\"red\"><b>*** Console stopped by user ***</b></font>" ) );
	}
}
//
void Console::error( QProcess::ProcessError e )
{
	QString s;
	switch ( e )
	{
	case QProcess::FailedToStart:
		s = tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." );
		break;
	case QProcess::Crashed:
		s = tr( "The process crashed some time after starting successfully." );
		break;
	case QProcess::Timedout:
		s = tr( "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." );
		break;
	case QProcess::WriteError:
		s = tr( "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." );
		break;
	case QProcess::ReadError:
		s = tr( "An error occurred when attempting to read from the process. For example, the process may not be running." );
		break;
	case QProcess::UnknownError:
	default:
		s = tr( "An unknown error occurred. This is the default return value of error()." );
		break;
	}
	emit messageBox( s );
}
//
void Console::finished( int i, QProcess::ExitStatus e )
{
	QString s;
	switch ( e )
	{
	case QProcess::NormalExit:
		s = tr( "The process exited normally." );
		break;
	case QProcess::CrashExit:
		s = tr( "The process crashed." );
		break;
	default:
		s = tr( "An unknown error occurred." );
	}
	// parser
	if ( mParser )
		emit messageBox( mParser->resultString() );
	delete mParser;
	// send command finish
	emit messageBox( s.prepend( "<b><font color=\"blue\">*** " ).append( tr( " Exit Code: %1" ).arg( i ) ).append( " ***</font></b>" ) );
	// execute next command
	if ( mStop && !mConsoleCommandsList.isEmpty() )
	{
		if ( QMessageBox::question( qApp->activeWindow(), tr( "Stop console..." ), tr( "There are pending commands, do you want to cancel them ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
		{
			mStop = false;
			mConsoleCommandsList.clear();
			return;
		}
	}
	// continue...
	mStop = false;	
	run();
}
//
void Console::readyRead()
{
	if ( isInstalled() )
	{
		
		QString s = mCodec->toUnicode( mProcess->readAllStandardOutput().data() );
		if ( mParser )
			mParser->appendToBuffer( s );
		emit dataAvailable( s );
	}
}
//
void Console::started()
{
	//emit messageBox( tr( "Process Started" ) );
}
//
void Console::stateChanged( QProcess::ProcessState e )
{
	QString s;
	switch ( e )
	{
	case QProcess::NotRunning:
		s = tr( "The process is not running." );
		break;
	case QProcess::Starting:
		s = tr( "The process is starting, but the program has not yet been invoked." );
		break;
	case QProcess::Running:
		s = tr( "The process is running and is ready for reading and writing." );
		break;
	}
	//emit messageBox( s );
}
//
void Console::updateCodec()
{
	mCodec = QTextCodec::codecForName ( Settings::current()->value( "Plugins/Console/Charset", "System" ).toString().toLatin1() );
}
//
void Console::saveCodeCoverage(const QString &name)
{
  QString testname=name+"/"+infos().Name;
#ifdef __COVERAGESCANNER__
  __coveragescanner_filename(codeCoverageFile().toAscii());
  __coveragescanner_testname(testname.toAscii());
  __coveragescanner_save();
#endif
}
//
Q_EXPORT_PLUGIN2( BaseConsole, Console )
