#include "Cpp.h"
#include "CppChild.h"
#include "AbstractProjectItemModel.h"
#include "AbstractProjectProxy.h"
//
#include <QFileInfo>
#include <QDir>
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
	// plugin infos
	mPluginInfos.Caption = tr( "C/C++ Files Plugin Manager" );
	mPluginInfos.Description = tr( "This plugin allow you to use C/C++ files" );
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name = "Cpp";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Installed = false;
	//
	mExtensions << CppChild::headerExtensions() << CppChild::sourceExtensions();
}
//
bool Cpp::install()
{
	mPluginInfos.Installed = true;
	return true;
}
//
bool Cpp::uninstall()
{
	mPluginInfos.Installed = false;
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
bool Cpp::open( const QString& s, AbstractProjectProxy* p )
{
	// try finding already existing child
	foreach ( QWidget* w, mWorkspace->documents() )
	{
		CppChild* c = qobject_cast<CppChild*>( w );
		if ( c )
		{
			// if already open return false
			if ( c->files().contains( s ) )
			{
				// active correct file in child
				c->showFile( s );
				// make this child the current one
				mWorkspace->setCurrentWidget( c );
				return false;
			}
			// else if fullPathBaseName are same, we can open it
			else if ( c->fullPathBaseName() == c->fullPathBaseName( s ) )
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
	return addNewDocument( s, p );
}
//
bool Cpp::openFile( const QString& s, AbstractProjectProxy* p )
{
	// get info on file
	QFileInfo f( s );
	// try opening relating file
	// get extensions to check
	QStringList extensions;
	if ( CppChild::headerExtensions().contains( f.completeSuffix(), Qt::CaseInsensitive ) )
		extensions << CppChild::sourceExtensions();
	else if ( CppChild::sourceExtensions().contains( f.completeSuffix(), Qt::CaseInsensitive ) )
		extensions << CppChild::headerExtensions();
	// if extensions to check
	if ( !extensions.isEmpty() )
	{
		// get all path to check in
		QStringList l;
		if ( p )
		{
			l << p->project()->getValuesList( "INCLUDEPATH" );
			l << p->project()->getValuesList( "DEPENDPATH" );
			l << p->project()->getValuesList( "VPATH" );
			// made path absolute
			for ( int i = 0; i < l.count(); i++ )
			{
				QFileInfo fs( l.at( i ) );
				if ( fs.isRelative() )
					l[i] = QString( "%1/%2" ).arg( p->project()->path(), l.at( i ) );
				l[i] = QFileInfo( l.at( i ) ).canonicalFilePath();
			}
		}
		// add file path if not present
		if ( !l.contains( f.canonicalPath() ) )
			l << f.canonicalPath();
		// loop all path
		foreach ( QString ds, l )
		{
			// go to path
			QDir d( ds );
			// looping all files
			foreach ( QString file, d.entryList( QStringList( QString( "%1.*" ).arg( f.baseName() ) ), QDir::Files ) )
			{
				// we only need to open one
				if ( extensions.contains( QFileInfo( file ).completeSuffix(), Qt::CaseInsensitive ) )
				{
					open( d.absoluteFilePath( file ), p );
					// open the requested file
					return open( f.canonicalFilePath(), p );
				}
			}
		}
	}
	// open the requested file
	return open( f.canonicalFilePath(), p );
}
//
void Cpp::saveCodeCoverage(const QString &name,const QString&teststate)
{
  QString testname=name+"/"+infos().Name;
#ifdef __COVERAGESCANNER__
  __coveragescanner_filename(codeCoverageFile().toAscii());
  __coveragescanner_teststate(teststate.toAscii());
  __coveragescanner_testname(testname.toAscii());
  __coveragescanner_save();
#endif
}
//
Q_EXPORT_PLUGIN2( ChildCpp, Cpp )
