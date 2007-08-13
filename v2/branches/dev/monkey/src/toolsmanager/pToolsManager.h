#ifndef PTOOLSMANAGER_H
#define PTOOLSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QApplication>

class pSettings;
class pMenuBar;
class QAction;
	
struct Q_MONKEY_EXPORT pTool
{
	pTool( const QString& c, const QString& i, const QString& f, const QString& p, bool b = false )
	:	Caption( c ), FileIcon( i ), FilePath( f ), WorkingPath( p ), DesktopEntry( b ) {}

	QString Caption;
	QString FileIcon;
	QString FilePath;
	QString WorkingPath;
	bool DesktopEntry;
};

class Q_MONKEY_EXPORT pToolsManager : public QObject, public QSingleton<pToolsManager>
{
	Q_OBJECT
	friend class QSingleton<pToolsManager>;

public:
	enum ToolType { ttAll = -1, ttUserEntry, ttDesktopEntry };

	static const QList<pTool> tools( ToolType = pToolsManager::ttAll );
	static void initializeTools();
	
private:
	pToolsManager( QObject* = QApplication::instance() );

public slots:
	void toolsMenu_triggered( QAction* );

};

#endif // PTOOLSMANAGER_H
