#include "Cpp.h"
#include "CppChild.h"
//
#include <QFileInfo>
//
Cpp::~Cpp()
{
	if ( isInstalled() )
		uninstall();
}
//
void Cpp::initialize( Workspace* w )
{
	ChildPlugin::initialize( w );
	mExtensions << CppChild::headerExtensions() << CppChild::sourceExtensions();
}
//
QString Cpp::name() const
{
	return "Cpp";
}
//
QString Cpp::description() const
{
	return "This plugin handle C/C++ files";
}
//
bool Cpp::install()
{
	mInstalled = true;
	return true;
}
//
bool Cpp::uninstall()
{
	mInstalled = false;
	return true;
}
//
QStringList Cpp::filters() const
{
	QStringList f, l;
	// all
	foreach ( QString s, mExtensions )
		l << QString( "*.%1" ).arg( s );
	f << QString( "C/C++ Files ( %1 )" ).arg( l.join( " " ) );
	// headers
	l.clear();
	foreach ( QString s, CppChild::headerExtensions() )
		l << QString( "*.%1" ).arg( s );
	f << QString( "C/C++ Header Files ( %1 )" ).arg( l.join( " " ) );
	// sources
	l.clear();
	foreach ( QString s, CppChild::sourceExtensions() )
		l << QString( "*.%1" ).arg( s );
	f << QString( "C/C++ Source Files ( %1 )" ).arg( l.join( " " ) );
	//
	return f;
}
//
bool Cpp::addNewDocument( const QString& s, AbstractProjectProxy* p )
{
	CppChild* c = new CppChild;
	c->setProxy( p );
	c->addFile( s );
	int i = mWorkspace->addChild( c, QFileInfo( s ).baseName() );
	if ( i == -1 )
		delete c;
	return c;
}
//
bool Cpp::openFile( const QString& s, AbstractProjectProxy* p )
{
	QString fp = QFileInfo( s ).canonicalFilePath();
	// try finding already existing child
	foreach ( QWidget* w, mWorkspace->documents() )
	{
		CppChild* c = qobject_cast<CppChild*>( w );
		if ( c && c->proxy() == p )
		{
			// if already open return false
			if ( c->files().contains( fp ) )
			{
				// active correct file in child
				c->showFile( s );
				// make this child the current one
				mWorkspace->setCurrentWidget( c );
				return false;
			}
			// else if fullPathBaseName are same, we can open it
			else if ( c->fullPathBaseName() == c->fullPathBaseName( fp ) )
			{
				// open file in child
				c->addFile( s );
				// make this child the current one
				mWorkspace->setCurrentWidget( c );
				return true;
			}
		}
	}
	// else create a new one
	return addNewDocument( fp, p );
}
//
Q_EXPORT_PLUGIN2( ChildCpp, Cpp )
