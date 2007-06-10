#include <QObject>
#include <QAction>
#include <QActionGroup>
#include <QString>
#include <QDialog>
#include <QKeySequence>
#include <QTreeWidget>

#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "KeySequenceInput.h"

class ActionManager : public QObject
{
Q_OBJECT
private:

	QList <MonkeyActGroup*> actionGroups;

	static ActionManager* mSelf;	
    
    MonkeyAction* selectedAction;
    KeySequenceInput* kinput;
    QTreeWidget * list;
    QPushButton* defaultbtn;
    QPushButton* clearbtn;
     QPushButton* setbtn;

    //returns QString("OK") if all OK, and name of shortcut, with it are conflict, if are conflict
    QString setShortcutForAction ( MonkeyAction* action, QKeySequence shortcut);
    
public:
	static ActionManager* self ();
    
    ActionManager ();
	void addActGroup ( MonkeyActGroup* grp);
	void deleteActGroup ( MonkeyActGroup* grp);
    
	static QKeySequence getShortCut ( MonkeyAction* );
	QDialog* shotcutsConfig ();
    
public slots:
    void showSettings ();
    void shortcutSelected ();
    void setDefaultClicked ();
    void changeShortcut ();
    void shortcutEdited ();
};
