
#include "DockGNUDebugger.h"
#include "pFileManager.h"
#include "ProjectItem.h"
#include "MonkeyCore.h"
#include "pAbstractChild.h"

/* debugger */
//

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>

#include "pMonkeyStudio.h"

QSize pixmapSize(16,16);

DockGNUDebugger::DockGNUDebugger( QWidget* w )
	: pDockWidget( w )
{

	// create tabwidget
/*	// connections
	connect( lwBuildSteps, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( lwBuildSteps_itemPressed( QListWidgetItem* ) ) );
	connect( leRawCommand, SIGNAL( returnPressed() ), this, SLOT( leRawCommand_returnPressed() ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandError( const pCommand&, QProcess::ProcessError ) ), this, SLOT( commandError( const pCommand&, QProcess::ProcessError ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandReadyRead( const pCommand&, const QByteArray& ) ), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStarted( const pCommand& ) ), this, SLOT( commandStarted( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStateChanged( const pCommand&, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( const pCommand&, QProcess::ProcessState ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandSkipped( const pCommand& ) ), this, SLOT( commandSkipped( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SLOT( appendStep( const pConsoleManager::Step& ) ) );
*/

	connect (MonkeyCore::mainWindow(), SIGNAL( aboutToClose()), this , SLOT(onAboutToClose()));


	// create plugin Manager
	mw = new QMainWindow(w);
	setWidget(mw);
	
	setMinimumHeight( 120 );

	// create parser
	Parser = new GdbParser(this);

	kernelDispatcher = new GdbKernelDispatcher(Parser);
	mw->addDockWidget(Qt::RightDockWidgetArea, kernelDispatcher->getContainer());

	// create backtrace
	backtraceGdb = new GdbBackTrace(Parser);
	mw->tabifyDockWidget(kernelDispatcher->getContainer(),backtraceGdb->getContainer());

	// create registers	
	registersGdb = new GdbRegisters(Parser);
	mw->tabifyDockWidget(backtraceGdb->getContainer(),registersGdb->getContainer());

	// create watch
	watchGdb = new GdbWatch(Parser);
	mw->tabifyDockWidget(registersGdb->getContainer(),watchGdb->getContainer());

	// create bridge
	bridgeEditor = new GdbBridgeEditor(Parser);
	mw->tabifyDockWidget(watchGdb->getContainer(), bridgeEditor->getContainer());
//	mw->tabifyDockWidget(registersGdb->getContainer(), bridgeEditor->getContainer());

	// create breakpoint
	breakpointGdb = new  GdbBreakPoint(Parser);
	mw->tabifyDockWidget(bridgeEditor->getContainer(),breakpointGdb->getContainer());

	// create answer
	answerGdb = new  GdbAnswer(Parser);
	mw->tabifyDockWidget(breakpointGdb->getContainer(),answerGdb->getContainer());

	// create control
	controlGdb = new  GdbControl(Parser);
	mw->setCentralWidget(controlGdb->getContainer());

	// add plugin under manager
	kernelDispatcher->add(breakpointGdb);
	kernelDispatcher->add(watchGdb);
	kernelDispatcher->add(registersGdb);
	kernelDispatcher->add(backtraceGdb);
	kernelDispatcher->add(controlGdb);
	kernelDispatcher->add(bridgeEditor);
	kernelDispatcher->add(answerGdb);


	// generic
	connect(Parser, SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
	connect(Parser, SIGNAL(error(int, QString)), this , SLOT(onError(int, QString)));
	connect(Parser, SIGNAL(info(int, QString)), this , SLOT(onInfo(int, QString)));

	// target
	connect(Parser, SIGNAL(targetLoaded(int, QString)), this , SLOT(onTargetLoaded(int, QString)));
	connect(Parser, SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
	connect(Parser, SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
	connect(Parser, SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));

	
	// connect brigde to Qsci
	connect(bridgeEditor, SIGNAL(breakpoint(QByteArray , int, bool)), this , SLOT(onBreakpoint(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(backtrace(QByteArray , int)), this , SLOT(onBacktrace(QByteArray, int)));
	connect(bridgeEditor, SIGNAL(breakpointMoved(QByteArray , int, int)), this , SLOT(onBreakpointMoved(QByteArray, int, int)));
	connect(bridgeEditor, SIGNAL(breakpointConditionnaled(QByteArray , int,bool)), this , SLOT(onBreakpointConditionnaled(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(breakpointEnabled(QByteArray , int,bool)), this , SLOT(onBreakpointEnabled(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(gotoBreakpoint(QByteArray , int)), this , SLOT(onGotoBreakpoint(QByteArray, int)));

	// create QProcess for GDb
	pConsole =  new GdbProcess();
 
	connect(pConsole, SIGNAL( commandStarted( const pCommand& )), this, SLOT(gdbStarted( const pCommand& )));
	connect(pConsole, SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus )), this, SLOT( gdbFinished( const pCommand&, int, QProcess::ExitStatus )));
	connect(pConsole, SIGNAL( commandReadyRead( const pCommand&, const QByteArray& )), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& )));

	// plugin send a command to Gdb
	connect(kernelDispatcher, SIGNAL(sendRawData(GdbCore*, QByteArray)) ,this , SLOT(onSendRawData(GdbCore *, QByteArray) ));

	// start gdb
	Cmd = new pCommand( "gdb", "gdb", QString::null,true, QStringList() << "gdb", QString::null,  false );
	pConsole->addCommand(*Cmd);
	//pConsole->executeProcess();

	// CRLF
	#ifdef Q_OS_WIN 
		crlf = "\r\n";
	#endif
	#ifdef Q_OS_MAC 
		crlf = "\n";
	#endif
	#ifdef Q_OS_UNIX
		crlf = "\n";
	#endif

	// when a file is opened

	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString& ) ), this, SLOT( onFileOpened( const QString& ) ) );
//	connect( MonkeyCore::workspace(), SIGNAL( fileClosed( const QString& ) ), this, SLOT( onFileClosed( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), this, SLOT( onFileClosed( int ) ) );

	connect( controlGdb, SIGNAL(wantExit()), this , SLOT(onWantExit()));
	connect( controlGdb, SIGNAL(wantStart(QString)), this , SLOT(onWantStart(QString)));

}
//
void DockGNUDebugger::onWantStart(QString file)
{
	// start gdb
	pConsole->executeProcess();
	// wait gdb start ....
	pConsole->waitForStarted(2000);
	// and load target
	onSendRawData(NULL,"file " + file.toLocal8Bit());
}
//
void DockGNUDebugger::onWantExit()
{
	// stop gdb
	pConsole->stopCurrentCommand(true);
	
	kernelDispatcher->stopAll();
	kernelDispatcher->setStopProcess();

	// first delete back trace under all editor
	for(int i=0; i<editor.pointeur.count(); i++)
	{
		editor.pointeur.at(i)->markerDeleteAll(iconbacktrace);
		editor.pointeur.at(i)->markerDeleteAll(iconbreakenable);
	}
	hide();
}
// EDITOR
// a new file is open under editor
void DockGNUDebugger::onFileOpened( const QString& file )
{
	if(MonkeyCore::fileManager()->currentChild())
 	{
		// get new file
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			// get name of file
			QString name = MonkeyCore::fileManager()->currentChildFile();

			// save opened editor
			editor.fileName << name;
			editor.pointeur << e;

			// set margin Qsci sensitive
			e->setMarginSensitivity(0,true);
			// connect margin clicked
			connect (e, SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));
	
			// set somes icons on under editor
			iconbreakenable = e->markerDefine (QPixmap(":/icons/break_enable.png").scaled(pixmapSize) );
			iconbreakdisable = e->markerDefine (QPixmap(":/icons/break_disable.png").scaled(pixmapSize));
			iconbacktrace = e->markerDefine (QPixmap(":/icons/play.png").scaled(pixmapSize));
			iconconditionenable = e->markerDefine (QPixmap(":/icons/break_conditional_enable.png").scaled(pixmapSize));
			iconconditiondisable = e->markerDefine (QPixmap(":/icons/break_conditional_disable.png").scaled(pixmapSize));

			// get if this editor haved breakpoint befor close / re-opening
			QGdbMessageCore m;
			m.id=-1;
			m.msg = 	 "^notify,interpreter=\"editor\",currentCmd=\"none\",event=\"requested-breakpoint\",fileName=\"" + name.toLocal8Bit() + "\"";
			kernelDispatcher->process(m);

			// restor backtrace
			if(name.toLocal8Bit() == currentBacktraceFile)
				onBacktrace(name.toLocal8Bit() , currentBacktraceLine);
		}
	}
}
//
void DockGNUDebugger::onFileClosed( int r )
{
	// delete editor
	editor.pointeur.removeAt(r);
	editor.fileName.removeAt(r);
}
//
pEditor *DockGNUDebugger::findFile(QByteArray file)
{
	for(int i=0; i< editor.pointeur.count(); i++)
	{
		QString fullName = editor.fileName.at(i);
		if(QFileInfo(fullName).fileName().toLocal8Bit() == file) return editor.pointeur.at(i);
	}
	return NULL;
}
// user clicked margin
void DockGNUDebugger::onMarginClicked(int marginIndex, int line , Qt::KeyboardModifiers state)
{
	// get the name of file
	QString fileName = MonkeyCore::fileManager()->currentChildFile();

	QGdbMessageCore m;
	m.msg = "^done,interpreter=\"editor\",currentCmd=\"none\",event=\"CTRL+B\",fullname=\"" + fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(line+1) + "\"";
	kernelDispatcher->process(m);

}
// show icon under file
void DockGNUDebugger::onBreakpoint(QByteArray filename, int line, bool b)
{
	if(MonkeyCore::fileManager()->currentChild() )
	{
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			if(b)
			{
				e->markerAdd (line-1, iconbreakenable);
			}
			else
			{
				e->markerDelete(line-1, iconbreakenable);
			}
		}
	}
}
//
// move backtrace under editor
void DockGNUDebugger::onBacktrace(QByteArray filename, int line)
{
	// save current file for restor after closed
	currentBacktraceFile = filename;
	currentBacktraceLine = line;

	// first delete back trace under all editor
	for(int i=0; i<editor.pointeur.count(); i++)
		editor.pointeur.at(i)->markerDeleteAll(iconbacktrace);

	// open file (if is not same)
	MonkeyCore::workspace()->goToLine(filename, QPoint(1,line), true);

	// now the current file is
	pEditor * e = MonkeyCore::fileManager()->currentChild()->currentEditor();

	e->markerAdd (line-1, iconbacktrace);
}
//
// some time gdb move breakpoint under next line 
void DockGNUDebugger::onBreakpointMoved(QByteArray filename, int beforLine, int afterLine)
{
	pEditor *e = findFile(filename);
	if(e)
	{
		e->markerDelete (beforLine-1, iconbreakenable);
		e->markerAdd(afterLine-1, iconbreakenable);
	}
}
//
// show breakpoint enable or not
void DockGNUDebugger::onBreakpointEnabled(QByteArray filename, int line, bool b)
{
	pEditor *e = findFile(filename);
	if(e)
	{
		if(b)
		{
			e->markerDelete (line-1, iconbreakdisable);
			e->markerAdd(line-1, iconbreakenable);
		}
		else
		{
			e->markerDelete (line-1, iconbreakenable);
			e->markerAdd(line-1, iconbreakdisable);
		}
	}
}
//
// goto line at file and at line contain breakpoint
void DockGNUDebugger::onGotoBreakpoint(QByteArray filename, int line)
{
	if(MonkeyCore::workspace() )
	{
		MonkeyCore::workspace()->goToLine(filename, QPoint(1,line), true);
	}
}
//
void DockGNUDebugger::onBreakpointConditionnaled(QByteArray filename, int line, bool b)
{
	// remove breakpoint
//	editor->addBreakpoint("",/*filename,*/ line, false);
//	if(b) editor->addBreakpoint("./src/resources/break_conditionnal_enable.png",/*filename,*/ line, true);
//		else editor->addBreakpoint("./src/resources/break_enable.png",/*filename,*/ line, true);
}



