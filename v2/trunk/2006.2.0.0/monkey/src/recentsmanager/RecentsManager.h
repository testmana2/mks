#ifndef RECENTSMANAGER_H
#define RECENTSMANAGER_H
//
#include <QPointer>
//
#include "MonkeyExport.h"
//
class Settings;
class MenuBar;
class QAction;
//
class Q_MONKEY_EXPORT RecentsManager : public QObject
{
	Q_OBJECT
	//
public:
	static RecentsManager* self( QObject* = 0 );
	//
	int maxRecentFiles() const;
	int maxRecentProjects() const;
	//
private:
	RecentsManager( QObject* = 0 );
	Settings* settings() const;
	MenuBar* menuBar() const;
	void initialize();
	//
	static QPointer<RecentsManager> mSelf;
	QList<QAction*> mRecentFiles;
	QList<QAction*> mRecentProjects;
	//
public slots:
	void setMaxRecentFiles( int );
	void setMaxRecentProjects( int );
	//
	void recentFiles_triggered( QAction* );
	void updateRecentFiles();
	void addRecentFile( const QString& );
	//
	void recentProjects_triggered( QAction* );
	void updateRecentProjects();
	void addRecentProject( const QString& );
	//
signals:
	void openFileRequested( const QString& );
	void openProjectRequested( const QString& );
	//
};
//
#endif // RECENTSMANAGER_H
