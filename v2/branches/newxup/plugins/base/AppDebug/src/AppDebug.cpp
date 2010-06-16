#include "AppDebug.h"

#include <stdio.h>

void AppDebug::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "AppDebug" );
	mPluginInfos.Description = tr( "A plugin that help to debug Mks itself" );
	mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "AppDebug.png", ":/icons" );
}

bool AppDebug::install()
{
	qInstallMsgHandler( AppDebug::qtMessageHandler );
	
	return true;
}

bool AppDebug::uninstall()
{
	qInstallMsgHandler( 0 );
	
	return true;
}

void AppDebug::qtMessageHandler( QtMsgType type, const char* msg )
{
	switch ( type ) {
		case QtDebugMsg:
			fprintf( stderr, "Debug: %s\n", msg );
			break;
		case QtWarningMsg:
			fprintf( stderr, "Warning: %s\n", msg );
			break;
		case QtCriticalMsg:
			fprintf( stderr, "Critical: %s\n", msg );
			break;
		case QtFatalMsg:
			fprintf( stderr, "Fatal: %s\n", msg );
			abort();
	}
}

Q_EXPORT_PLUGIN2( BaseAppDebug, AppDebug )
