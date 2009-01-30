#include "UIMonkeyDbg.h"
#include "../pEditor.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>

UIMonkeyDbg::UIMonkeyDbg( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	
	mDebuggerInitialized = false;
	mDebugger = new QGdb::Driver( this );
	
	initConnections();
}

UIMonkeyDbg::~UIMonkeyDbg()
{
}

void UIMonkeyDbg::showEvent( QShowEvent* event )
{
	QMainWindow::showEvent( event );
	
	if ( !mDebuggerInitialized )
	{
		mDebuggerInitialized = true;
		
		if ( !mDebugger->connectToGdb() )
		{
			debuggerStateChanged( QGdb::DISCONNECTED );
		}
	}
}

void UIMonkeyDbg::initConnections()
{
	// debuggerContinue
	connect( mDebugger, SIGNAL( callbackMessage( const QString&, QGdb::CBType ) ), this, SLOT( debuggerCallbackMessage( const QString&, QGdb::CBType ) ) );
	connect( mDebugger, SIGNAL( stateChanged( QGdb::State ) ), this, SLOT( debuggerStateChanged( QGdb::State ) ) );
	
	// gui
	connect( aCloseFile, SIGNAL( triggered() ), this, SLOT( closeCurrentFile() ) );
	connect( aCloseAll, SIGNAL( triggered() ), this, SLOT( closeAllFiles() ) );
}

void UIMonkeyDbg::appendLog( const QString& log )
{
	pteLog->appendPlainText( log );
}

void UIMonkeyDbg::appendConsole( const QString& msg )
{
	pteConsole->appendPlainText( msg );
}

void UIMonkeyDbg::appendPipe( const QString& msg )
{
	ptePipe->appendPlainText( msg );
}

bool UIMonkeyDbg::openFile( const QString& fileName )
{
	if ( mOpenedFiles.contains( fileName ) )
	{
		maWorkspace->setActiveSubWindow( mOpenedFiles[ fileName ].second );
		return true;
	}
	
	pEditor* editor = new pEditor( maWorkspace );
	
	if ( !editor->openFile( fileName, "UTF-8" ) )
	{
		delete editor;
		return false;
	}
	
	QListWidgetItem* item = new QListWidgetItem( lwFiles );
	item->setText( QFileInfo( fileName ).fileName() );
	item->setData( Qt::UserRole, fileName );
	
	QMdiSubWindow* subWindow = maWorkspace->addSubWindow( editor );
	subWindow->setWindowFilePath( fileName );
	
	mOpenedFiles[ fileName ].first = item;
	mOpenedFiles[ fileName ].second = subWindow;
	
	connect( subWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( subWindow_destroyed( QObject* ) ) );
	
	subWindow->showMaximized();
	
	return true;
}

void UIMonkeyDbg::closeCurrentFile()
{
	maWorkspace->closeActiveSubWindow();
}

void UIMonkeyDbg::closeAllFiles()
{
	maWorkspace->closeAllSubWindows();
}

bool UIMonkeyDbg::loadTarget( const QString& fileName )
{
}

void UIMonkeyDbg::debuggerRun()
{
}

void UIMonkeyDbg::debuggerContinue()
{
}

void UIMonkeyDbg::debuggerStepInto( bool instruction )
{
}

void UIMonkeyDbg::debuggerStepOver( bool instruction )
{
}

void UIMonkeyDbg::debuggerStepOut()
{
}

void UIMonkeyDbg::debuggerStop()
{
}

void UIMonkeyDbg::debuggerKill()
{
}

void UIMonkeyDbg::debuggerCallbackMessage( const QString& message, QGdb::CBType type )
{
	switch ( type )
	{
		case QGdb::CONSOLE:
			appendConsole( message );
			break;
		case QGdb::TARGET:
			appendLog( QString( "TARGET> %1" ).arg( message ) );
			break;
		case QGdb::LOG:
			appendLog( QString( "LOG> %1" ).arg( message ) );
			break;
		case QGdb::TO_GDB:
			appendPipe( QString( ">> %1" ).arg( message ) );
			break;
		case QGdb::FROM_GDB:
			appendPipe( QString( "<< %1" ).arg( message ) );
			break;
		case QGdb::ASYNC:
			appendLog( QString( "ASYNC> %1" ).arg( message ) );
			break;
	}
}

