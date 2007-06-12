#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H
#include <QDebug>
#include <QDialog>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "ActionManager.h"
#include "Settings.h"
#include "Workspace.h"
#include "MenuBar.h"
#include "KeySequenceInput.h"

ActionManager* ActionManager::mSelf = 0L;
QSettings* ActionManager::settings;

ActionManager* ActionManager::self()
{
	if ( !mSelf )
		mSelf = new ActionManager();
	return mSelf;
}

ActionManager::ActionManager ()
{
    //Can't use MonkeyAction there, because MonkeyAction use ActionManager. It's reqursion
    QMenu* menu = Workspace::self()->menuBar()->menu ("Tools");
    QAction * act = new QAction ("Shortcut settings", this);
    menu->addAction ( act);
    connect ( act, SIGNAL ( triggered()), this, SLOT ( showSettings ()));
}

void ActionManager::addActGroup ( MonkeyActGroup* grp)
{
	actionGroups.append ( grp);
}

void ActionManager::deleteActGroup ( MonkeyActGroup* grp)
{
	actionGroups.removeAll (grp);
}

void ActionManager::shortcutSelected ()
{
  QTreeWidgetItem * item = list->selectedItems()[0]; //do not need select more then one :D
 if ( not item->parent() )
  {
    defaultbtn->setEnabled (false);
    clearbtn->setEnabled (false);
    return; //Group header selected
   }
 defaultbtn->setEnabled (true);
 clearbtn->setEnabled (true);
 QString groupText = item->parent()->text(0);
  MonkeyActGroup* group;
 QTreeWidget* tree = item->treeWidget();
 for ( int i = 0; i< actionGroups.size(); i++)
  if (  actionGroups[i]->property("text").toString() == groupText)
  {
   group = actionGroups[i];
   break;
  }
  //now maybe group is finded :D
 for ( int i = 0; i< group->actions().size(); i++)
  if ( group->actions()[i]->text() == item->text(0)) //
  {
   selectedAction = (MonkeyAction*)group->actions()[i];
	 break;
  }
 if ( not selectedAction)
  qDebug ("What the fuck!");
 kinput->setEnabled (true);
 kinput->setText (selectedAction->shortcut().toString());
   setbtn->setEnabled ( false);
}

void ActionManager::showSettings ()
{
    selectedAction = NULL;
    QDialog wgt (NULL,Qt::Dialog);
    wgt.setFixedSize ( 400, 480);
    QVBoxLayout vbox ( &wgt);
    list = new QTreeWidget ( &wgt);
    list->setColumnCount (2);
    list->header()->hide();
    list->header()->setResizeMode(QHeaderView::ResizeToContents);
    MonkeyActGroup* actgrp;
    QString grpName;
    QList<QAction*> actions;
    for ( int grpn = 0; grpn< actionGroups.size (); grpn++ )
	{
        QTreeWidgetItem* grpitem = new QTreeWidgetItem (list, QStringList (actionGroups[grpn]->property("text").toString()));
        grpitem->setExpanded ( true);
		actions = actionGroups[grpn]->actions();
		for (int actn = 0; actn < actions.size(); actn ++)
        {
            QTreeWidgetItem* newItem = new QTreeWidgetItem ( grpitem, QStringList ()<<( grpName+ actions[actn]->text())<<(actions[actn]->shortcut().toString()));
            newItem->setIcon ( 0, actions[actn]->icon());
	}   }
    
    vbox.addWidget ( list);
    //actions[actn]->property("Default shortcut").toString()
    QHBoxLayout confbox (&wgt);
    vbox.addLayout (&confbox);
    defaultbtn = new QPushButton ("Restore default", &wgt);
    defaultbtn->setEnabled (false);
    confbox.addWidget ( defaultbtn);
    clearbtn = new QPushButton ("Clear", &wgt);
    clearbtn->setEnabled ( false);
    confbox.addWidget ( clearbtn);
    kinput = new KeySequenceInput (&wgt);
    kinput->setEnabled (false);
    confbox.addWidget ( kinput);
    setbtn = new QPushButton ("Set", &wgt);
     setbtn->setEnabled (false);
    confbox.addWidget ( setbtn);
    QHBoxLayout hbox (&wgt);
    vbox.addLayout ( &hbox);
//     hbox.addSpacing (290);
    QPushButton closebtn ("Close", &wgt);
    hbox.addWidget ( &closebtn);
    connect ( &closebtn, SIGNAL (clicked()), &wgt, SLOT (close()) );
    connect ( clearbtn, SIGNAL (clicked()), kinput, SLOT (clear()) );
    connect ( defaultbtn, SIGNAL (clicked()), this, SLOT (setDefaultClicked()) );
    connect ( list, SIGNAL (  itemSelectionChanged()), this, SLOT (shortcutSelected()));
    connect ( setbtn, SIGNAL (clicked()), SLOT (changeShortcut()));
    connect ( kinput, SIGNAL ( textChanged( const QString )), this, SLOT (shortcutEdited ()));
    wgt.exec ();
    kinput = NULL;
    list = NULL;
}

