#include "SearchAndReplaceV2.h"
#include "SearchWidget.h"
#include "SearchResultsDock.h"

#include <MonkeyCore.h>
#include <pWorkspace.h>
#include <pAbstractChild.h>
#include <UIMain.h>

void SearchAndReplaceV2::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "SearchAndReplaceV2" );
	mPluginInfos.Description = tr( "Search & Replace plugin" );
	mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = false;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "SearchAndReplaceV2.png", ":/icons" );
}

bool SearchAndReplaceV2::install()
{
	mWidget = new SearchWidget;
	MonkeyCore::workspace()->layout()->addWidget( mWidget );
	mWidget->setVisible( false );
	
	mDock = new SearchResultsDock( mWidget->searchThread() );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDock, mDock->windowTitle(), mDock->windowIcon() );
	mDock->setVisible( false );
	
	mWidget->setResultsDock( mDock );
	
	pMenuBar* mb = MonkeyCore::menuBar();
	QAction* action;
	
	mb->beginGroup( "mEdit/mSearchReplace" );
		action = mb->action( "aSearchFile" );
		connect( action, SIGNAL( triggered() ), this, SLOT( searchFile_triggered() ) );
		
		action = mb->action( "aReplaceFile", tr( "&Replace..." ), QIcon( ":/edit/icons/edit/replace.png" ), tr( "Ctrl+R" ), tr( "Replace in the current file..." ) );
		connect( action, SIGNAL( triggered() ), this, SLOT( replaceFile_triggered() ) );
		
		action = mb->action( "aSearchPrevious", tr( "Search Previous" ), QIcon( ":/edit/icons/edit/previous.png" ), tr( "Shift+F3" ), tr( "Search previous occurrence" ) );
		connect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbPrevious_clicked() ) );
		
		action = mb->action( "aSearchNext", tr( "Search Next" ), QIcon( ":/edit/icons/edit/next.png" ), tr( "F3" ), tr( "Search next occurrence" ) );
		connect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbNext_clicked() ) );
	mb->endGroup();
	
	return true;
}

bool SearchAndReplaceV2::uninstall()
{
	pMenuBar* mb = MonkeyCore::menuBar();
	QAction* action;
	
	mb->beginGroup( "mEdit/mSearchReplace" );
		action = mb->action( "aSearchFile" );
		disconnect( action, SIGNAL( triggered() ), this, SLOT( searchFile_triggered() ) );
		
		action = mb->action( "aReplaceFile" );
		disconnect( action, SIGNAL( triggered() ), this, SLOT( replaceFile_triggered() ) );
		delete action;
		
		action = mb->action( "aSearchPrevious" );
		disconnect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbPrevious_clicked() ) );
		delete action;
		
		action = mb->action( "aSearchNext" );
		disconnect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbNext_clicked() ) );
		delete action;
	mb->endGroup();
	
	delete mDock;
	delete mWidget;
	
	return true;
}

void SearchAndReplaceV2::searchFile_triggered()
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();
	
	if ( ( document && document->editor() ) || !document )
	{
		mWidget->setMode( SearchAndReplaceV2::ModeSearch );
	}
}

void SearchAndReplaceV2::replaceFile_triggered()
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();
	
	if ( ( document && document->editor() ) || !document )
	{
		mWidget->setMode( SearchAndReplaceV2::ModeReplace );
	}
}

Q_EXPORT_PLUGIN2( BaseSearchAndReplaceV2, SearchAndReplaceV2 )
