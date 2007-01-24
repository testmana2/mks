#include "MessageBox.h"
#include "UIMessageBox.h"
#include "MenuBar.h"
#include "PluginsManager.h"
#include "TabToolBar.h"
#include "Console.h"
//
#include <QScrollBar>
//
void MessageBox::initialize( Workspace* w )
{
	WorkspacePlugin::initialize( w );
	//
	mPluginInfos.Caption = tr( "Message Box" );
	mPluginInfos.Description = tr( "Message box for messages and errors" );
	mPluginInfos.Type = BasePlugin::iWorkspace;
	mPluginInfos.Name = "MessageBox";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Installed = false;
	//
	mMessageBox = 0L;
	errorsCount = 0;
	warningsCount = 0;
};
//
bool MessageBox::install()
{
	if ( isInstalled() )
		return true;
	// reset values
	errorsCount = 0;
	warningsCount = 0;	
	// create dock
	mMessageBox = UIMessageBox::self( mWorkspace );
	// create parser
	parser = new gccParser;
	// create actions
	mWorkspace->menuBar()->action( "aSeparator3" );
	QAction* aShowConsole = mWorkspace->menuBar()->action( "mView/aShowConsole", tr( "Show Console" ), QIcon( ":/icons/tabconsole.png" ), tr( "F11" ) );
	QAction* aShowListBox = mWorkspace->menuBar()->action( "mView/aShowListBox", tr( "Show ListBox" ), QIcon( ":/icons/builderrorR.png" ), tr( "F12" ) );
	// connections
	connect( aShowConsole, SIGNAL( triggered() ), this, SLOT( showConsole() ) );
	connect( aShowListBox, SIGNAL( triggered() ), this, SLOT( showListBox() ) );
	// connect to Console bridge
	BasePlugin* bp = mWorkspace->pluginsManager()->plugin( "Console" );
	if ( bp )
	{
		connect( bp, SIGNAL( messageBox( const QString& ) ), this, SLOT( messageBox( const QString& ) ) );
		connect( bp, SIGNAL( clearMessageBox() ), this, SLOT( clearMessageBox() ) );
		connect( bp, SIGNAL( dataAvailable( const QString& ) ), this, SLOT( dataAvailable( const QString& ) ) );
		connect( bp, SIGNAL( showListBox() ), this, SLOT( showListBox() ) );
		connect( bp, SIGNAL( showConsole() ), this, SLOT( showConsole() ) );
	}
	// add dock to tabbar
	mWorkspace->tabToolBar()->bar( TabToolBar::Bottom )->appendTab( mMessageBox,  QPixmap( ":/icons/builderror.png" ), tr( "Message Box" ) );
	// set plugin installed
	mPluginInfos.Installed = true;
	return true;
};
//
bool MessageBox::uninstall()
{
	if ( !isInstalled() )
		return true;
	// disconnect actions
	QAction* aShowConsole = mWorkspace->menuBar()->action( "mView/aShowConsole" );
	QAction* aShowListBox = mWorkspace->menuBar()->action( "mView/aShowListBox" );
	disconnect( aShowConsole, SIGNAL( triggered() ), this, SLOT( showConsole() ) );
	disconnect( aShowListBox, SIGNAL( triggered() ), this, SLOT( showListBox() ) );
	// delete actions
	delete aShowConsole;
	delete aShowListBox;
	// connect to Console bridge
	BasePlugin* bp = mWorkspace->pluginsManager()->plugin( "Console" );
	if ( bp )
	{
		disconnect( bp, SIGNAL( messageBox( const QString& ) ), this, SLOT( messageBox( const QString& ) ) );
		disconnect( bp, SIGNAL( clearMessageBox() ), this, SLOT( clearMessageBox() ) );
		disconnect( bp, SIGNAL( dataAvailable( const QString& ) ), this, SLOT( dataAvailable( const QString& ) ) );
		disconnect( bp, SIGNAL( showListBox() ), this, SLOT( showListBox() ) );
		disconnect( bp, SIGNAL( showConsole() ), this, SLOT( showConsole() ) );
	}
	// remove dock from tabtoolbar
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->removeTab( mMessageBox );
	// delete dock
	delete mMessageBox;
	// delete parser
	delete parser;
	// set plugin not installed
	mPluginInfos.Installed = false;
	return true;
};
//
void MessageBox::messageBox( const QString& s ) 
{
	if ( !isInstalled() )
		return;
	// we check if the scroll bar is at maximum
	bool b = mMessageBox->tbMessages->verticalScrollBar()->value() == mMessageBox->tbMessages->verticalScrollBar()->maximum();
	// append text
	mMessageBox->tbMessages->moveCursor( QTextCursor::End );
	mMessageBox->tbMessages->insertHtml( s +"<br />" );
	// if scrollbar is at maximum, increase it
	if ( b )
		mMessageBox->tbMessages->verticalScrollBar()->setValue( mMessageBox->tbMessages->verticalScrollBar()->maximum() );
}
//
void MessageBox::clearMessageBox()
{
	if ( !isInstalled() )
		return;
	mMessageBox->lwErrors->clear();
	mMessageBox->tbMessages->clear();
	//
	errPointers.clear();
	errorsCount = 0;
	warningsCount = 0;
}
//
void MessageBox::dataAvailable( const QString& s )
{
	if ( !isInstalled() )
		return;
	// we check if the scroll bar is at maximum
	bool b = mMessageBox->tbMessages->verticalScrollBar()->value() == mMessageBox->tbMessages->verticalScrollBar()->maximum();
	// append log
	mMessageBox->tbMessages->moveCursor( QTextCursor::End );
	mMessageBox->tbMessages->insertPlainText( s );
	// if scrollbar is at maximum, increase it
	if ( b )
		mMessageBox->tbMessages->verticalScrollBar()->setValue( mMessageBox->tbMessages->verticalScrollBar()->maximum() );
	// parsing log
	QVector<errStruct*>* vector = parser->parseString( s );
	if ( vector == NULL )
		return;
	// loop on errors
	errPointers += *vector;
	// for each vector create an item in list widget
	foreach ( errStruct* e, *vector )
	{
		QListWidgetItem* it = new QListWidgetItem( mMessageBox->lwErrors );
		it->setText( QString( "%1:%2: %3" ).arg( e->fileName ).arg( e->line ).arg( e->text ) );
		switch ( e->type )
		{
		case error:
			it->setIcon( QIcon( ":/icons/builderror.png" ) );
			it->setBackground( QColor( 255, 0, 0, 20 ) );
			errorsCount++;
			break;
		case warning:
			it->setIcon( QIcon( ":/icons/buildwarning.png" ) );
			it->setBackground( QColor( 0, 255, 0, 20 ) );
			warningsCount++;
			break;
		}
	}
}
//
void MessageBox::showListBox() 
{
	if ( !isInstalled() )
		return;
	if ( !mMessageBox->isVisible() )
		mMessageBox->show();
	mMessageBox->twMessageBox->setCurrentWidget( mMessageBox->lwErrors );
};
//
void MessageBox::showConsole() 
{
	if ( !isInstalled() )
		return;
	if ( !mMessageBox->isVisible() )
		mMessageBox->show();
	mMessageBox->twMessageBox->setCurrentWidget( mMessageBox->tbMessages );
};
//
/*
void MessageBox::buildComplete (bool nullExitCode) 
{
	if ( nullExitCode)
	{	QListWidgetItem* item = new QListWidgetItem	(QIcon ( ":/icons/buildwarning.png"  ) ,
							( QString ("Build complete with 0 errors and %1 warning(s). [beta:If you see wrong message, please, send console output to the trac.monkeystudio.org]").arg(warningsCount))	,
							ui->listWidget);	
		item->setBackground(QBrush (QColor(0, 255, 0, 70)));	
	}
	else
	{	QListWidgetItem* item = new QListWidgetItem	(QIcon ( ":/icons/builderror.png"  ) ,
							( QString ("Build failed. %1 error(s) and %2 warning(s). [beta:If you see wrong message, please, send console output to the trac.monkeystudio.org]").arg (errorsCount).arg(warningsCount))	,
							ui->listWidget);	
		item->setBackground(QBrush (QColor(255, 0, 0, 70)));	
	}
	errPointers.append ( (errStruct*)NULL);
};
*/
void MessageBox::makeGoto ()
{
/*
	if (errPointers [ui->listWidget->currentRow()] != NULL)
		emit gotoLine ( errPointers[ui->listWidget->currentRow()]->fileName, errPointers[ui->listWidget->currentRow()]->line);
*/
};
//
Q_EXPORT_PLUGIN2( WorkspaceMessageBox, MessageBox )
