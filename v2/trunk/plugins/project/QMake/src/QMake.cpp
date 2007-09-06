#include "QMake.h"
#include "QMakeItem.h"
#include "QMakeParser.h"
#include "UISettingsQMake.h"
#include "UIQMakeProjectSettings.h"

QMake::QMake()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "QMake" );
	mPluginInfos.Description = tr( "Plugin for managing QMake project" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "QMake";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

QMake::~QMake()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool QMake::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set usable suffixes
		mSuffixes[tr( "Qt Projects" )] = QStringList() << "*.pro" << "*.pri" << "*.inc";
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// clear suffixes
		mSuffixes.clear();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QWidget* QMake::settingsWidget()
{
	return UISettingsQMake::instance();
}

ProjectItem* QMake::openProject( const QString& s, ProjectItem* pi )
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() )
		return 0;
	// crete root project item
	QMakeItem* it =  new QMakeItem;
	// parse project
	QMakeParser p( s, it );
	// return root item
	return it;
}

void QMake::editSettings( ProjectItem* it )
{
	if ( it )
		UIQMakeProjectSettings::instance( it )->exec();
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
