/************************************************************
	GNUDebuggerChild is Ui Form
	By Xiantia
************************************************************/

#include "GNUDebuggerChild.h"
//
#include <QCloseEvent>
#include <QMessageBox>
//
// for menu contextual
#include <QLabel>
#if QT_VERSION >= 0x040200
 #include <QWidgetAction>
#endif
//
QPointer<UiGNUDebugger> UiGNUDebugger::_self = 0L;
//

// ==================== SLOT des Plugin ======================

// Right click on Stacked
void UiGNUDebugger::onPluginMouse(QMouseEvent *e)
{
	if( e->button()  == Qt::RightButton)
		menu->popup(QCursor::pos());
}
//
// On plugin request data from debugger
void UiGNUDebugger::onPluginAction(GNUDebuggerPluginInterface *plug_sender , struct GNUDebuggerPluginStruct *debugger_struct )
{
	// Save plugin
	pluginDebuggerManager.setPluginSender( plug_sender);

//printQstring(plug_sender->pluginName());
//printQstring(debugger_struct->qsCurrentCommand);

	// Plugin send to this or debugger ?
	switch(debugger_struct->iCurrentCommandType)
	{
		case  PLUGIN_TO_GDB:
		{
			if(!debugger_struct->qsCurrentCommand.isEmpty())
			{
				butonContinue->setEnabled(false);
				butonNextStepInto->setEnabled(false);
				butonNextStepOver->setEnabled(false);
				gdbWorking->setText(tr("Gdb working ..."));
				qActionDebuggerStop->setEnabled(false);

				pluginDebuggerManager.copyStruct(debugger_struct);
				
				if(DebuggerDriver)
					DebuggerDriver->setCommand(debugger_struct->qsCurrentCommand);
		//		pluginWatchDog.start(15000);
			}
			break;
		}

		case  PLUGIN_TO_PARENT:
		{
//			qDebug("plugin request emit signal debugger at line");
			QStringList list = debugger_struct->qsCurrentCommand.split(":");
			emit debuggerSignalAtLine(list.at(0), list.at(1).toInt());
			break;
		}
		default :	QMessageBox::critical (this, tr("Error"), tr("PLUGIN   SEND COMMAND IN plugAction unknow "),QMessageBox::Ok,QMessageBox::NoButton);
	}
}	
//
// Select other plugin in menu contextual
void UiGNUDebugger::onPluginMenuAction(QAction * action)
{
	if(pluginDebuggerManager.contains(action->parentWidget()) != -1)
		stackedWidget->setCurrentWidget(action->parentWidget());
}

/*
// NO USE
void UIdebugger::onPluginWatchDog()
{
	QString str = "Debugger frontend have detected time out.\n\nPlugin name : " + qpPlugSender->pluginName() + \
		"\nLast Command : " + plugin_struct.qsCurrentCommand + \
		"\nLast data from Gdb : " + plugin_struct.qsDataFromGdb + \
		"\nSend to : " + ((plugin_struct.iCurrentCommandType == PLUGIN_TO_GDB) ? "Gdb":"Debugger frontend" ) + \
		"\nYour personnal command number : " +  QString::number(plugin_struct.iPluginCommand);

	QMessageBox::critical (this, tr("Plugin TimeOut"),str,QMessageBox::Ok,QMessageBox::NoButton);
	// il y un plug qui merde , stop le debugger correctement
	on_qActionDebugerStop();
}
*/


// ======================== UI_ DEBUGGER ==========================

UiGNUDebugger* UiGNUDebugger::self( QWidget* parent )
{
	if ( !_self )
		_self = new UiGNUDebugger( parent );
	return _self;
}
//

