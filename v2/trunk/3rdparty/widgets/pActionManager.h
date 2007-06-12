#ifndef PACTIONMANAGER_H
#define PACTIONMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QApplication>

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

private:
	QList <pActionGroup*> actionGroups;

	QSettings* mSettings;
	pAction* selectedAction;
	pKeySequenceInput* kinput;
	QTreeWidget* list;
	QPushButton* defaultbtn;
	QPushButton* clearbtn;
	QPushButton* setbtn;

	//returns QString("OK") if all OK, and name of shortcut, with it are conflict, if are conflict
	QString setShortcutForAction( pAction*, const QKeySequence& );

public:
	pActionManager( QObject* = QApplication::instance() );

	void setSettings( QSettings*, bool = true );
	QSettings* settings() const;
	void reloadSettings();

	void addGroup( pActionGroup* );
	void removeGroup( pActionGroup* );

	static QKeySequence getShortCut( pAction* );
	QDialog* shotcutsConfig( QWidget* = QApplication::activeWindow() );

public slots:
	void showSettings();

private slots:
	void shortcutSelected();
	void setDefaultClicked();
	void changeShortcut();
	void shortcutEdited();

};

#endif // PACTIONMANAGER_H
