/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 01h15
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : MenuBar.h
 * LICENSE      : GPL
 * COMMENTARY   : This class is the monkey studio menuBar created using the pMenuBar class
 ********************************************************************************************************/
#ifndef MenuBar_H
#define MenuBar_H
//
#include "pMenuBar.h"
//
#include <QList>
#include <QAction>
//
class MenuBar : public pMenuBar
{
	Q_OBJECT
	//
public:
	static MenuBar* self( QWidget* = 0 );
	//
	void initialize();
	//
private:
	MenuBar( QWidget* = 0 );
	//
	void initializeStyles();
	//
	static QPointer<MenuBar> mSelf;
	QList<QAction*> mRecentFiles;
	QList<QAction*> mRecentProjects;
	//
public slots:
	void recentFiles_triggered( QAction* );
	void updateRecentFiles();
	void addRecentFile( const QString& );
	void agStyles_triggered( QAction* );
	void recentProjects_triggered( QAction* );
	void updateRecentProjects();
	void addRecentProject( const QString& );
	void aboutApplication_triggered();
	void aboutQt_triggered();
	//
signals:
	void openFileRequested( const QString& );
	void openProjectRequested( const QString& );
	//
};
//
#endif // MenuBar_H