UiGNUDebugger::UiGNUDebugger( QWidget* parent )
	: QDockWidget/*QFrame*/( parent )
{
	qActionDebuggerStart = new QAction(QIcon( ":/Icons/Icons/buttonnext.png" ), tr("Debugger Start"),this);
	qActionDebuggerStart->setObjectName("qActionDebuggerStart");
	qActionDebuggerStop = new QAction(QIcon( ":/Icons/Icons/projectcloseall.png" ),tr("Debugger Stop"),this);
	qActionDebuggerStop->setObjectName("qActionDebuggerStop");
	qActionDebuggerArgs = new QAction(QIcon( ":/Icons/Icons/editclear.png" ),tr("Program arguments"),this);
	qActionDebuggerArgs->setObjectName("qActionDebuggerArgs");
	qActionPluginSetting = new QAction(QIcon( ":/Icons/Icons/editsettings.png" ),tr("Plugin setting"),this);
	qActionPluginSetting->setObjectName("qActionPluginSetting");
	//
	setupUi( this );
 	//
	butonContinue->setEnabled(false);
	butonNextStepInto->setEnabled(false);
	butonNextStepOver->setEnabled(false);
	//
	connect(butonContinue,SIGNAL(clicked()), this, SLOT(ondebuggerContinue()));
	connect(butonNextStepOver,SIGNAL(clicked()),this,SLOT(ondebuggerNextStepOver()));
	connect(butonNextStepInto,SIGNAL(clicked()), this, SLOT(ondebuggerNextStepInto()));
	
	//  GDB driver
	DebuggerDriver = NULL;
	// create menu contextual
	debuggerSetMenuContext();
	// loading plugins
	pluginDebuggerManager.init();
	pluginDebuggerManager.load(this,menu, stackedWidget);	

	BreakpointList.clear();
	
	setMinimumHeight( 150 );
}


// ============= PUBLIC SLOT =============== =========

void UiGNUDebugger::debuggerProjetOpened(bool open)
{
	// un projet est ouvert ?
	pluginDebuggerManager.notifyProjetOpened(open);

	if(open)
	{
		// initialise les plugin
		qActionDebuggerStart->setEnabled(true);
		qActionDebuggerStop->setEnabled(false);
	}
	else
	{
		qActionDebuggerStart->setEnabled(false);
		qActionDebuggerStop->setEnabled(false);
		// force stop debugger si il est en fonction et que l'on 
		// ferme le projet
		debuggerStop();
	}
}


void UiGNUDebugger::debuggerSetProgName(QString progName)
{
	m_progName =progName;
	// fix by P@sNox
	m_progName.replace( "\"", QString::null );
#ifdef Q_OS_WIN32
	if ( !m_progName.endsWith( ".exe", Qt::CaseInsensitive ) )
		m_progName.append( ".exe" );
#endif
}

//=================== PRIVATE FUNCTION ================

