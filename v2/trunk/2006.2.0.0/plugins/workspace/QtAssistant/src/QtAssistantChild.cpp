#include "QtAssistantChild.h"
#include "Workspace.h"
// assistant include
#include "mainwindow.h"
#include "helpdialog.h"
#include "tabbedbrowser.h"
#include "helpwindow.h"
//
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
//
QtAssistantChild* QtAssistantChild::mSelf = 0L;
//
QtAssistantChild* QtAssistantChild::self( Workspace* w, MainWindow* mw )
{
	if ( !mSelf )
		mSelf = new QtAssistantChild( w, mw );
	return mSelf;
}
//
QtAssistantChild::QtAssistantChild( Workspace* w, MainWindow* mw )
{
	Q_ASSERT( w );
	Q_ASSERT( mw );
	mWorkspace = w;
	mMain = mw;
	//
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	// modify some mMain widget
	mMain->menuBar()->hide();
	mMain->setIconSize( QSize( 16, 16 ) );
	mMain->statusBar()->hide();
	// connection
	connect( mMain->helpDialog(), SIGNAL( showLink( const QString& ) ), this, SLOT( showLink( const QString& ) ) );
	connect( mMain->browsers()->findChild<QTabWidget*>( "tab" ), SIGNAL( currentChanged( int ) ), this, SLOT( showLink() ) );
	// add assistant as widget child
	vl->addWidget( mMain );
	setWindowTitle( mMain->windowTitle() );
}
//
void QtAssistantChild::closeEvent( QCloseEvent* e )
{
	// don't destroy the child when we close it in workspace
	setAttribute( Qt::WA_DeleteOnClose, false );
	// call defaul closeevent of abstract child
	AbstractChild::closeEvent( e );
}
//
void QtAssistantChild::showLink( const QString& s )
{
	// check if child is in workspace
	if ( mWorkspace->indexOf( this ) != -1 )
		mWorkspace->setCurrentWidget( this );
	else
		mWorkspace->addChild( this, "Qt Assistant" );
	// check if the the current browser is already tracked
	HelpWindow* hw = mMain->browsers()->currentBrowser();
	if ( !mHelps.contains( hw ) )
	{
		mHelps << hw;
		connect( hw, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( showLink( const QUrl& ) ) );
	}
	// tell filename has changed
	emit currentFileChanged( !s.isEmpty() ? s : currentFile() );
}
//
void QtAssistantChild::showLink( const QUrl& u )
{
	// tell filename has changed
	showLink( u.toString() );
}
// return cursor position if available
QPoint QtAssistantChild::cursorPosition() const
{
	// there is no cursor position in assistant
	return QPoint( -1, -1 );
}
// show the current file in child
void QtAssistantChild::showFile( const QString& )
{ /*mMain->showLink( s );*/ }
// the current visible / focused file
QString QtAssistantChild::currentFile() const
{
	return mMain->browsers()->currentBrowser()->source().toString();
}
// return the current file modified flag
bool QtAssistantChild::isModified() const
{ return false; }
// return the current file undo flag
bool QtAssistantChild::isUndoAvailable() const
{ return false; }
// return the current file redo flag
bool QtAssistantChild::isRedoAvailable() const
{ return false; }
// return the current file paste available
bool QtAssistantChild::isPasteAvailable() const
{ return false; }
// return the current file copy available
bool QtAssistantChild::isCopyAvailable() const
{ return false; }
// return the modified state of file
bool QtAssistantChild::isModified( const QString& ) const
{ return false; }
// ask to save file
void QtAssistantChild::saveFile( const QString& )
{}
//
void QtAssistantChild::saveFiles()
{}
// ask to save file
void QtAssistantChild::loadFile( const QString& )
{}
//
void QtAssistantChild::printFile( const QString& )
{}
// ask to quick print this file
void QtAssistantChild::quickPrintFile( const QString& )
{}
// undo
void QtAssistantChild::undo()
{}
// redo
void QtAssistantChild::redo()
{}
// cut
void QtAssistantChild::cut()
{}
// copy
void QtAssistantChild::copy()
{}
// paste
void QtAssistantChild::paste()
{}
// search/replace
void QtAssistantChild::searchReplace()
{}
// go to
void QtAssistantChild::goTo()
{}
// search/replace available
bool QtAssistantChild::isSearchReplaceAvailable() const
{ return false; }
// goto available
bool QtAssistantChild::isGoToAvailable() const
{ return false; }
// print available
bool QtAssistantChild::isPrintAvailable() const
{ return false; }
