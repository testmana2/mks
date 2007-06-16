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

private:
	QList <pActionGroup*> actionGroups;

	QSettings* mSettings;

	//returns QString("OK") if all OK, and name of shortcut, with it are conflict, if are conflict
	QString setShortcutForAction( pAction*, const QKeySequence& );

	//internal functions for access to QSettings
	QKeySequence readFromSettings( const QString&);
	void writeToSettings( const QString&, const QKeySequence& );

public:
	pActionManager( QObject* = QApplication::instance() );

	void setSettings( QSettings*, bool = true );
	QSettings* settings() const;
	void reloadSettings();

	void addGroup( pActionGroup* );
	void removeGroup( pActionGroup* );

	static QKeySequence getShortCut( const QString&, const QString&, const QKeySequence& );

public slots:
	void showSettings();

};

#endif // PACTIONMANAGER_H