void UiGNUDebugger::debuggerStart()
{
	if(!DebuggerDriver)
	{
		if(QFile::exists( m_progName  ))
		{
			pluginDebuggerManager.init();
			currentCommand.clear();

			clearBreakpointIndex();
			
			// lance le debugger
			gdbWorking->setText(tr("Gdb started ..."));

			DebuggerDriver = new GNUDebuggerDriver;
			connect(DebuggerDriver,SIGNAL(gdbDataAvailable(QString)), this ,SLOT(onDataFromGdb(QString)));

			DebuggerDriver->setConfig();

			// fai lui ouvrir l'executable
			DebuggerDriver->setFile(m_progName);
			
			// notify les plug que le debugger est lancer
			pluginDebuggerManager.notifyGdbStarted(true);

			// send les breakpoints qui sont present avant le lancement du debugger			
			setBreakpointOnStart();

			// lance le programme
			DebuggerDriver->setRun(m_progArgs);

			// notify au parent que le debugger est lancer
			emit debuggerSignalStarted();

			// grise les menu start / stop debugger
			qActionDebuggerStart->setEnabled(false);
			qActionDebuggerStop->setEnabled(true);
			mGdbRuning = true;
		}
		else
		{
			QMessageBox::critical (this, tr("Error"), tr("Executable ") + m_progName + tr(" file no found."),QMessageBox::Ok,QMessageBox::NoButton);
			on_qActionDebuggerStop_triggered();
		}
	}
}
//
//
void UiGNUDebugger::debuggerStop()
{
	if(DebuggerDriver)
	{
		disconnect(DebuggerDriver,SIGNAL(gdbDataAvailable(QString)), this ,SLOT(onDataFromGdb(QString)));
	
		DebuggerDriver->setQuit();
		DebuggerDriver=NULL;
	
		butonContinue->setEnabled(false);
		butonNextStepInto->setEnabled(false);
		butonNextStepOver->setEnabled(false);

		gdbWorking->setText(tr("Gdb no started"));
		
		emit debuggerSignalStoped();

		// efface la fleche bleu.
		emit debuggerSignalAtLine("",-1);
	
		// notify all plug Gdb stoped
		pluginDebuggerManager.notifyGdbStarted(false);
		mGdbRuning = false;
	
	}
}
//
// relance le programme et va au prochain breakpoint
void UiGNUDebugger::ondebuggerContinue()
{
	if(DebuggerDriver)
	{
		butonContinue->setEnabled(false);
		butonNextStepInto->setEnabled(false);
		butonNextStepOver->setEnabled(false);

		gdbWorking->setText(tr("Gdb working ..."));
		qActionDebuggerStop->setEnabled(false);

		DebuggerDriver->setContinue();
		// efface la fleche
		emit debuggerSignalAtLine("",-1);
		mGdbRuning = true;
	}
}
//
// Effectu une seule commande et redonne la main
// dans le thread courant
// step over
void UiGNUDebugger::ondebuggerNextStepOver()
{
	if(DebuggerDriver)
	{
		butonContinue->setEnabled(false);
		butonNextStepInto->setEnabled(false);
		butonNextStepOver->setEnabled(false);

		gdbWorking->setText(tr("Gdb working ..."));
		qActionDebuggerStop->setEnabled(false);

		currentCommand = "GET_NEXT_OVER";
		DebuggerDriver->setNextStepOver();
		mGdbRuning = true;
	}
}
//
// step into
void UiGNUDebugger::ondebuggerNextStepInto()
{
	if(DebuggerDriver)
	{
		butonContinue->setEnabled(false);
		butonNextStepInto->setEnabled(false);
		butonNextStepOver->setEnabled(false);

		gdbWorking->setText(tr("Gdb working ..."));
		qActionDebuggerStop->setEnabled(false);

		currentCommand = "GET_NEXT_INTO";
		DebuggerDriver->setNextStepInto();
		mGdbRuning = true;
	}
}


