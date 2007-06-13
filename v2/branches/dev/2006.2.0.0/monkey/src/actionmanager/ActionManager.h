#include <QObject>
#include <QString>
#include <QSettings>

#include <QKeySequence>
#include <QTreeWidget>
#include <QPushButton>


#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "KeySequenceInput.h"

/*
ActionManager is system of 4 classes:  
	MonkeyAction (.h, .cpp), 
	MonkeyActGroup (.h, .cpp), 
	KeySequenceInput (.h, .cpp), 
	ActionManager (.h, .cpp), 
It's provides support easy for use action shortcuts configuring system. 
See comments for a classes.

ActionManager class is kernel. it's provides configuration of shortcuts, 
and saving/reading settins.

Author - Kopats Andrei. 
hlamer at tut dot by
Writed for the Monkey Developer Studio project
(www.monkeystudio.org)
Date: 14/06/2007
License: GPL
*/

class ActionManager : public QObject
{
Q_OBJECT
private:

	QList <MonkeyActGroup*> actionGroups;

	static ActionManager* mSelf;	
    
    MonkeyAction* selectedAction;
    KeySequenceInput* keyInput;
    QTreeWidget * actListWidget;
    QPushButton* defaultBtn;
    QPushButton* clearBtn;
    QPushButton* setBtn;
    
    static QSettings*  settings;
     
     //returns QString("OK") if all OK, and name of shortcut, with it are conflict, if are conflict
    QString setShortcutForAction ( MonkeyAction* action, QKeySequence shortcut);

	//internal functions for access to QSettings
	QKeySequence readFromSettings (QString name);
	void writeToSettings (QString name, QKeySequence seq);
    
public:
	static ActionManager* self ();
    
	void addActGroup ( MonkeyActGroup* grp);
	void deleteActGroup ( MonkeyActGroup* grp);
    
	//This method calling by MonkeyAction for getting shortcut for self
	static QKeySequence getShortCut ( QString groupName, QString actionName, QKeySequence defaultShortcut);
	//set QSettings, that ActionManager will use
	//if needToReload  == true, additionaly reloadSettings will be called
    void setSettings ( QSettings* newset, bool needToReload = true);
	//get QSettings, which ActionManager currently using.
    QSettings* getSettings ();
	//reread from a current QSettings all shortcuts and set it for a actions
	void reloadSettings ();

private slots:
	//here is events from a buttons of configuring dialog and from the KeySequenceInput
    void shortcutSelected ();
    void setDefaultClicked ();
    void changeShortcut ();
    void shortcutEdited ();	

public slots:
	//will create and display shortcuts config widget.
    void showSettings ();
};
