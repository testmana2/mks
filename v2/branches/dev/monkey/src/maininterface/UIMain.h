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

#include <QAction>
#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pMainWindow.h"

class QActionGroup;
class pFileManager;
class pWorkspace;

//public accessible list of actions.
struct kernelActions_s
{
	//file menu
	QAction *aFileNew,	*aFileOpen, *aFileRecentsClear, *aFileSessionSave, *aFileSessionRestore, *aFileSaveCurrent, *aFileSaveAll, *aFileCloseCurrent, *aFileCloseAll, *aFileQuickPrint, *aFilePrint, *aFileQuit;
	//edit menu
	QAction *aEditSettings, *aEditShortcutsEditor, *aEditUndo, *aEditRedo, *aEditCopy, *aEditCut, *aEditPaste, *aEditSearchReplace, *aEditSearchNext, *aEditSearchPrevious, *aEditGoTo, *aEditExpandAbbreviation;
	//view menu
	QAction *aViewNext, *aViewPrevious;
	//project menu
	QAction *aProjectNew, *aProjectOpen, *aProjectSaveCurrent, *aProjectSaveAll, *aProjectCloseCurrent, *aProjectCloseAll, *aProjectSettings, *aProjectRecentsClear;
	//tools menu
	QAction *aToolsEdit;
	//plugins menu
	QAction *aPluginsManage;
	//help menu
	QAction *aHelpManual, *aHelpAbout, *aHelpAboutQt, *aHelpTestReport;
} ;


class Q_MONKEY_EXPORT kernelActionsContainer : public QObject, public QSingleton<kernelActionsContainer>
{
public:
	struct kernelActions_s kernelActions;
};


class Q_MONKEY_EXPORT UIMain : public pMainWindow, public QSingleton<UIMain>
{
	Q_OBJECT
	friend class QSingleton<UIMain>;

public:
	pFileManager* fileManager();
	pWorkspace* workspace();

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