// PARSER
// msg venant de GDB
// fonction mere
// a chaque message de Gdb on appel cette fonction qui reagit
void UiGNUDebugger::onDataFromGdb(QString st)
{

/*
	new info
	info sharedlibrary -> show all .so to load
	set confirm on/off -> no anwser y / n
*/

	/*
	Program termined
	gdb -> Program exited normaly.
	
	Testing on :
	gdb for windows 5.2.1, i686-pc-mingw32 , Windows XP service pack 2 -> status (ok)
	gdb for linux GNU gdb 6.4.90-debian, i486-linux-gnu , kubuntu 6.10 -> status (ok)
	*/

	if( st.contains("Program exited "))
	{
		QMessageBox::information(0,tr("Information."), tr("Program finished."),QMessageBox::Ok);
		on_qActionDebuggerStop_triggered(); // after it call debugerStop();
		mGdbRuning = false;
		return;
	}

	/*
	Program no compiled with debug option
	gdb -> Reading symbols from /home/dev/..... no debugging symbols found ( windows)
	gdb -> No source file named ( kubuntu )
	
	Testing on :
	gdb for windows 5.2.1, i686-pc-mingw32 , Windows XP service pack 2 -> status (ok)
	gdb for linux GNU gdb 6.4.90-debian, i486-linux-gnu , kubuntu 6.10 -> status (  WORK but bug )
	*/

	if( st.contains("no debugging symbols found") || st.contains("No source file named")) 
	{
		on_qActionDebuggerStop_triggered(); // after it call debugerStop();
		QMessageBox::information(this,tr("Information."), tr("Your project is not building in debug mode. ! "),QMessageBox::Ok);
		mGdbRuning = false;
		return;
	}

	/*
	The code source is more recent than program
	gdb -> warning: Source file is more recent than executable

	Testing on :
	gdb for windows 5.2.1, i686-pc-mingw32 , Windows XP service pack 2 -> status (ok)
	gdb for linux GNU gdb 6.4.90-debian, i486-linux-gnu , kubuntu 6.10 -> status (ok)

	*/

	if(st.contains("warning: Source file is more recent than executable"))
	{
		QMessageBox::information(this,tr("Information."), tr("Your source file is more recent than executable ! \nBuild it befort."),QMessageBox::Ok);
		on_qActionDebuggerStop_triggered(); // after it call debugerStop();
		mGdbRuning = false;
		return;
	}
		
	/* 
	Gdb est sur un breakpoint
	gdb -> Breakpoint 1, main (argc=0x3, argv=0xbfe6ef84) at main.cpp:18
	gdb -> Breakpoint 1, boucle (i=0) at src/main.cpp:24

	Testing on :
	gdb for windows 5.2.1, i686-pc-mingw32 (ok)
	gdb for linux GNU gdb 6.4.90-debian, i486-linux-gnu , kubuntu 6.10 -> status (ok)
	*/

	if( st.contains("Breakpoint")  && st.contains("at") && !(st.contains("file")) )
	{
qWarning("on break");
		mGdbRuning = false;
	 	pluginDebuggerManager.setCurrentCommand(DEBUGGER_ON_BREAKPOINT);
	}

	/* 
	Gdb retourn l'index du breakpoint
	Breakpoint 1 at 0x8052a27: file main.cpp, line 18. (gentoo)
	Breakpoint 2 at 0x804a39b: file src/main.cpp, line 26. (kubuntu)

	Testing on :
	gdb for windows 5.2.1, i686-pc-mingw32 (ok)
	gdb for linux GNU gdb 6.4.90-debian, i486-linux-gnu , kubuntu 6.10 -> status (ok)
	*/

	if( (st.contains("Breakpoint")  ) && st.contains("at") && st.contains("file") && st.contains("line"))// && !st.contains("Note"))
	{
		setBreakpointIndex(st);
		//return;
	}
	// l'utilisateur execute un pas suivant
	if(currentCommand ==  "GET_NEXT_OVER" || currentCommand == "GET_NEXT_INTO")
	{
		mGdbRuning = false;
		pluginDebuggerManager.setCurrentCommand(DEBUGGER_STEP_OVER);
	}

	/*
	Gestion des plugins
	*/

	pluginDebuggerManager.execute(st);

	butonContinue->setEnabled(true);
	butonNextStepInto->setEnabled(true);
	butonNextStepOver->setEnabled(true);

	gdbWorking->setText(tr("Gdb waitting you"));
	qActionDebuggerStop->setEnabled(true);
}


void UiGNUDebugger::mousePressEvent ( QMouseEvent * event )
{
	if( event->button()  == Qt::RightButton)
		menu->popup(QCursor::pos());
}


void UiGNUDebugger::closeEvent( QCloseEvent* e )
{
	e->accept();
}

void UiGNUDebugger::on_qActionDebuggerArgs_triggered()
{
//	UIdebugerArgs::self()->exec();
//	m_progArgs = UIdebugerArgs::self()->GetArgs();
	
}

//================== Menu Contextual =====================

