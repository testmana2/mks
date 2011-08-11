#include "PHPQtProjectItem.h"
#include "XUPProjectItemHelper.h"

#include <UIXUPEditor.h>
#include <CLIToolPlugin.h>
#include <UIMain.h>
#include <PluginsManager.h>

#include <QDir>
#include <QDebug>

PHPQtProjectItem::PHPQtProjectItem()
	: XUPProjectItem()
{
}

QString PHPQtProjectItem::projectType() const
{
	return PLUGIN_NAME;
}

void PHPQtProjectItem::installCommands()
{
	// get plugins
	CLIToolPlugin* ip = interpreter();
	
	// temp command
	pCommand cmd;

	// build command
	if ( ip ) {
		cmd = ip->command();
	}
	
	//cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( XUPProjectItemHelper::projectSettingsValue( this, "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() ) {
		mainFile = relativeFilePath( findFile( "main.php" ).value( 0 ).absoluteFilePath() );
	}
	
	// available commands
	if ( ip ) {
		// execute project
		cmd = cmdInterpret;
		cmd.setText( tr( "Run" ) );
		cmd.setArguments( mainFile );
		cmd.setParsers( QStringList() );
		cmd.setTryAllParsers( false );
		addCommand( cmd, "mInterpreter" );
	}
	
	// phpqtuic4 on current file
	cmd = cmdInterpret;
	cmd.setText( tr( "Generate current form implementation" ) );
	cmd.setCommand( "phpqtuic4" );
	cmd.setArguments( "\"$cf$\" -o \"$cf$.php\"" );
	cmd.setWorkingDirectory( "$cfp$" );
	cmd.setParsers( QStringList() );
	cmd.setSkipOnError( false );
	cmd.setTryAllParsers( false );
	addCommand( cmd, "mInterpreter" );
	
	// pyrcc4 on current file
	cmd = cmdInterpret;
	cmd.setText( tr( "Generate current resource implementation" ) );
	cmd.setCommand( "phpqtrcc4" );
	cmd.setArguments( "\"$cf$\" -o \"$cf$.php\"" );
	cmd.setWorkingDirectory( "$cfp$" );
	cmd.setParsers( QStringList() );
	cmd.setSkipOnError( false );
	cmd.setTryAllParsers( false );
	addCommand( cmd, "mInterpreter" );
	
	// pylupdate4 on current file
	cmd = cmdInterpret;
	cmd.setText( tr( "Generate current project translation files" ) );
	cmd.setCommand( "phpqtlupdate4" );
	cmd.setArguments( "\"$cp$\"" );
	cmd.setWorkingDirectory( "$cpp$" );
	cmd.setParsers( QStringList() );
	cmd.setSkipOnError( false );
	cmd.setTryAllParsers( false );
	addCommand( cmd, "mInterpreter" );
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

bool PHPQtProjectItem::edit()
{
	UIXUPEditor dlg( MonkeyCore::mainWindow() );
	dlg.setupProject( this );
	return dlg.exec() == QDialog::Accepted;
}

bool PHPQtProjectItem::editProjectFiles()
{
	UIXUPEditor dlg( MonkeyCore::mainWindow() );
	dlg.setupProject( this );
	dlg.showProjectFilesPage();
	return dlg.exec() == QDialog::Accepted;
}

CLIToolPlugin* PHPQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "PHP" );
}