// Console frome pConsoleManager
void DockGNUDebugger::gdbStarted( const pCommand& c)
{
	kernelDispatcher->gdbStarted();
}


void DockGNUDebugger::gdbFinished( const pCommand& c, int a , QProcess::ExitStatus )
{
	kernelDispatcher->gdbFinished();
}

void DockGNUDebugger::commandReadyRead( const pCommand& c , const QByteArray& d)
{
	Parser->processParsing(d);
}


void DockGNUDebugger::onSendRawData(GdbCore *plug, QByteArray data)
{
	Parser->setLastCommand(data);
	pConsole->sendRawData(data + crlf);
}


// From parser
void DockGNUDebugger::onDone(int id, QString st)
{
	QRegExp r("GNU .* (.*)");
	if(r.exactMatch(st))
	{
		QStringList list = r.capturedTexts();

		QString version = list.at(2);
		setWindowTitle(version);
	}
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
}

void DockGNUDebugger::onError(int id, QString st)
{
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
}

void DockGNUDebugger::onInfo(int id, QString st)
{
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
}

// target
void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	kernelDispatcher->targetLoaded();
}

void DockGNUDebugger::onTargetExited(int id, QString st)
{
	kernelDispatcher->targetExited();
}

void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	kernelDispatcher->targetRunning();
}

void DockGNUDebugger::onTargetStopped(int id, QString st)
{
	kernelDispatcher->targetStopped();
}

void DockGNUDebugger::onAboutToClose()
{
	delete Parser;
//	kernelDispatcher->stopAll();
//	kernelDispatcher->setStopProcess();

/*	bridgeEditor->setStopProcess();
	backtraceGdb->setStopProcess();
	registersGdb->setStopProcess();
	watchGdb->setStopProcess();
	breakpointGdb->setStopProcess();
	controlGdb->setStopProcess();
	answerGdb->setStopProcess();	
	
*/	pConsole->stopCurrentCommand(true); // terminate gdb
//	delete pConsole;
//	pConsole->waitForFinished();
}

DockGNUDebugger:: ~DockGNUDebugger()
{

}




