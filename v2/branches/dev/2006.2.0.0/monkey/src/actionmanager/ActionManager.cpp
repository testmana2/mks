#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H

#include <QtGlobal>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDialog>
#include <QMessageBox>

#include "ActionManager.h"

ActionManager* ActionManager::mSelf = 0L;
QSettings* ActionManager::settings = 0L;

ActionManager* ActionManager::self()
{
	if ( !mSelf )
		mSelf = new ActionManager();
		
	return mSelf;
}

void ActionManager::addActGroup ( MonkeyActGroup* grp )
{
	actionGroups.append ( grp );
}

void ActionManager::deleteActGroup ( MonkeyActGroup* grp )
{
	actionGroups.removeAll ( grp );
}

void ActionManager::shortcutSelected ()
{
	QTreeWidgetItem * item = actListWidget->selectedItems() [ 0 ]; //do not need select more then one :D
	
	if ( not item->parent() )    //seems it's a group header selected
	{
		defaultBtn->setEnabled ( false );
		clearBtn->setEnabled ( false );
		return ;
	}
	
	defaultBtn->setEnabled ( true );
	clearBtn->setEnabled ( true );
	QString groupText = item->parent() ->text( 0 );
	MonkeyActGroup* group = NULL;
	
	for ( int i = 0; i < actionGroups.size(); i++ )
		if ( actionGroups[ i ] ->property( "text" ).toString() == groupText )
		{
			group = actionGroups[ i ];
			break;
		}
		
	Q_CHECK_PTR ( group );
	//now maybe group is finded :D
	selectedAction = NULL;
	
	for ( int i = 0; i < group->actions().size(); i++ )
		if ( group->actions() [ i ] ->text() == item->text( 0 ) )    //
		{
			selectedAction = ( MonkeyAction* ) group->actions() [ i ];
			break;
		}
		
	Q_CHECK_PTR ( selectedAction );
	keyInput->setEnabled ( true );
	keyInput->setText ( selectedAction->shortcut().toString() );
	setBtn->setEnabled ( false );
}

void ActionManager::showSettings ()
{
	QDialog wgt ( NULL, Qt::Dialog );
	wgt.setFixedSize ( 400, 480 );
	QVBoxLayout vbox ( &wgt );
	actListWidget = new QTreeWidget ( &wgt );
	actListWidget->setColumnCount ( 2 );
	actListWidget->header() ->hide();
	actListWidget->header() ->setResizeMode( QHeaderView::ResizeToContents );
	QString grpName;
	QTreeWidgetItem* grpitem;
	QTreeWidgetItem* newItem;
	QList<QAction*> actions;
	
	for ( int grpn = 0; grpn < actionGroups.size (); grpn++ )
	{
		grpitem = new QTreeWidgetItem ( actListWidget, QStringList ( actionGroups[ grpn ] ->property( "text" ).toString() ) );
		grpitem->setExpanded ( true );
		actions = actionGroups[ grpn ] ->actions();
		
		for ( int actn = 0; actn < actions.size(); actn ++ )
		{
			newItem = new QTreeWidgetItem ( grpitem, QStringList () << ( grpName + actions[ actn ] ->text() ) << ( actions[ actn ] ->shortcut().toString() ) );
			newItem->setIcon ( 0, actions[ actn ] ->icon() );
		}
		
	}
	
	vbox.addWidget ( actListWidget );
	QHBoxLayout confBox ( &wgt );
	defaultBtn = new QPushButton ( "Restore default", &wgt );
	defaultBtn->setEnabled ( false );
	confBox.addWidget ( defaultBtn );
	clearBtn = new QPushButton ( "Clear", &wgt );
	clearBtn->setEnabled ( false );
	confBox.addWidget ( clearBtn );
	keyInput = new KeySequenceInput ( &wgt );
	keyInput->setEnabled ( false );
	confBox.addWidget ( keyInput );
	setBtn = new QPushButton ( "Set", &wgt );
	setBtn->setEnabled ( false );
	confBox.addWidget ( setBtn );
	vbox.addLayout ( &confBox );
	QHBoxLayout hbox ( &wgt );
	vbox.addLayout ( &hbox );
	QPushButton closeBtn ( "Close", &wgt );
	hbox.addWidget ( &closeBtn );
	connect ( &closeBtn, SIGNAL ( clicked() ), &wgt, SLOT ( close() ) );
	connect ( clearBtn, SIGNAL ( clicked() ), keyInput, SLOT ( clear() ) );
	connect ( defaultBtn, SIGNAL ( clicked() ), this, SLOT ( setDefaultClicked() ) );
	connect ( actListWidget, SIGNAL ( itemSelectionChanged() ), this, SLOT ( shortcutSelected() ) );
	connect ( setBtn, SIGNAL ( clicked() ), SLOT ( changeShortcut() ) );
	connect ( keyInput, SIGNAL ( textChanged( const QString ) ), this, SLOT ( shortcutEdited () ) );
	wgt.exec (); //root of evil
	selectedAction = NULL;
	keyInput = NULL;
	actListWidget = NULL;
}

