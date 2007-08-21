/*

author : Xiantia
date : 02/09/06

class name UIdebugger
*/
	
	/* 
	A pour effet de griser les menu start et stop du menu contextuel.
	De stopper le debugger si il etait en fonction a la fermeture du projet
	De notify a tout les plugin que le projet est ouvert / ferm�
	*/
	
	//	debuggerProjetOpened(true / false);

	/* 
	Permet d'ajouter / supprimer un  breakpoint dans le debugger
	*/

	//	void debuggerToggleBreakpoint(QString Filname, int atLine);

	/*
	Donne au debugger le nom du programme
	*/

	//	debugerSetProgName(QString ProgName); // remplace debugerStart()

	/* 
	signal emit
	*/

	//	debuggerSignalAtLine(QString file, int numLine);
	//	debuggerSignalStoped();
	//	debuggerSignalStarted();


#ifndef UIDEBUGER_H
#define UIDEBUGER_H
//
#include "ui_UiGNUDebugger.h"
#include "GNUDebuggerDriver.h"
#include "GNUDebuggerPluginManager.h"

//
#include <QTimer>
#include <QPointer>
#include <QFile>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QWidget>
#include <QTableWidget>



// =============== CLASS UI_DEBUGGER =============

class UiGNUDebugger : public QDockWidget/*QFrame*/, public Ui::UiGNUDebugger
{
	Q_OBJECT
	//
public :
	static UiGNUDebugger* self( QWidget* = 0 );
	//
	// enable / desable debuger menu contextuel start/stop
	void debuggerProjetOpened(bool open);
	//
	// enregistre le chemin de l'executable
	void debuggerSetProgName(QString progName);
	//
	// toggle breakpoint
	int debuggerToggleBreakpoint(QString, int);
	//
	bool isGdbRuning() {return mGdbRuning;}

	void clearBreakpointIndex();

	QList<int> getBreakpoint(QString file);	

protected:
	void closeEvent( QCloseEvent* );

private:
	UiGNUDebugger( QWidget* = 0 );
	static QPointer<UiGNUDebugger> _self;

	// PLUGIN
	class GNUDebuggerPluginManager pluginDebuggerManager;
	//
	// start / stop debugger
	void debuggerStart();
	void debuggerStop();

	// BREAKPOINT
	struct sfile
	{
		QString File_name;
		QList <int> Bp_list;
		QList <int> Bp_index;
	};

	// list de breakpoint
	int findFile(QString file);
	QList <sfile> BreakpointList;

//	void breakpointInit();
	void setBreakpointOnStart();
	void setBreakpointIndex(QString st);
	
	// MENU
	QMenu *menu;
	QAction * qActionDebuggerStart;
	QAction * qActionDebuggerStop;
	QAction * qActionDebuggerArgs;
	QAction * qActionPluginSetting;
	void debuggerSetMenuContext();

	QString currentCommand;
	QString m_progName;		// nom du programme a lancer
	QString m_progArgs;		// list des argument du programme

	//  pointeur de gdb	
	GNUDebuggerDriver *DebuggerDriver;
	bool mGdbRuning;

public slots:
	void mousePressEvent ( QMouseEvent * event ) ;
	// on click sur un plugin -> ouverture du menu contextuel
	void onPluginMouse(QMouseEvent *);
	// un plugin demande des infos a gdb 
	void onPluginAction(GNUDebuggerPluginInterface * , GNUDebuggerPluginStruct *);
	// on a selectionn�un plug a afficher dans le stacked
	void onPluginMenuAction(QAction * action);
	
	// action du menu par defaut
	void on_qActionDebuggerStart_triggered();
	void on_qActionDebuggerStop_triggered();
	void on_qActionDebuggerArgs_triggered();
	void on_qActionPluginSetting_triggered();

	// action du debugger par defaut
	void ondebuggerNextStepOver();
	void ondebuggerNextStepInto();
	void ondebuggerContinue();

	// message en provenance de gdb
	void onDataFromGdb(QString st);

	// watch dog des plugin
//	void onPluginWatchDog();

signals:
	// le debugger est sur un breakpoint, donc notify le 
	void debuggerSignalAtLine(QString, int);
	// le debugger est arret�
	void debuggerSignalStoped();
	// le debugger est lanc�
	void debuggerSignalStarted();
};
//
#endif // UIDEBUGER_H
