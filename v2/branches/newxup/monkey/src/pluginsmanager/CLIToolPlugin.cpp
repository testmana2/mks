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
#include "ui/UICLIToolSettings.h"

#include <QApplication>

CLIToolPlugin::CLIToolPlugin()
	: BasePlugin()
{
}

QWidget* CLIToolPlugin::settingsWidget() const
{
	return new UICLIToolSettings( const_cast<CLIToolPlugin*>( this ), QApplication::activeWindow() );
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

QStringList CLIToolPlugin::availableParsers() const
{
	return QStringList();
}
