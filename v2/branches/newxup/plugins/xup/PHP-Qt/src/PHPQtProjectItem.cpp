#include "PHPQtProjectItem.h"

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
		cmd.setText( tr( "Start" ) );
		cmd.setArguments( mainFile );
		cmd.setParsers( QStringList() );
		cmd.setTryAllParsers( false );
		addCommand( cmd, "mInterpreter" );
		
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

bool PHPQtProjectItem::edit()
{
	return UIXUPEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

CLIToolPlugin* PHPQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "PHP" );
}
