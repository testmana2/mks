#ifndef PTOOLSMANAGER_H
#define PTOOLSMANAGER_H

#include "MonkeyExport.h"

#include <QFileIconProvider>

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

class Q_MONKEY_EXPORT pToolsManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	enum ToolType { ttAll = -1, ttUserEntry, ttDesktopEntry };

	static const QFileIconProvider* iconProvider();
	static const QList<pTool> tools( ToolType = pToolsManager::ttAll );
	static void initializeTools();

protected:
	static QFileIconProvider* mIconProvider;

	pToolsManager( QObject* = 0 );
	~pToolsManager();

public slots:
	void editTools_triggered();
	void toolsMenu_triggered( QAction* );

};

#endif // PTOOLSMANAGER_H
