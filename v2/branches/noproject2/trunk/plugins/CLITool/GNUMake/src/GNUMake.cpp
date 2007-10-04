#include "GNUMake.h"
#include "GNUMakeParser.h"
#include "pConsoleManager.h"

GNUMake::GNUMake ()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "GNUMake" );
	mPluginInfos.Description = tr( "Plugin for execute GNU Make in console and parse it's output" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iCLITool;
	mPluginInfos.Name = "GNUMake";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

bool GNUMake::setEnabled( bool b)
{
	if ( b == mPluginInfos.Enabled)
		return true;
	mPluginInfos.Enabled = b;
 	if (b)
 		pConsoleManager::instance()->addParser (new GNUMakeParser());
 	else
 		pConsoleManager::instance()->removeParser ("GNUMake");
	return true;
}

QString GNUMake::name()
{
    return QString ("GNUMake");
}

QString GNUMake::getCommand ()
{
    //here possible will be some code, determining execute command, etc
    return QString ("make"); 
}

QString GNUMake::getSwitches (QString oldSwitches)
{
    //here possible will be code, showing to user config dialog
    return oldSwitches;
}

pCommandParser* GNUMake::generateParser()
{
    return new GNUMakeParser();
}

Q_EXPORT_PLUGIN2( CLIToolGNUMake, GNUMake )

