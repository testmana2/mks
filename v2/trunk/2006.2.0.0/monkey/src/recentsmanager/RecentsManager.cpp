#include "RecentsManager.h"
#include "Settings.h"
#include "MenuBar.h"
//
#include <QAction>
#include <QFileInfo>
//
QPointer<RecentsManager> RecentsManager::mSelf = 0L;
//
RecentsManager* RecentsManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new RecentsManager( p );
	return mSelf;
}
//
RecentsManager::RecentsManager( QObject* p )
	: QObject( p )
{
	initialize();
}
//
Settings* RecentsManager::settings() const
{
	return Settings::current();
}
//
MenuBar* RecentsManager::menuBar() const
{
	return MenuBar::self();
}
//
void RecentsManager::initialize()
{
	setMaxRecentFiles( maxRecentFiles() );
	setMaxRecentProjects( maxRecentProjects() );
	updateRecentFiles();
	updateRecentProjects();
	connect( menuBar()->menu( "mFile/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentFiles_triggered( QAction* ) ) );
	connect( menuBar()->menu( "mProject/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentProjects_triggered( QAction* ) ) );
}
//
int RecentsManager::maxRecentFiles() const
{
	return settings()->value( "Recents/MaxFiles", 15 ).toInt();
}
//
int RecentsManager::maxRecentProjects() const
{
	return settings()->value( "Recents/MaxProjects", 15 ).toInt();
}
//
void RecentsManager::setMaxRecentFiles( int i )
{
	if ( i != settings()->value( "Recents/MaxFiles" ).toInt() )
	{
		settings()->setValue( "Recents/MaxFiles", i );
		updateRecentFiles();
	}
}
//
void RecentsManager::setMaxRecentProjects( int i )
{
	if ( i != settings()->value( "Recents/MaxProjects" ).toInt() )
	{
		settings()->setValue( "Recents/MaxProjects", i );
		updateRecentProjects();
	}
}

//
void RecentsManager::recentFiles_triggered( QAction* a )
{
	if ( a->text() != tr( "&Clear" ) )
		emit openFileRequested( a->data().toString() );
	else if ( a->text() == tr( "&Clear" ) )
	{
		settings()->setValue( "Recents/Files", QStringList() );
		updateRecentFiles();
	}
}
//
void RecentsManager::updateRecentFiles()
{
	foreach ( QAction* a, mRecentFiles )
		a->deleteLater();
	mRecentFiles.clear();
	QStringList l = settings()->value( "Recents/Files" ).toStringList();
	QAction* a;
	for ( int i = 0; i < maxRecentFiles(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				QString s = QString( "%1 %2" ).arg( i +1 ).arg( f.fileName() );
				a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentFiles.append( a );
				menuBar()->menu( "mFile/mRecents" )->addAction( a );
			}
		}
	}
}
//
void RecentsManager::addRecentFile( const QString& s )
{
	QStringList f = settings()->value( "Recents/Files" ).toStringList();
	f.removeAll( s );
	f.prepend( s );
	while ( f.size() > maxRecentFiles() )
		f.removeLast();
	settings()->setValue( "Recents/Files", f );
	updateRecentFiles();
}
//
void RecentsManager::recentProjects_triggered( QAction* a )
{
	if ( a->text() != tr( "&Clear" ) )
		emit openProjectRequested( a->data().toString() );
	else if ( a->text() == tr( "&Clear" ) )
	{
		settings()->setValue( "Recents/Projects", QStringList() );
		updateRecentProjects();
	}
}
//
void RecentsManager::updateRecentProjects()
{
	foreach ( QAction* a, mRecentProjects )
		a->deleteLater();
	mRecentProjects.clear();
	QStringList l = settings()->value( "Recents/Projects" ).toStringList();
	QAction* a;
	for ( int i = 0; i < maxRecentProjects(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				QString s = QString( "%1 %2" ).arg( i +1 ).arg( f.fileName() );
				a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentProjects.append( a );
				menuBar()->menu( "mProject/mRecents" )->addAction( a );
			}
		}
	}
}
//
void RecentsManager::addRecentProject( const QString& s )
{
	QStringList f = settings()->value( "Recents/Projects" ).toStringList();
	f.removeAll( s );
	f.prepend( s );
	while ( f.size() > maxRecentProjects() )
		f.removeLast();
	settings()->setValue( "Recents/Projects", f );
	updateRecentProjects();
}
