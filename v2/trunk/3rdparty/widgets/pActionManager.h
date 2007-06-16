#ifndef PACTIONMANAGER_H
#define PACTIONMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

class pActionGroup;
class pAction;
class pKeySequenceInput;
class QTreeWidget;
class QPushButton;
class QSettings;

class Q_MONKEY_EXPORT pActionManager : public QObject, public QSingleton<pActionManager>
{
	Q_OBJECT
	friend class QSingleton<pActionManager>;
	friend class pShortcutsEditor;

public:
	pActionManager( QObject* = QApplication::instance() );

	void setSettings( QSettings*, bool = true );
	QSettings* settings() const;
	void reloadSettings();

	void addGroup( pActionGroup* );
	void removeGroup( pActionGroup* );

	static QKeySequence getShortCut( const QString&, const QString&, const QKeySequence& );

private:
	// return OK or the conflicting action name and shortcut
	QString setShortcutForAction( pAction*, const QKeySequence& );

	// internal functions for access to QSettings
	// FIXME: P@sNox: hlamer what is this for ?!
	QKeySequence readFromSettings( const QString& );
	void writeToSettings( const QString&, const QKeySequence& );

	QList <pActionGroup*> actionGroups;
	QSettings* mSettings;

public slots:
	void showSettings();

};

#endif // PACTIONMANAGER_H
