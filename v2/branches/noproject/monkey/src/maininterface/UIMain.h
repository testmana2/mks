/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef UIMAIN_H
#define UIMAIN_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pMainWindow.h"

class QActionGroup;
class pFileManager;
class pWorkspace;
class PluginsManager;
class UIProjectsManager;

class Q_MONKEY_EXPORT UIMain : public pMainWindow, public QSingleton<UIMain>
{
	Q_OBJECT
	friend class QSingleton<UIMain>;

public:
	PluginsManager* pluginsManager();
	pFileManager* fileManager();
	pWorkspace* workspace();
	UIProjectsManager* projectsManager();

	//Pointers for some actions stored fore more quick access to it
	struct {
		QAction* build;
		QAction* clean;
		QAction* reBuild;
		QAction* distClean;
		QAction* execute;
		QAction* buildExecute;
	} buildActions;

protected:
	void closeEvent( QCloseEvent* );

private:
	UIMain( QWidget* = 0 );
	void initMenuBar();
	void initToolBar();
	void initConnections();
	void initGui();

	QActionGroup* agStyles;

};

#endif // UIMAIN_H
