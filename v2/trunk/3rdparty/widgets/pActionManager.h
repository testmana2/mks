#ifndef PACTIONMANAGER_H
#define PACTIONMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QHash>

class QAction;
class pKeySequenceInput;
class QTreeWidget;
class QPushButton;
class QSettings;

typedef QList<QAction*> pActionList;
typedef QHash<QString, pActionList> pHashActionList;

class Q_MONKEY_EXPORT pActionManager : public QObject, public QSingleton<pActionManager>
{
	Q_OBJECT
	friend class QSingleton<pActionManager>;
	friend class pShortcutsEditor;

public:
	static void setSettings( QSettings*, bool = true );
	static QSettings* settings();
	static void reloadSettings();

	static QAction* addAction( const QString&, QAction* );
	static pHashActionList actions();

	static QString globalGroup();

	static QKeySequence getShortcut( const QString&, QAction*, const QKeySequence& );
	static bool setShortcut( QAction*, const QKeySequence& );

	static QString lastError();

private:
	pActionManager( QObject* = QApplication::instance() );

	// internal functions for access to QSettings
	// FIXME: P@sNox: hlamer what is this for ?!
	QKeySequence readFromSettings( const QString& );
	void writeToSettings( const QString&, const QKeySequence& );

	QSettings* mSettings;
	pHashActionList mActions;
	QString mError;

public slots:
	void showSettings();

};

#endif // PACTIONMANAGER_H
