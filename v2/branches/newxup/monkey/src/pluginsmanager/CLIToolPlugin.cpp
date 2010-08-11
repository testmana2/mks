/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio IDE
** FileName  : CLIToolPlugin.cpp
** Date      : 2009-12-09T00:37:00
** License   : GPL
** Comment   : 
** Home Page : http://www.monkeystudio.org
**
**
****************************************************************************/
#include "CLIToolPlugin.h"
#include "ui/UIAdditionalCommandsSettings.h"
#include "ui/UICLIToolSettings.h"

CLIToolPlugin::CLIToolPlugin()
	: BasePlugin()
{
}

pCommand CLIToolPlugin::command() const
{
	// get settings object
	pSettings* settings = MonkeyCore::settings();
	pCommand cmd;
	
	cmd.setText( settings->value( settingsKey( "command/Text" ) ).toString() );
	cmd.setCommand( settings->value( settingsKey( "command/Command" ) ).toString() );
	cmd.setArguments( settings->value( settingsKey( "command/Arguments" ) ).toString() );
	cmd.setWorkingDirectory( settings->value( settingsKey( "command/WorkingDirectory" ) ).toString() );
	cmd.setParsers( settings->value( settingsKey( "command/Parsers" ) ).toStringList() );
	cmd.setTryAllParsers( settings->value( settingsKey( "command/TryAll" ), false ).toBool() );
	cmd.setSkipOnError( settings->value( settingsKey( "command/SkipOnError" ), false ).toBool() );
	
	// if no user commands get global ones
	if ( !cmd.isValid() )
	{
		cmd = defaultCommand();
	}
	
	return cmd;
}

void CLIToolPlugin::setCommand( const pCommand& cmd )
{
	pSettings* settings = MonkeyCore::settings();
	
	settings->setValue( settingsKey( "command/Text" ), cmd.text() );
	settings->setValue( settingsKey( "command/Command" ), cmd.command() );
	settings->setValue( settingsKey( "command/Arguments" ), cmd.arguments() );
	settings->setValue( settingsKey( "command/WorkingDirectory" ), cmd.workingDirectory() );
	settings->setValue( settingsKey( "command/Parsers" ), cmd.parsers() );
	settings->setValue( settingsKey( "command/TryAll" ), cmd.tryAllParsers() );
	settings->setValue( settingsKey( "command/SkipOnError" ), cmd.skipOnError() );
}

QWidget* CLIToolPlugin::settingsWidget()
{
	return new UICLIToolSettings( this, QApplication::activeWindow() );
}


pCommandList CLIToolPlugin::defaultCommands() const
{
	return pCommandList();
}

pCommandList CLIToolPlugin::userCommands() const
{
	// commands list
	pCommandList cmds;
	// get settings object
	pSettings* settings = MonkeyCore::settings();
	
	// read user commands for this plugin
	int size = settings->beginReadArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < size; i++ )
	{
		settings->setArrayIndex( i );
		pCommand cmd;
		
		cmd.setText( settings->value( "Text" ).toString() );
		cmd.setCommand( settings->value( "Command" ).toString() );
		cmd.setArguments( settings->value( "Arguments" ).toString() );
		cmd.setWorkingDirectory( settings->value( "WorkingDirectory" ).toString() );
		cmd.setParsers( settings->value( "Parsers" ).toStringList() );
		cmd.setTryAllParsers( settings->value( "TryAll" ).toBool() );
		cmd.setSkipOnError( settings->value( "SkipOnError" ).toBool() );
		
		cmds << cmd;
	}
	settings->endArray();
	
	// if no user commands get global ones
	if ( cmds.isEmpty() )
	{
		cmds << defaultCommands();
	}
	
	// return list
	return cmds;
}

void CLIToolPlugin::setUserCommands( const pCommandList& cmds ) const
{
	// get settings object
	pSettings* settings = MonkeyCore::settings();
	
	// remove old key
	settings->remove( settingsKey( "Commands" ) );
	
	// write user commands for this plugin
	settings->beginWriteArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < cmds.count(); i++ )
	{
		settings->setArrayIndex( i );
		const pCommand& cmd = cmds[i];
		
		settings->setValue( "Text", cmd.text() );
		settings->setValue( "Command", cmd.command() );
		settings->setValue( "Arguments", cmd.arguments() );
		settings->setValue( "WorkingDirectory", cmd.workingDirectory() );
		settings->setValue( "Parsers", cmd.parsers() );
		settings->setValue( "TryAll", cmd.tryAllParsers() );
		settings->setValue( "SkipOnError", cmd.skipOnError() );
	}
	settings->endArray();
}

QWidget* CLIToolPlugin::additionalCommandsSettingsWidget()
{
	return new UIAdditionalCommandsSettings( this, defaultCommands(), userCommands(), QApplication::activeWindow() );
}

QStringList CLIToolPlugin::availableParsers() const
{
	return QStringList();
}