void ActionManager::setDefaultClicked ()
{
	QString newShortcut = selectedAction->property( "Default shortcut" ).toString();
	QString result = setShortcutForAction ( selectedAction, QKeySequence ( newShortcut ) );
	
	if ( result == "OK" )
	{
		actListWidget->selectedItems() [ 0 ] ->setText ( 1, newShortcut );
		keyInput->setText ( newShortcut );
		
		settings->remove ( "ActionManarer/Shortcuts/" + selectedAction->actionGroup() ->objectName() + '/' + selectedAction->objectName() )
		
		;
		setBtn->setEnabled ( false );
	}
	
	else
	{
		QMessageBox::warning ( keyInput, "Error", "Key sequence " + newShortcut + " already assigned to the \'" + result + '\"', QMessageBox::Close );
		keyInput->setText ( selectedAction->shortcut().toString() );
	}
}

void ActionManager::changeShortcut ()
{
	QString newShortcutText = keyInput->text();
	QString result = setShortcutForAction ( selectedAction, QKeySequence ( newShortcutText ) );
	
	if ( result == "OK" )
	{
		actListWidget->selectedItems() [ 0 ] ->setText ( 1, newShortcutText );
		keyInput->setText ( newShortcutText );
		setBtn->setEnabled ( false );
		settings->setValue ( "ActionManarer/Shortcuts/" + selectedAction->actionGroup() ->objectName() + '/' + selectedAction->objectName(), newShortcutText );
		qWarning ( qPrintable ( "setted value" + selectedAction->actionGroup() ->objectName() + '/' + selectedAction->objectName() ) );
	}
	
	else
	{
		QMessageBox::warning ( keyInput, "Error", "Key sequence " + newShortcutText + " already assigned to the \'" + result + '\"', QMessageBox::Close );
		keyInput->setText ( selectedAction->shortcut().toString() );
	}
}

QString ActionManager::setShortcutForAction ( MonkeyAction* action, QKeySequence shortcut )
{
	bool existWithSameShortcut = false;
	QString actionName;
	QList<QAction*> actions;
	
	for ( int grpn = 0; grpn < actionGroups.size (); grpn++ )
	{
		actions = actionGroups[ grpn ] ->actions();
		
		for ( int actn = 0; actn < actions.size(); actn ++ )
		{
			if ( ( actions[ actn ] != action ) and ( actions[ actn ] ->shortcut() == shortcut ) and ( shortcut != QKeySequence() ) )
			{
				existWithSameShortcut = true;
				actionName = actionGroups[ grpn ] ->property( "text" ).toString() + '/' + actions[ actn ] ->text();
				break;
			}
			
		}
		
		if ( existWithSameShortcut )
			break;
	}
	
	if ( existWithSameShortcut )
	{
		qDebug ( qPrintable ( "Action with shortcut " + shortcut.toString() + "already exist (" + actionName + ")" ) );
		return actionName;
	}
	
	else
	{
		action->setShortcut ( shortcut );
		return "OK";
	}
}

void ActionManager::shortcutEdited ()
{
	setBtn->setEnabled ( true );
}

void ActionManager::setSettings ( QSettings* newSettings, bool needToReload )
{
	Q_CHECK_PTR ( newSettings );
	
	if ( settings == newSettings )
		return ;
		
	settings = newSettings;
	
	if ( needToReload )
		reloadSettings ();
}

QSettings* ActionManager::getSettings ()
{
	return settings;
}

void ActionManager::reloadSettings ()
{
	QList<QAction*> actions;
	
	for ( int i = 0; i < actionGroups.size(); i++ )
	{
		actions = actionGroups[ i ] ->actions ();
		
		for ( int j = 0; j < actions.size(); j ++ )
			actions[ j ] ->setShortcut ( QKeySequence ( settings->value( "ActionManarer/Shortcuts/" + actionGroups[ i ] ->objectName() + '/' + actions[ j ] ->objectName(), actions[ j ] ->property( "Default shortcut" ) ).toString() ) );
	}
}

QKeySequence ActionManager::getShortCut ( QString groupName, QString actionName, QKeySequence defaultShortcut )
{
	QKeySequence result ( settings->value( "ActionManarer/Shortcuts/" + groupName + '/' + actionName, defaultShortcut ).toString() );
	return result;
}

#endif //_ACTIONMANAGER_H
