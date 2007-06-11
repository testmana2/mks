#include "pActionManager.h"
#include "pActionGroup.h"
#include "pAction.h"
#include "pKeySequenceInput.h"

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QSettings>

pActionManager::pActionManager( QObject* o )
	: QObject( o ), mSettings( 0L )
{
}

void pActionManager::setSetting( QSettings* s, bool b )
{
	mSettings = s;
	if ( b )
		reloadSettings();
}

QSettings* pActionManager::settings() const
{
	return mSettings;
}

void pActionManager::reloadSettings()
{
	// need reload shortcut
}

void pActionManager::addGroup( pActionGroup* g )
{
	actionGroups.append( g );
}

void pActionManager::removeGroup( pActionGroup* g )
{
	actionGroups.removeAll( g );
}

QKeySequence pActionManager::getShortCut( pAction* a )
{
	if ( !instance()->mSettings )
		return QKeySequence();

	QString s = "Shortcuts";
	qDebug( qPrintable( s ) );
	qDebug( qPrintable( instance()->mSettings->value( "shortcutName" ).toString() +"kj" ) );
	return QKeySequence( instance()->mSettings->value( "shortcutName" ).toString() );
}

void pActionManager::shortcutSelected()
{
	// get selected item
	QTreeWidgetItem* it = list->selectedItems().value( 0 );

	// if item is a group header cancel
	if( not it->parent() )
	{
		defaultbtn->setEnabled( false );
		clearbtn->setEnabled( false );
		return; //Group header selected
	}

	// enable buttons
	defaultbtn->setEnabled( true );
	clearbtn->setEnabled( true );

	// get groupe name
	QString sg = it->parent()->text( 0 );

	// get group
	pActionGroup* g;

	// get tree
	QTreeWidget* tree = it->treeWidget();

	foreach ( pActionGroup* ag, actionGroups )
	{
		if ( ag->property( "Caption" ).toString() == sg )
		{
			g = ag;
			break;
		}
	}

	//now maybe group is finded ;)
	foreach ( QAction* a, g->actions() )
	{
		if ( a->text() == it->text( 0 ) )
		{
			selectedAction = qobject_cast<pAction*>( a );
			break;
		}
	}

	if ( selectedAction )
	{
		kinput->setEnabled( true );
		kinput->setText( selectedAction->shortcut().toString() );
		setbtn->setEnabled( false );
	}
}

void pActionManager::showSettings()
{
	// FIXME: from P@sNox to hlamer, do this dialog in a new class, it s really durty !
	selectedAction = 0;
	QDialog d( QApplication::activeWindow() );
	d.setFixedSize( 400, 400 );
	QVBoxLayout vb( &d );
	list = new QTreeWidget;
	list->setColumnCount( 2 );
	list->header()->hide();
	list->header()->setResizeMode( QHeaderView::ResizeToContents );
	vb.addWidget( list );

	// fill tree
	foreach ( pActionGroup* g, actionGroups )
	{
		QTreeWidgetItem* gi = new QTreeWidgetItem( list );
		gi->setText( 0, g->property( "Caption" ).toString() );
		gi->setExpanded( true );
		foreach ( QAction* a, g->actions() )
		{
			QTreeWidgetItem* it = new QTreeWidgetItem( gi );
			it->setText( 0, a->text() );
			it->setText( 1, a->shortcut().toString() );
			it->setIcon( 0, a->icon() );
		}
	}

	QHBoxLayout hb( &d );
	vb.addLayout( &hb );
	defaultbtn = new QPushButton( "Restore default", &d );
	defaultbtn->setEnabled( false );
	hb.addWidget( defaultbtn );
	clearbtn = new QPushButton( "Clear", &d );
	clearbtn->setEnabled( false );
	hb.addWidget( clearbtn );
	kinput = new pKeySequenceInput( &d );
	kinput->setEnabled( false );
	hb.addWidget( kinput );
	setbtn = new QPushButton( "Set", &d );
	setbtn->setEnabled( false );
	hb.addWidget( setbtn );
	QHBoxLayout hbox( &d );
	vb.addLayout( &hbox);
	QPushButton closebtn( "Close", &d );
	hbox.addWidget( &closebtn );

	connect( &closebtn, SIGNAL( clicked() ), &d, SLOT( close() ) );
	connect( clearbtn, SIGNAL( clicked() ), kinput, SLOT( clear() ) );
	connect( defaultbtn, SIGNAL( clicked() ), this, SLOT( setDefaultClicked() ) );
	connect( list, SIGNAL( itemSelectionChanged() ), this, SLOT( shortcutSelected() ) );
	connect( setbtn, SIGNAL( clicked() ), SLOT(changeShortcut()));
	connect( kinput, SIGNAL( textChanged( const QString& ) ), this, SLOT( shortcutEdited() ) );

	d.exec();

	kinput = NULL;
	list = NULL;
}

void pActionManager::setDefaultClicked()
{
	if ( list->selectedItems().count() )
	{
		QString s = selectedAction->property( "Default Shortcut" ).toString();
		kinput->setText( s );
		list->selectedItems()[0]->setText( 1, s );
	}
}

void pActionManager::changeShortcut()
{
	if ( !list->selectedItems().count() )
		return;

	QString s = kinput->text();
	QString r = setShortcutForAction( selectedAction, QKeySequence( s ) );
	if( r == "OK" )
	{
		list->selectedItems()[0]->setText( 1, s );
		setbtn->setEnabled( false );
	}
	else
	{
		QMessageBox::warning( kinput, "Error","Key sequence "+s+" already assigned to the \'"+r+'\"', QMessageBox::Close );
		kinput->setText( selectedAction->shortcut().toString() );
	}  
}

QString pActionManager::setShortcutForAction( pAction* action, const QKeySequence& shortcut)
{
	bool existWithSameShortcut = false;
	QString actionName;

	foreach ( pActionGroup* ag, actionGroups )
	{
		foreach ( QAction* a, ag->actions() )
		{
			if ( a != action && a->shortcut() != shortcut && !shortcut.isEmpty() )
			{
				existWithSameShortcut = true;
				actionName = a->objectName() + "/" + a->text();
				break;
			}
		}
		if ( existWithSameShortcut )
			break;
	}

	if ( existWithSameShortcut )
	{
		qDebug( qPrintable( "Action with shortcut " +shortcut.toString() +"already exist(" +actionName +")" ) );
		return actionName;
	}
	else
	{
		action->setShortcut( shortcut );
		return "OK";
	}
}

void pActionManager::shortcutEdited()
{
	setbtn->setEnabled( true );
}
