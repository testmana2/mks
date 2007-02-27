/**************************************************************************
	Main class GNUDebugger (interface with Monkey)
	Ui class GNUDebuggerChild
	SubClass GNUDebuggerPluginManager

Monkey	->	GNUDebugger	(interface)
		|
		-> GNUDebuggerChild	(UI form)
			|
			> GNUDebuggerPluginManager (load / execute plug)

****************************************************************************/
#include <QMessageBox>

#include "GNUDebugger.h"
#include "Workspace.h"
#include "PluginsManager.h"


#include "TabToolBar.h"
#include "MenuBar.h"
#include "StatusBar.h"


#include "GNUDebuggerChild.h"
#include "UISettingsGNUDebugger.h"

#include "ProjectsManager.h"
#include "AbstractProjectProxy.h"

// list of all menu 
// desable when debugger start
// else enable
QStringList menuList = QStringList()	<< "mFile/aExit" \
					<< "mProject/aNew" \
					<< "mProject/aOpen" \
					<< "mProject/mClose/aCurrent" \
					<< "mProject/mClose/aAll"\
					<< "mBuild/mBuild/aCurrent"\
					<< "mBuild/mBuild/aAll"\
					<< "mBuild/mRebuild/aCurrent"\
					<< "mBuild/mRebuild/aAll"\
					<< "mBuild/aStop"\
					<< "mBuild/mClean/aCurrent"\
					<< "mBuild/mClean/aAll"\
					<< "mBuild/aExecute"\
;


// pixmap


enum {PIX_BREAKPOINT, PIX_DEBUGGER_AT};

 
// contructor
GNUDebugger::~GNUDebugger()
{
	if ( isInstalled() )
		uninstall();
}

//
void GNUDebugger::initialize( Workspace* w )
{

	WorkspacePlugin::initialize( w );
	// plugin infos
	mPluginInfos.Caption = tr( "GNU Debugger" );
	mPluginInfos.Description = tr( "This plugin embbeded the GNU Debugger for Monkey by Xiantia" );
	mPluginInfos.Type = BasePlugin::iWorkspace;
	mPluginInfos.Name = "GNU Debugger";
	mPluginInfos.Version = "1.0.3";
	mPluginInfos.Installed = false;

	connect(mWorkspace, SIGNAL(projectOpened(bool)), this, SLOT(onProjectOpened(bool)));
	connect(mWorkspace, SIGNAL(projectName(QString)), this, SLOT(onProjectName(QString)));
	connect(&timer, SIGNAL(timeout ()), this, SLOT(onSetBreakpointOnOpened()));

	breakpoint.load(":/icons/buttoncancel.png");
	breakpoint = breakpoint.scaled(QSize(12,12));
	
	debuggerAt.load(":/icons/play.png");
	debuggerAt = debuggerAt.scaled(QSize(12,12));

}

//

bool GNUDebugger::install()
{
	if ( isInstalled() )
		return true;

	// new instance iof debuggerChild
	mGNUDebugger = UiGNUDebugger::self(mWorkspace);
	connect(mGNUDebugger, SIGNAL(debuggerSignalStarted()), this, SLOT(onDebuggerStarted()));
	connect(mGNUDebugger, SIGNAL(debuggerSignalStoped()), this, SLOT(onDebuggerStopped()));

	// add this on menu View
	mWorkspace->menuBar()->action( "aSeparator3" );
	mWorkspace->menuBar()->action( "mView/aShowDebugger", tr( "Show Debugger" ), QIcon( ":/icons/DebuggerIcon.png" ),tr( "F10" ) );

	// add this on dock
	mWorkspace->tabToolBar()->bar( TabToolBar::Bottom )->appendTab(mGNUDebugger, QPixmap( ":/icons/DebuggerIcon.png" ),mPluginInfos.Caption );

	mPluginInfos.Installed = true;


		
	// connect to debugger bridge
	BasePlugin* bp = mWorkspace->pluginsManager()->plugin( "debugger" );
	if ( bp )
	{
		connect(bp, SIGNAL(signalMarginClicked(MonkeyEditor * , QString,int)), this, SLOT(onMarginClicked(MonkeyEditor *,QString, int)));
		connect(bp, SIGNAL(signalEditorOpened(MonkeyEditor *)), this, SLOT(onEditorOpened(MonkeyEditor *)));

		connect(this, SIGNAL(signalSetMarker(MonkeyEditor*,QPixmap ,int)), bp , SLOT(onSetMarker(MonkeyEditor*,QPixmap , int)));
		connect(this, SIGNAL(signalAddMarker(MonkeyEditor*,int ,int)), bp , SLOT(onAddMarker(MonkeyEditor*,int , int)));
		connect(this, SIGNAL(signalAddMarker(QString,int ,int)), bp , SLOT(onAddMarker(QString,int , int)));
		connect(this, SIGNAL(signalDelMarker(MonkeyEditor*,int ,int)), bp , SLOT(onDelMarker(MonkeyEditor*,int , int)));
		connect(this, SIGNAL(signalDelAllMarker(MonkeyEditor*,int)), bp , SLOT(onDelAllMarker(MonkeyEditor*, int)));
		connect(this, SIGNAL(signalDelAllMarker(QString,int)), bp , SLOT(onDelAllMarker(QString, int)));
		connect(this,SIGNAL(signalGotoLine(QString, int)), bp , SLOT(onGotoLine(QString, int)));	
	
		connect(mGNUDebugger,SIGNAL(debuggerSignalAtLine(QString, int)), this , SLOT(onGotoLine(QString, int)));	

	}

	return true;
}
//
bool GNUDebugger::uninstall()
{
	if ( !isInstalled() )
		return true;

	mWorkspace->tabToolBar()->bar( TabToolBar::Bottom )->removeTab(mGNUDebugger );
	delete mGNUDebugger;
	mPluginInfos.Installed = false;
	return true;
}

