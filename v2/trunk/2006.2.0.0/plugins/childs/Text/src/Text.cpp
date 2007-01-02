#include "Text.h"
#include "TextChild.h"
//
#include <QFileInfo>
//
Text::~Text()
{
	if ( isInstalled() )
		uninstall();
}
//
void Text::initialize( Workspace* w )
{
	ChildPlugin::initialize( w );
	mExtensions = QStringList() << "txt" << "cfg" << "ini" << "conf";
}
//
QString Text::name() const
{
	return "Text";
}
//
QString Text::description() const
{
	return "This plugin can show plain text file: txt, cfg and ini";
}
//
bool Text::install()
{
	mInstalled = true;
	return true;
}
//
bool Text::uninstall()
{
	mInstalled = false;
	return true;
}
//
QStringList Text::filters() const
{
	QStringList e = extensions();
	QStringList f;
	foreach ( QString s, e )
		f << QString( "*.%1" ).arg( s );
	return QStringList() << QString( "Text Files ( %1 )" ).arg( f.join( " " ) );
}
//
bool Text::addNewDocument( const QString& s, AbstractProjectProxy* p )
{
	TextChild* c = new TextChild;
	c->setProxy( p );
	c->addFile( s );
	int i = mWorkspace->addChild( c, QFileInfo( s ).fileName() );
	if ( i == -1 )
		delete c;
	return c;
}
//
bool Text::openFile( const QString& s, AbstractProjectProxy* p )
{
	QString f = QFileInfo( s ).canonicalFilePath();
	// try finding already existing child
	foreach ( QWidget* w, mWorkspace->documents() )
	{
		TextChild* c = qobject_cast<TextChild*>( w );
		if ( c && c->proxy() == p && c->files().contains( f ) )
		{
			mWorkspace->setCurrentWidget( c );
			return false;
		}
	}
	// else create a new one
	return addNewDocument( f, p );
}
//
Q_EXPORT_PLUGIN2( EditorText, Text )
