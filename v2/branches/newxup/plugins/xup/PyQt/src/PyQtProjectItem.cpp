#include "PyQtProjectItem.h"

#include <UIXUPEditor.h>
#include <CLIToolPlugin.h>
#include <UIMain.h>
#include <PluginsManager.h>

#include <QDir>
#include <QDebug>

PyQtProjectItem::PyQtProjectItem()
	: XUPProjectItem()
{
}

QString PyQtProjectItem::projectType() const
{
	return PLUGIN_NAME;
}

void PyQtProjectItem::installCommands()
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
	QString mainFile = relativeFilePath( attribute( "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() ) {
		mainFile = relativeFilePath( findFile( "main.py" ).value( 0 ).absoluteFilePath() );
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

bool PyQtProjectItem::edit()
{
	return UIXUPEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

CLIToolPlugin* PyQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "Python" );
}