void ActionManager::setDefaultClicked ()
{
QString newShortcut = selectedAction->property("Default shortcut").toString();
QString result = setShortcutForAction ( selectedAction, QKeySequence (newShortcut));
 if ( result == "OK")
 {
     list->selectedItems()[0]->setText (1,newShortcut);
     kinput->setText (newShortcut);
	 settings->remove ("ActionManarer/Shortcuts/"+selectedAction->actionGroup()->objectName()+'/'+selectedAction->objectName());
     setbtn->setEnabled (false);
  }
 else
 {
    QMessageBox::warning ( kinput, "Error","Key sequence "+newShortcut+" already assigned to the \'"+result+'\"',QMessageBox::Close);
    kinput->setText ( selectedAction->shortcut().toString());
  }
}

void ActionManager::changeShortcut ()
{
 QString newShortcut = kinput->text();
 QString result = setShortcutForAction ( selectedAction, QKeySequence (newShortcut));
 if ( result == "OK")
 {
     list->selectedItems()[0]->setText (1,newShortcut);
     kinput->setText (newShortcut);
     setbtn->setEnabled (false);
	 settings->setValue ("ActionManarer/Shortcuts/"+selectedAction->actionGroup()->objectName()+'/'+selectedAction->objectName(),newShortcut);
	 qWarning ( qPrintable ("setted value"+selectedAction->actionGroup()->objectName()+'/'+selectedAction->objectName()));
 }
 else
 {
    QMessageBox::warning ( kinput, "Error","Key sequence "+newShortcut+" already assigned to the \'"+result+'\"',QMessageBox::Close);
    kinput->setText ( selectedAction->shortcut().toString());
  }
 }
 
 QString ActionManager::setShortcutForAction ( MonkeyAction* action, QKeySequence shortcut)
 {
  bool existWithSameShortcut = false;
    QString actionName;
    QList<QAction*> actions;  
     for ( int grpn = 0; grpn< actionGroups.size (); grpn++ )
	{
		actions = actionGroups[grpn]->actions();
		for (int actn = 0; actn < actions.size(); actn ++)
        {
            if ((actions[actn] != action) and (actions[actn]->shortcut() == shortcut) and (shortcut!=QKeySequence()))
            {
                existWithSameShortcut = true;
                actionName = actionGroups[grpn]->property("text").toString()+'/'+actions[actn]->text();
                break;
            }
        }   
        if (existWithSameShortcut) 
            break;
    }
 if ( existWithSameShortcut )
 {
     qDebug (qPrintable ("Action with shortcut "+shortcut.toString()+"already exist ("+actionName+")"));
     return actionName;
 }
 else
 {
     action->setShortcut ( shortcut);
     return "OK";
 }
}
 
 void ActionManager::shortcutEdited ()
 {
   setbtn->setEnabled ( true);
 }
 
 void ActionManager::setSettings ( QSettings* newsettings, bool needToReload)
 {
	if ( settings == newsettings )
		return;
	settings = newsettings;
	if ( needToReload)
		reloadSettings ();
 }
 
 QSettings* ActionManager::getSettings ()
 {
	 return settings;
 }
 
 void ActionManager::reloadSettings ()
 {
	 QList<QAction*> actions;
	 for ( int i = 0; i< actionGroups.size(); i++)
	 {
		actions = actionGroups[i]->actions ();
		 for ( int j = 0; j< actions.size(); j ++)
			 actions[j]->setShortcut ( QKeySequence (settings->value("ActionManarer/Shortcuts/"+actionGroups[i]->objectName()+'/'+actions[j]->objectName(), actions[j]->property("Default shortcut")).toString()));
	 }
 }
 
 QKeySequence ActionManager::getShortCut ( QString groupName, QString actionName, QKeySequence defaultShortcut)
 {
	QKeySequence result  (settings->value("ActionManarer/Shortcuts/"+groupName+'/'+actionName, defaultShortcut).toString());
	return result;
 }
 
#endif //_ACTIONMANAGER_H
