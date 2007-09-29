/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIProjectsManager
** FileName  : UIProjectsManager.cpp
** Date      : 2007-09-03T20:14:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UIProjectsManager.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pRecentsManager.h"
#include "ProjectsProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "pFileManager.h"
#include "UITemplatesWizard.h"
#include "pTreeComboBox.h"

#include <QHeaderView>

using namespace pMonkeyStudio;

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mProjects( new ProjectsModel( this ) ), mProxy( new ProjectsProxy( mProjects ) )
{
	setupUi( this );
	
	// set icons size
	tbButtons->setIconSize( QSize( 16, 16 ) );
	
	// set layout margin & spacing
	tbButtons->layout()->setMargin( 0 );
	tbButtons->layout()->setSpacing( 0 );
	
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// set buttons action
	tbButtons->addAction( mb->action( "mView/aFiltered" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aNew" ) );
	tbButtons->addAction( mb->action( "mProject/aOpen" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mSave/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mSave/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mClose/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mClose/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aSettings" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aAddExistingFiles" ) );
	
	// set treeview view for combo list
	cbProjects->setModel( mProjects->projectsProxy() );
	
	// set projects properties
	tvProjects->header()->hide();
	
	// set proxy properties
	// set types to filter
	mProxy->setFilterRoles( QList<int>() << ProjectItem::ProjectType << ProjectItem::ValueType );
	// filter are negate
	mProxy->setNegateFilter( false );
	// apply filtering
	mProxy->setFiltering( true );
	// set view proxy
	tvProjects->setModel( mProxy );
	
	// set filter filtering state
	mb->action( "mView/aFiltered" )->setChecked( mProxy->isFiltering() );
	
	// connections
	connect( mProxy, SIGNAL( filteringChanged( bool ) ), mb->action( "mView/aFiltered" ), SLOT( setChecked( bool ) ) );
	connect( cbProjects, SIGNAL( activated( const QModelIndex& ) ), this, SLOT( cbProjects_activated( const QModelIndex& ) ) );
	connect( cbProjects, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( cbProjects_activated( const QModelIndex& ) ) );
	connect( tvProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvProjects_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
}

UIProjectsManager::~UIProjectsManager()
{ projectCloseAll_triggered(); }

ProjectItem* UIProjectsManager::currentProject() const
{
	// get current item
	ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( tvProjects->currentIndex() ) );
	// return project item
	return it ? it->project() : 0;
}

void UIProjectsManager::initializeProject( ProjectItem* it )
{
	// clear selected item
	tvProjects->selectionModel()->clear();
	// append project item
	mProjects->appendRow( it );
	// refresh project
	mProxy->refresh( it );
	// set current project
	tvProjects->setCurrentIndex( mProxy->mapFromSource( it->index() ) );
}

void UIProjectsManager::cbProjects_activated( const QModelIndex& i )
{ tvProjects->setCurrentIndex( mProxy->mapFromSource( mProjects->projectsProxy()->mapToSource( i ) ) ); }

void UIProjectsManager::tvProjects_currentChanged( const QModelIndex& c, const QModelIndex& )
{
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// get pluginsmanager
	PluginsManager* pm = PluginsManager::instance();
	// get current project
	ProjectItem* it = currentProject();
	// set compiler, debugger and interpreter
	pm->setCurrentCompiler( it ? it->compiler() : 0 );
	pm->setCurrentDebugger( it ? it->debugger() : 0 );
	pm->setCurrentInterpreter( it ? it->interpreter() : 0 );
	// desactive project action
	mb->action( "mProject/mSave/aCurrent" )->setEnabled( it );
	mb->action( "mProject/mSave/aAll" )->setEnabled( it );
	mb->action( "mProject/mClose/aCurrent" )->setEnabled( it );
	mb->action( "mProject/mClose/aAll" )->setEnabled( it );
	mb->action( "mProject/aSettings" )->setEnabled( it );
	mb->action( "mProject/aAddExistingFiles" )->setEnabled( it );
	// select correct project in combobox
	cbProjects->setCurrentIndex( it ? mProjects->projectsProxy()->mapFromSource( it->index() ) : QModelIndex() );
}

void UIProjectsManager::on_tvProjects_doubleClicked( const QModelIndex& i )
{
	// get item
	ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( i ) );
	// if item and file exists, open it
	if ( it && QFile::exists( it->getFilePath() ) )
		pFileManager::instance()->openFile( it->getFilePath() );
}

bool UIProjectsManager::openProject( const QString& s )
{
	if ( ProjectItem* it = PluginsManager::instance()->projectItem( s ) )
	{
		if ( it->open() )
		{
			initializeProject( it );
			return true;
		}
		else
			delete it;
	}
	else
		warning( tr( "Open Project..." ), tr( "There is no plugin that can manage this project :\n[%1]" ).arg( s ) );
	return false;
}

bool UIProjectsManager::closeProject( const QString& s )
{
	foreach ( ProjectItem* p, mProjects->projects( true ) )
		if ( p->canonicalFilePath() == QFileInfo( s ).canonicalFilePath() )
		{
			p->close();
			return true;
		}
	return false;
}

void UIProjectsManager::projectNew_triggered()
{
	UITemplatesWizard* d = UITemplatesWizard::instance( this );
	d->setType( pTemplate::ttProjects );
	d->exec();
}

void UIProjectsManager::projectOpen_triggered()
{
	// get last file open path
	const QString mPath = pRecentsManager::instance()->recentProjectOpenPath();
	// get available filters
	QString mFilters = availableProjectsFilters();
	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, availableProjectsSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Projects (%1);;" ).arg( s.trimmed() ) );
	}
	// open open file dialog
	QStringList l = getOpenFileNames( tr( "Choose the project(s) to open" ), mPath, mFilters, window() );
	// for each entry, open file
	foreach ( QString s, l )
	{
		if ( openProject( s ) )
			// append file to recents
			pRecentsManager::instance()->addRecentProject( s );
		else
			// remove it from recents files
			pRecentsManager::instance()->removeRecentProject( s );
	}
	// store file open path
	if ( !l.isEmpty() )
		pRecentsManager::instance()->setRecentProjectOpenPath( QFileInfo( l.at( 0 ) ).canonicalPath() );
}

