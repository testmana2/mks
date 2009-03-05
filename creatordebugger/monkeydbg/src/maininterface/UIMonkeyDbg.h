#ifndef UIMONKEYDBG_H
#define UIMONKEYDBG_H

#include "ui_UIMonkeyDbg.h"

#include <QMap>

class QMdiSubWindow;
class pEditor;

typedef QPair<QListWidgetItem*, QMdiSubWindow*> ItemSubWindow;

class UIMonkeyDbg : public QMainWindow, public Ui::UIMonkeyDbg
{
	Q_OBJECT

public:
	UIMonkeyDbg( QWidget* parent);
	virtual ~UIMonkeyDbg();

protected:
	QMap<QString, ItemSubWindow> mOpenedFiles;
	
	void initConnections();
	
public slots:
	pEditor* openFile( const QString& fileName );
	void closeCurrentFile();
	void closeAllFiles();
	
protected slots:
	// gui
	void subWindow_destroyed( QObject* object );
	void on_lwFiles_itemActivated( QListWidgetItem* item );
	void on_maWorkspace_subWindowActivated( QMdiSubWindow* subWindow );
	void on_aOpenFile_triggered();
	void on_aLoadTarget_triggered();
};

#endif // UIMONKEYDBG_H
