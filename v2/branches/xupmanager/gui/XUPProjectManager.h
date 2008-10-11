#ifndef XUPPROJECTMANAGER_H
#define XUPPROJECTMANAGER_H

#include "ui_XUPProjectManager.h"

#include <QMap>

class XUPProjectModel;
class XUPProjectItem;
class QPlainTextEdit;

class XUPProjectManager : public QWidget, public Ui::XUPProjectManager
{
	Q_OBJECT

public:
	enum ActionType { atOpen = 0, atClose };
	
	XUPProjectManager( QWidget* parent = 0 );
	virtual ~XUPProjectManager();
	
	QAction* action( XUPProjectManager::ActionType type );
	XUPProjectModel* currentProject() const;

protected:
	QMap<XUPProjectManager::ActionType, QAction*> mActions;
	QMap<QString, QPlainTextEdit*> mOpenedFiles;
	QMenu* mDebugMenu;

public slots:
	void addError( const QString& error );
	bool openFile( const QString& fileName, const QString& encoding = "UTF-8" );
	void openProject();
	void closeProject();
	void setCurrentProject( XUPProjectModel* project );

protected slots:
	void fileClosed( QObject* object );
	void on_cbProjects_currentIndexChanged( int id );
	void on_tbDebug_triggered( QAction* action );
	void on_tvCurrentProject_activated( const QModelIndex& index );
};

#endif // XUPPROJECTMANAGER_H