void UIMonkeyDbg::debuggerStateChanged( QGdb::State state )
{
	appendLog( QString( "State changed to: %1" ).arg( state ) );
	
	switch ( state )
	{
		case QGdb::DISCONNECTED:
			aLoadTarget->setEnabled( false );
			aRun->setEnabled( false );
			aContinue->setEnabled( false );
			aStepInto->setEnabled( false );
			aStepOver->setEnabled( false );
			aStepOut->setEnabled( false );
			aStop->setEnabled( false );
			aKill->setEnabled( false );
			break;
		case QGdb::CONNECTED:
			aLoadTarget->setEnabled( true );
			aRun->setEnabled( false );
			aContinue->setEnabled( false );
			aStepInto->setEnabled( false );
			aStepOver->setEnabled( false );
			aStepOut->setEnabled( false );
			aStop->setEnabled( false );
			aKill->setEnabled( false );
			break;
		case QGdb::TARGET_SETTED:
			aLoadTarget->setEnabled( false );
			aRun->setEnabled( true );
			aContinue->setEnabled( false );
			aStepInto->setEnabled( false );
			aStepOver->setEnabled( false );
			aStepOut->setEnabled( false );
			aStop->setEnabled( false );
			aKill->setEnabled( false );
			break;
		case QGdb::RUNNING:
			aLoadTarget->setEnabled( false );
			aRun->setEnabled( false );
			aContinue->setEnabled( false );
			aStepInto->setEnabled( false );
			aStepOver->setEnabled( false );
			aStepOut->setEnabled( false );
			aStop->setEnabled( true );
			aKill->setEnabled( true );
			break;
		case QGdb::STOPPED:
			aLoadTarget->setEnabled( false );
			aRun->setEnabled( false );
			aContinue->setEnabled( true );
			aStepInto->setEnabled( true );
			aStepOver->setEnabled( true );
			aStepOut->setEnabled( true );
			aStop->setEnabled( false );
			aKill->setEnabled( true );
			break;
	}
}

void UIMonkeyDbg::subWindow_destroyed( QObject* object )
{
	QWidget* widget = qobject_cast<QWidget*>( object );
	const QString file = widget->windowFilePath();
	
	delete mOpenedFiles[ file ].first;
	mOpenedFiles.remove( file );
}

void UIMonkeyDbg::on_lwFiles_itemActivated( QListWidgetItem* item )
{
	QMdiSubWindow* subWindow = mOpenedFiles.value( item->data( Qt::UserRole ).toString() ).second;
	
	if ( subWindow && maWorkspace->activeSubWindow() != subWindow )
	{
		maWorkspace->setActiveSubWindow( subWindow );
	}
}

void UIMonkeyDbg::on_maWorkspace_subWindowActivated( QMdiSubWindow* subWindow )
{
	if ( !subWindow )
	{
		return;
	}
	
	QListWidgetItem* item = mOpenedFiles.value( subWindow->windowFilePath() ).first;
	
	if ( item && lwFiles->currentItem() != item )
	{
		lwFiles->setCurrentItem( item );
	}
}

void UIMonkeyDbg::on_aOpenFile_triggered()
{
	const QStringList files = QFileDialog::getOpenFileNames( this, tr( "Open sources files" ), ".", QString::null );
	
	foreach ( const QString& file, files )
	{
		if ( !openFile( file ) )
		{
			QMessageBox::warning( this, tr( "Warning..." ), tr( "An error occur while opening file: %1" ).arg( file ) );
		}
	}
}

void UIMonkeyDbg::on_aLoadTarget_triggered()
{
}
