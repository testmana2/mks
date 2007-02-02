#include "Ui.h"
//
#include <QDesktopServices>
#include <QUrl>
//
Ui::~Ui()
{
	if ( isInstalled() )
		uninstall();
}
//
void Ui::initialize( Workspace* w )
{
	ChildPlugin::initialize( w );
	// plugin infos
	mPluginInfos.Caption = tr( "Qt UI File Manager " );
	mPluginInfos.Description = tr( "This plugin allow you to use Qt Ui files" );
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name ="Ui";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Installed = false;
	//
	mExtensions = QStringList() << "ui";
}
//
bool Ui::install()
{
	mPluginInfos.Installed = true;
	return true;
}
//
bool Ui::uninstall()
{
	mPluginInfos.Installed = false;
	return true;
}
//
QStringList Ui::filters() const
{
	QStringList e = extensions();
	QStringList f;
	foreach ( QString s, e )
		f << QString( "*.%1" ).arg( s );
	return QStringList() << QString( "Qt Forms Files ( %1 )" ).arg( f.join( " " ) );
}
//
bool Ui::addNewDocument( const QString&, AbstractProjectProxy* )
{
	return false;
}
//
bool Ui::openFile( const QString& s, AbstractProjectProxy* )
{
	return QDesktopServices::openUrl( QUrl::fromLocalFile( s ) );
}
//
Q_EXPORT_PLUGIN2( EditorUi, Ui )