void UiGNUDebugger::on_qActionDebuggerStart_triggered()
{
	qActionDebuggerStart->setEnabled(false);
	qActionDebuggerStop->setEnabled(true);
	qActionDebuggerArgs->setEnabled(false);
	debuggerStart();
}
//
void UiGNUDebugger::on_qActionDebuggerStop_triggered()
{
	qActionDebuggerStart->setEnabled(true);
	qActionDebuggerStop->setEnabled(false);
	qActionDebuggerArgs->setEnabled(true);
	debuggerStop();
}
//
void UiGNUDebugger::on_qActionPluginSetting_triggered()
{
	// recherche le plugin courant
	QWidget *w = stackedWidget->currentWidget();
	int i = pluginDebuggerManager.contains(w);
	if(i!=-1)
	{
		// ce plug a t-il une configuration possible ?
		QDialog *d = pluginDebuggerManager.plugin(i)->pluginSetting();
		if(d!=NULL) 
			d->exec();
		else
			QMessageBox::information(0,pluginDebuggerManager.plugin(i)->pluginName(), tr("This plugin don't have a setting. ! "),QMessageBox::Ok);
	}
}
//
void UiGNUDebugger::debuggerSetMenuContext()
{

#if QT_VERSION >= 0x040200

	QLabel *lbRemote;
	QWidgetAction *qwaRemote;
	QLabel *lbPlugin;
	QWidgetAction *qwaPlugin;

	lbRemote = new QLabel("<html><b><i >Remote debugger 1.0.3</i></b></html>");
	lbRemote->setAlignment(Qt::AlignCenter);

	qwaRemote = new QWidgetAction(0);
	qwaRemote->setDefaultWidget(lbRemote);

	lbPlugin = new QLabel("<html><b><i >Plugins</i></b></html>");
	lbPlugin->setAlignment(Qt::AlignCenter);

	qwaPlugin = new QWidgetAction(0);
	qwaPlugin->setDefaultWidget(lbPlugin);

#endif
	
	menu = new QMenu("",0);

	qActionDebuggerStop->setEnabled(false);


#if QT_VERSION >= 0x040200
	menu->addAction(qwaRemote);
#endif
	menu->addSeparator ();
	menu->addAction(qActionDebuggerStart);
	menu->addAction(qActionDebuggerStop);
	menu->addAction(qActionDebuggerArgs);
	menu->addAction(qActionPluginSetting);


	menu->addSeparator ();

#if QT_VERSION >= 0x040200
	menu->addAction(qwaPlugin);
	menu->addSeparator ();
#endif
}

//========================= BREAKPOINT ========================

// Parcour la liste a la recherche du fichier source
// si le fichier exist retourn sont index dans la liste
// sinon -1
int UiGNUDebugger::findFile(QString fileName)
{
	int numberfile = BreakpointList.count();

	if(numberfile )
	{	
		for(int i=0; i<numberfile; i++)
		{
			//modif QFileInfo for ubuntu V1.0.2
			if(QFileInfo(BreakpointList[i].File_name).fileName() == QFileInfo(fileName).fileName()) return i;
		}
	}
//qDebug("return -1");
	return -1;
}


// le breakpoint est memoris par GDB
// il retourne sont index
void UiGNUDebugger::setBreakpointIndex(QString st)
{
// Breakpoint 1 at 0x8052a27: file main.cpp, line 18.
// recupere le 1
//
// or 
//
// if breakpoint allready set
// Note: breakpoint 2 also set at pc 0x804aa84.
// Breakpoint 3 at 0x804aa84: file src/main.cpp, line 11.

int indexLine;
	QList <QString> line = st.split("\n");

	if( (st.contains("Note")  ) && st.contains("also set at"))
		indexLine = 1;
	else 
		indexLine = 0;	

	
//qDebug(st.toAscii());
	QList <QString> list = line.at(indexLine).split(" ");

	QString file = list[5]; // non du fichier
	file.remove(",");	
//qDebug(file.toAscii());
	QString numberLine = list[7]; // ligne du fichier
	numberLine.remove(".");

	QString index = list[1]; // index du breakpoint suivant GDB
//qDebug(QString("online : " + numberLine + " index : " + index).toAscii());

	int i = findFile(file);		// recherche le fichier dans la liste
	if(i !=-1)
	{
//		qDebug("ok index");

		int j = BreakpointList[i].Bp_list.indexOf( numberLine.toInt() );
		if(j != -1)
			BreakpointList[i].Bp_index <<  index.toInt() ;
	}
}