QWidget* GNUDebugger::settingsWidget()
{
	return new UISettingsGNUDebugger;
}

// SLOT
// desable some menu
void GNUDebugger::onDebuggerStarted()
{

	for(int i=0; i< menuList.count(); i++)
		mWorkspace->menuBar()->action( menuList.at(i) )->setEnabled( false );

//	mWorkspace->statusBar()->setFileName("Gdb starting");
}

// enable some menu
void GNUDebugger::onDebuggerStopped()
{
	for(int i=0; i< menuList.count(); i++)
		mWorkspace->menuBar()->action( menuList.at(i) )->setEnabled( true );

//	mWorkspace->statusBar()->setFileName("Gdb stopped");
}

// new project opened
void GNUDebugger::onProjectOpened(bool b)
{
	if(b)
	{
		// reading breakpoint in .pro
		settingBreakpoint = mWorkspace->projectsManager()->currentProject()->getValuesList("DEBUGGER_BREAKPOINT");
		// save breakpoint on true structure
		for(int i=0; i<settingBreakpoint.count(); i+=2)
			mGNUDebugger->debuggerToggleBreakpoint(QFileInfo(settingBreakpoint.at(i)).fileName(),settingBreakpoint.at(i+1).toInt());
	}
	else
	{
		// save breakpoint to .pro
//		mWorkspace->projectsManager()->currentProject()->setValuesList("DEBUGGER_BREAKPOINT", settingBreakpoint);
	}

	// notify plugin
	mGNUDebugger->debuggerProjetOpened(b);
}

// set the executable file to debugger
void GNUDebugger::onProjectName(QString name)
{
	mGNUDebugger->debuggerSetProgName(name);
}

// when the debugger is on breakpoint
void GNUDebugger::onGotoLine(QString file, int line)
{

	if(!file.isEmpty())
	{
		qWarning("==================\nDebugger request switch to file :");
		qWarning(file.toAscii());
		mWorkspace->switchToFile(file);

	}
	
	// if file = "" , this signal is for ALL open editor 
	emit signalDelAllMarker(file,PIX_DEBUGGER_AT);
	emit signalAddMarker(file,line-1,PIX_DEBUGGER_AT);

	// jump in line to editor
	emit signalGotoLine(file, line-1);
//	mWorkspace->statusBar()->setFileName("Gdb on breakpoint");
}

// when the new file is opened
void GNUDebugger::onEditorOpened(MonkeyEditor *pMonkeyEditor)
{
	// define marker
	emit signalSetMarker(pMonkeyEditor, breakpoint, PIX_BREAKPOINT);
	emit signalSetMarker(pMonkeyEditor, debuggerAt, PIX_DEBUGGER_AT);
	
	// call onSetBreakpointOnOpened()
	// do not use emit signal !
	timer.setSingleShot(true);
	timer.start(1);
}


// call by timer, add and show all breakpoint to the editor
void GNUDebugger::onSetBreakpointOnOpened()
{
	// reload breakpoint and show this in editor
	for(int i=0; i<settingBreakpoint.count(); i+=2)
		emit signalAddMarker(settingBreakpoint.at(i), settingBreakpoint.at(i+1).toInt()-1, PIX_BREAKPOINT);
}


// modify list of breakpoint for save to .pro
void GNUDebugger::modifySetting(QString file, QList <int> list)
{
	// remove all bp by name file
	while(settingBreakpoint.indexOf(file) != -1) 
	{
		int i = settingBreakpoint.indexOf(file);
		settingBreakpoint.removeAt(i); // remove file name
		settingBreakpoint.removeAt(i); // remove line
	}
	

	for(int j=0; j <list.count(); j++)
	{
		settingBreakpoint.append(file);
		settingBreakpoint.append(QString::number(list.at(j)));
	}

/*	qWarning("= list breakpoint =");
	for(int i=0; i<settingBreakpoint.count(); i++)
		qWarning(settingBreakpoint.at(i).toAscii());
*/}


// toogle breakpoint
void GNUDebugger::onMarginClicked(MonkeyEditor *pMonkeyEditor, QString fileName, int atLine)
{
	if(!mGNUDebugger->isGdbRuning())
	{
		if(mGNUDebugger->debuggerToggleBreakpoint(QFileInfo(fileName).fileName(), atLine+1))
			emit signalAddMarker(pMonkeyEditor, atLine, PIX_BREAKPOINT);
		else
			emit signalDelMarker(pMonkeyEditor, atLine, PIX_BREAKPOINT);
	
		modifySetting(QFileInfo(fileName).fileName(),mGNUDebugger->getBreakpoint(QFileInfo(fileName).fileName()));
	}
	else	QMessageBox::information(0,tr("Information."), tr("Your program running\nI can't add / remove breakpoint now."),QMessageBox::Ok);

	
}


Q_EXPORT_PLUGIN2( WorkspaceGNUDebugger, GNUDebugger )

// end GNUDebugger
