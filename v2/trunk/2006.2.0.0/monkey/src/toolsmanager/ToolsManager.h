#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H
//
#include <QPointer>
//
#include "MonkeyExport.h"
//
class Settings;
class MenuBar;
class QAction;
//
class Q_MONKEY_EXPORT ToolsManager : public QObject
{
	Q_OBJECT
	//
public:
	static ToolsManager* self( QObject* = 0 );
	//
private:
	ToolsManager( QObject* = 0 );
	Settings* settings() const;
	MenuBar* menuBar() const;
	void initialize();
	//
	static QPointer<ToolsManager> mSelf;
	//
public slots:
	void toolsMenu_triggered( QAction* );
	//
};
//
#endif // TOOLSMANAGER_H