void UiGNUDebugger::clearBreakpointIndex()
{
int numberfile = BreakpointList.count();

	for(int i =0; i<numberfile; i++)
	{
		BreakpointList[i].Bp_index.clear();
	}		

}

// initialise les breakpoint qui ont etaient fait
// avant le lancement de GDB
void UiGNUDebugger::setBreakpointOnStart()
{
int numberfile = BreakpointList.count();

	for(int i =0; i<numberfile; i++)
	{
		int numberlist = BreakpointList[i].Bp_list.count();

		for(int j =0; j<numberlist ;j++)
		{	if(DebuggerDriver) 
				DebuggerDriver->setBreakpoint("b " + QFileInfo(BreakpointList[i].File_name).fileName() + ":" + QString::number(BreakpointList[i].Bp_list[j]));
//			qDebug(QString("b " + BreakpointList[i].File_name + ":" + QString::number(BreakpointList[i].Bp_list[j])).toAscii());
		}
	}		
}
 


int UiGNUDebugger::debuggerToggleBreakpoint(QString fileName, int bp_line)
{
struct sfile mBreakpointList;

	mBreakpointList.File_name  = fileName;
	mBreakpointList.Bp_list << bp_line;
	
	int numberFile = BreakpointList.count();

	// liste deja initialise ?
	if(numberFile)// != -1)
	{	
		// oui
		// la liste contient deja le fichier ?
		int i = findFile(fileName);
//qDebug(fileName.toAscii());
		if(i != -1)
		{
//qDebug("fichier trouvé :)");
			// oui
			// si le breakpoint exit , alors supprime le
			if(  BreakpointList[i].Bp_list.contains(bp_line))
			{
//qDebug("supprime");
				int j = BreakpointList[i].Bp_list.indexOf(bp_line);
				BreakpointList[i].Bp_list.removeAt(j);
				// on verif que la liste n'est pas vide			
				// cas d'ajout puis suppression d'un bp avant le lancement de GDB
				// command "file /home/...."
				// bug fix				
				if(BreakpointList[i].Bp_index.count())
				{
					QString Breakpoint_at= QString("delete ") +QString::number(BreakpointList[i].Bp_index[j]) ;
//qDebug(Breakpoint_at.toAscii());	
					if(DebuggerDriver) 
						DebuggerDriver->setBreakpoint( Breakpoint_at);
//					if(BreakpointList[i].Bp_index.count() <= j)
					BreakpointList[i].Bp_index.removeAt(j);
				}	
				return 0;
//				breakpointSup( fileName ,QString::number(bp_line));
			}
			else
			{
//qDebug("ajoute");
				// sinon ajoute
				QString breakpoint_at = "b " + fileName + ":" + QString::number(bp_line) ;
				if(DebuggerDriver) DebuggerDriver->setBreakpoint(breakpoint_at);
				BreakpointList[i].Bp_list << bp_line;
//				BreakpointList[i].Bp_index << -1;
//				breakpointAdd(fileName,QString::number(bp_line));
				return 1;
			}
		}			
		else
		{
			// la liste est initialis�
			// mais ne contient pas ce fichier source
//qDebug("fichier non trouvé -> ajoute le ");
			BreakpointList << mBreakpointList; 
			QString breakpoint_at = QString("b ") + fileName + QString(":") + QString::number(bp_line);
			if(DebuggerDriver) DebuggerDriver->setBreakpoint(breakpoint_at);		
//			breakpointAdd(fileName,QString::number(bp_line));
			return 1;
		}
	}
	else
	// premier de la list
	{
//qDebug("List vierge");
		BreakpointList << mBreakpointList; 
		QString breakpoint_at = QString("b ") + fileName + QString(":") + QString::number(bp_line) ;
		if(DebuggerDriver) DebuggerDriver->setBreakpoint(breakpoint_at);		
//		breakpointAdd(fileName,QString::number(bp_line));
		return 1;
	}

}

QList<int> UiGNUDebugger::getBreakpoint(QString file)
{
	int index = findFile(file);
	return BreakpointList.at(index).Bp_list;
}
