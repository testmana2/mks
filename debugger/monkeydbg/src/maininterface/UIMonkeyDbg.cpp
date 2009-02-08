#include "UIMonkeyDbg.h"
#include "../pEditor.h"
#include "CallStackWidget.h"
#include "BreakpointWidget.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>

UIMonkeyDbg::UIMonkeyDbg( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	
	aCloseFile->setEnabled( false );
	aCloseAllFiles->setEnabled( false );
	
	mDebuggerInitialized = false;
	mDebugger = new QGdb::Driver( this );
	
	initConnections();
	
	CallStackWidget* csw = new CallStackWidget( mDebugger, dwCallStack );
	dwCallStack->setWidget( csw );
	
	BreakpointWidget* bpw = new BreakpointWidget( mDebugger, dwBreakpoint );
	dwBreakpoint->setWidget( bpw );
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
			QMessageBox::critical( this, tr( "Critical..." ), tr( "Can't connect to gdb" ) );
		}
	}
}

void UIMonkeyDbg::initConnections()
{
	// debugger
	connect( mDebugger, SIGNAL( callbackMessage( const QString&, QGdb::CBType ) ), this, SLOT( debuggerCallbackMessage( const QString&, QGdb::CBType ) ) );
	connect( mDebugger, SIGNAL( stateChanged( QGdb::State ) ), this, SLOT( debuggerStateChanged( QGdb::State ) ) );
	connect( mDebugger, SIGNAL( signalReceived( const QGdb::Signal& ) ), this, SLOT( debuggerSignalReceived( const QGdb::Signal& ) ) );
	connect( mDebugger, SIGNAL( exitSignalReceived( const QGdb::Signal& ) ), this, SLOT( debuggerExitSignalReceived( const QGdb::Signal& ) ) );
	connect( mDebugger, SIGNAL( exited( int ) ), this, SLOT( debuggerExited( int ) ) );
	connect( mDebugger, SIGNAL( callStackUpdated( const QGdb::CallStackFrameList&, int ) ), this, SLOT( debuggerCallStackUpdated( const QGdb::CallStackFrameList&, int ) ) );
	connect( mDebugger, SIGNAL( positionChanged( const QString&, int ) ), this, SLOT( debuggerPositionChanged( const QString&, int ) ) );
	connect( mDebugger, SIGNAL( breakpointAdded( const QGdb::Breakpoint& ) ), this, SLOT( debuggerBreakpointAdded( const QGdb::Breakpoint& ) ) );
	connect( mDebugger, SIGNAL( breakpointRemoved( const QGdb::Breakpoint& ) ), this, SLOT( debuggerBreakpointRemoved( const QGdb::Breakpoint& ) ) );
	
	// gui
	connect( aCloseFile, SIGNAL( triggered() ), this, SLOT( closeCurrentFile() ) );
	connect( aCloseAllFiles, SIGNAL( triggered() ), this, SLOT( closeAllFiles() ) );
	connect( aRun, SIGNAL( triggered() ), this, SLOT( debuggerRun() ) );
	connect( aContinue, SIGNAL( triggered() ), this, SLOT( debuggerContinue() ) );
	connect( aStepOut, SIGNAL( triggered() ), this, SLOT( debuggerStepOut() ) );
	connect( aStop, SIGNAL( triggered() ), this, SLOT( debuggerStop() ) );
	connect( aKill, SIGNAL( triggered() ), this, SLOT( debuggerKill() ) );
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

pEditor* UIMonkeyDbg::openFile( const QString& fileName )
{
	if ( mOpenedFiles.contains( fileName ) )
	{
		maWorkspace->setActiveSubWindow( mOpenedFiles[ fileName ].second );
		return qobject_cast<pEditor*>( mOpenedFiles[ fileName ].second->widget() );
	}
	
	pEditor* editor = new pEditor( maWorkspace );
	editor->setReadOnly( true );
	editor->setMarginLineNumbers( 1, true );
	editor->setMarginWidth( 1, 50 );
	editor->setMarginSensitivity( 1, true );
	editor->setFolding( QsciScintilla::BoxedTreeFoldStyle );
	editor->setIndentationWidth( 4 );
	editor->setCaretLineVisible( true );
	
	if ( !editor->openFile( fileName, "UTF-8" ) )
	{
		delete editor;
		return 0;
	}
	
	QListWidgetItem* item = new QListWidgetItem( lwFiles );
	item->setText( QFileInfo( fileName ).fileName() );
	item->setData( Qt::UserRole, fileName );
	
	QMdiSubWindow* subWindow = maWorkspace->addSubWindow( editor );
	subWindow->setWindowFilePath( fileName );
	
	mOpenedFiles[ fileName ].first = item;
	mOpenedFiles[ fileName ].second = subWindow;
	
	connect( editor, SIGNAL( breakpointToggled( int ) ), this, SLOT( editorBreakpointToggled( int ) ) );
	connect( subWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( subWindow_destroyed( QObject* ) ) );
	
	subWindow->showMaximized();
	
	return editor;
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
	return mDebugger->exec_setExecutable( fileName );
}

void UIMonkeyDbg::debuggerRun()
{
	if ( !mDebugger->exec_run() )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't run: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerContinue()
{
	if ( !mDebugger->exec_continue() )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't continue: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerStepInto( bool instruction )
{
	if ( !mDebugger->exec_stepInto( instruction ) )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't step into: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerStepOver( bool instruction )
{
	if ( !mDebugger->exec_stepOver( instruction ) )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't step over: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerStepOut()
{
	if ( !mDebugger->exec_stepOut() )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't step out: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerStop()
{
	if ( !mDebugger->exec_stop() )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't stop: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::debuggerKill()
{
	if ( !mDebugger->exec_kill() )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "Can't kill: %1" ).arg( mDebugger->lastError() ) );
	}
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
		case QGdb::ERROR:
			QMessageBox::critical( this, tr( "Critical..." ), message );
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

void UIMonkeyDbg::debuggerSignalReceived( const QGdb::Signal& signal )
{
	QMessageBox::critical( window(), tr( "Signal received..." ), tr( "Program received signal %1, %2" ).arg( signal.name ).arg( signal.meaning ) );

	if ( !mDebugger->stack_listFrames() )
	{
		QMessageBox::information( window(), tr( "Getting call stack..." ), tr( "Can't get the call stack, or corrupted traces." ) );
	}
}

void UIMonkeyDbg::debuggerExitSignalReceived( const QGdb::Signal& signal )
{
	QMessageBox::critical( window(), tr( "Signal received..." ), tr( "Program exited signal %1, %2" ).arg( signal.name ).arg( signal.meaning ) );
}

void UIMonkeyDbg::debuggerExited( int code )
{
	QMessageBox::information( window(), tr( "Program exited..." ), tr( "Program exited with code: %1" ).arg( code ) );
}

void UIMonkeyDbg::debuggerCallStackUpdated( const QGdb::CallStackFrameList& stack, int selectedLevel )
{
	Q_UNUSED( stack );
	Q_UNUSED( selectedLevel );
	appendLog( "callstack updated" );
}

void UIMonkeyDbg::debuggerPositionChanged( const QString& fileName, int line )
{
	pEditor* editor = 0;
	foreach ( QMdiSubWindow* window, maWorkspace->subWindowList() )
	{
		pEditor* tmpEditor = qobject_cast<pEditor*>( window->widget() );
		tmpEditor->clearDebuggerPosition();
		
		if ( window->windowFilePath() == fileName )
		{
			editor = tmpEditor;
		}
	}
	
	if ( !editor )
	{
		editor = openFile( fileName );
	}
	
	if ( editor )
	{
		QMdiSubWindow* window = qobject_cast<QMdiSubWindow*>( editor->parentWidget() );
		maWorkspace->setActiveSubWindow( window );
		editor->setDebuggerPosition( line -1 );
	}
}

void UIMonkeyDbg::debuggerBreakpointAdded( const QGdb::Breakpoint& breakpoint )
{
	pEditor* editor = 0;
	
	if ( mOpenedFiles.keys().contains( breakpoint.absolute_file ) )
	{
		QMdiSubWindow* subWindow = mOpenedFiles[ breakpoint.absolute_file ].second;
		editor = qobject_cast<pEditor*>( subWindow->widget() );
	}
	else
	{
		editor = openFile( breakpoint.absolute_file );
	}
	
	editor->setBreakpoint( breakpoint.line -1, pEditor::mdEnabledBreak ); // breakpoint.type
}

void UIMonkeyDbg::debuggerBreakpointRemoved( const QGdb::Breakpoint& breakpoint )
{
	pEditor* editor = 0;
	
	if ( mOpenedFiles.keys().contains( breakpoint.absolute_file ) )
	{
		QMdiSubWindow* subWindow = mOpenedFiles[ breakpoint.absolute_file ].second;
		editor = qobject_cast<pEditor*>( subWindow->widget() );
	}
	
	if ( editor )
	{
		editor->setBreakpoint( breakpoint.line -1, -1 );
	}
}

void UIMonkeyDbg::editorBreakpointToggled( int line )
{
	pEditor* editor = qobject_cast<pEditor*>( sender() );
	QMdiSubWindow* subWindow = qobject_cast<QMdiSubWindow*>( editor->parentWidget() );
	const QString fileName = subWindow->windowFilePath();
	
	mDebugger->break_breakpointToggled( fileName, line +1 );
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
	aCloseFile->setEnabled( subWindow );
	aCloseAllFiles->setEnabled( subWindow );
	
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
	const QString file = QFileDialog::getOpenFileName( this, tr( "Open executable to debug" ), ".", QString::null );
	
	if ( !loadTarget( file ) )
	{
		QMessageBox::warning( this, tr( "Warning..." ), tr( "An error occur while loading target: %1" ).arg( mDebugger->lastError() ) );
	}
}

void UIMonkeyDbg::on_aStepInto_triggered()
{
	debuggerStepInto( false );
}

void UIMonkeyDbg::on_aStepOver_triggered()
{
	debuggerStepOver( false );
}
