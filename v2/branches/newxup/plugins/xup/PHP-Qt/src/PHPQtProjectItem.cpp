#include "PHPQtProjectItem.h"
#include "src/gui/UIPHPEditor.h"

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
	QString mainFile = relativeFilePath( attribute( "MAIN_FILE" ) );
	
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
	return UIPHPEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

DocumentFilterMap PHPQtProjectItem::sourceFileNamePatterns() const
{
	DocumentFilterMap filters;
	int weight = 0;
	
	filters[ "FORMS" ].weight = weight++;
	filters[ "FORMS" ].label = tr( "Qt Forms" );
	filters[ "FORMS" ].icon = "forms.png";
	filters[ "FORMS" ].filters << "*.ui";
	
	filters[ "PHP_FILES" ].weight = weight++;
	filters[ "PHP_FILES" ].label = tr( "PHP Files" );
	filters[ "PHP_FILES" ].icon = "php.png";
	filters[ "PHP_FILES" ].filters << "*.php*";
	
	return filters;
}

CLIToolPlugin* PHPQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "PHP" );
}
