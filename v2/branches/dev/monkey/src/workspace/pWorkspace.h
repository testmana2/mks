/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pWorkspace.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PWORKSPACE_H
#define PWORKSPACE_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pTabbedWorkspace.h"

class pAbstractChild;
class pAbstractProjectProxy;

class Q_MONKEY_EXPORT pWorkspace : public pTabbedWorkspace, public QSingleton<pWorkspace>
{
	Q_OBJECT
	friend class QSingleton<pWorkspace>;

public:
	pAbstractChild* currentChild() const;
	pAbstractChild* child( int ) const;
	QList<pAbstractChild*> children() const;

	/*
	void openProject( const QString& );
	int addChild( AbstractChild*, const QString& );
	void childCloseEvent( AbstractChild*, QCloseEvent* );
	*/

private:
	pWorkspace( QWidget* = 0 );

private slots:
	void internal_currentChanged( int );
	void internal_aboutToCloseTab( int, QCloseEvent* );

public slots:
	//
	void openFile( const QString&, pAbstractProjectProxy* = 0, const QPoint& = QPoint() );

	// file menu
	void fileOpen_triggered();
	void fileSaveCurrent_triggered();
	void fileSaveAll_triggered();
	void fileCloseCurrent_triggered();
	void fileCloseAll_triggered();
	void fileSaveAsTemplate_triggered();
	void fileQuickPrint_triggered();
	void filePrint_triggered();
	void fileExit_triggered();

	// edit menu
	void editUndo_triggered();
	void editRedo_triggered();
	void editCut_triggered();
	void editCopy_triggered();
	void editPaste_triggered();
	void editSearchReplace_triggered();
	void editGoTo_triggered();

	// view menu

	// project menu
	void projectNew_triggered();
	void projectOpen_triggered();
	void projectSaveCurrent_triggered();
	void projectSaveAll_triggered();
	void projectCloseCurrent_triggered();
	void projectCloseAll_triggered();
	void projectSettings_triggered();

};

#endif // PWORKSPACE_H