void UIProjectsManager::projectSaveCurrent_triggered()
{
	ProjectItem* p = currentProject();
	if ( p )
		p->save( false );
}

void UIProjectsManager::projectSaveAll_triggered()
{
	foreach ( ProjectItem* p, mProjects->projects( false ) )
		p->saveAll( false );
}

void UIProjectsManager::projectCloseCurrent_triggered()
{
	ProjectItem* p = currentProject();
	if ( p )
	{
		 // fucking Qt bug that not emit currentChange is current item is not toplevelitem
		tvProjects->setCurrentIndex( mProxy->mapFromSource( p->index() ) );
		p->close();
	}
}

void UIProjectsManager::projectCloseAll_triggered()
{
	foreach ( ProjectItem* p, mProjects->projects( false ) )
	{
		 // fucking Qt bug that not emit currentChange is current item is not toplevelitem
		tvProjects->setCurrentIndex( mProxy->mapFromSource( p->index() ) );
		p->close();
	}
}

void UIProjectsManager::projectSettings_triggered()
{
	if ( ProjectItem* it = currentProject() )
		it->editSettings();
}

void UIProjectsManager::projectAddExistingFiles_triggered()
{
	if ( ProjectItem* it = currentProject() )
	{
		QStringList l = getOpenFileNames( tr( "Choose file(s) to add to your project" ), it->canonicalPath() );
		if ( !l.isEmpty() )
			it->addExistingFiles( l, it, "=" );
	}
}

