#include "pShortcutsEditor.h"
#include "pKeySequenceInput.h"
#include "pActionManager.h"
#include "pActionGroup.h"
#include "pAction.h"

#include <QBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

pShortcutsEditor::pShortcutsEditor( QWidget* w )
	: QDialog( w )
{
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( tr( "Shortcuts Editor" ) );
	setFixedSize ( 400, 480 );

	twShortcuts = new QTreeWidget( this );
	twShortcuts->setObjectName( "twShortcuts" );
	twShortcuts->setColumnCount( 2 );
	twShortcuts->header()->setResizeMode( QHeaderView::ResizeToContents );
	twShortcuts->header()->hide();

	pbRestore = new QPushButton( tr( "&Restore default" ), this );
	pbRestore->setObjectName( "pbRestore" );
	pbRestore->setEnabled( false );

	pbClear = new QPushButton( tr( "C&lear" ), this );
	pbClear->setObjectName( "pbClear" );
	pbClear->setEnabled( false );

	leShortcut = new pKeySequenceInput( this );
	leShortcut->setObjectName( "leShortcut" );
	leShortcut->setEnabled( false );

	pbSet = new QPushButton( tr( "&Set" ), this );
	pbSet->setObjectName( "pbSet" );
	pbSet->setEnabled( false );

	pbClose = new QPushButton( tr( "&Close" ), this );
	pbClose->setObjectName( "pbClose" );

	QBoxLayout* hl = new QBoxLayout( QBoxLayout::LeftToRight );
	hl->setMargin( 0 );
	hl->setSpacing( 3 );
	hl->addWidget( pbRestore );
	hl->addWidget( pbClear );
	hl->addWidget( leShortcut );
	hl->addWidget( pbSet );

	QBoxLayout* vl = new QBoxLayout( QBoxLayout::TopToBottom, this );
	vl->setMargin( 5 );
	vl->setSpacing( 3 );
	vl->addWidget( twShortcuts );
	vl->addLayout( hl );
	vl->addWidget( pbClose );

	// fill tree
	foreach ( pActionGroup* g, pActionManager::instance()->actionGroups )
	{
		// group item
		QTreeWidgetItem* gi = new QTreeWidgetItem( twShortcuts );
		gi->setText( 0, g->property( "Caption" ).toString() );
		gi->setExpanded( true );

		foreach ( QAction* a, g->actions() )
		{
			// action item
			QTreeWidgetItem* it = new QTreeWidgetItem( gi );
			it->setText( 0, a->text() );
			it->setText( 1, a->shortcut().toString() );
			it->setIcon( 0, a->icon() );
			// stock pointer action in data
			it->setData( 0, Qt::UserRole, reinterpret_cast<uint>( a ) );
		}
	}

	// connections
	connect( pbClear, SIGNAL( clicked() ), leShortcut, SLOT( clear() ) );
	connect( pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );
	QMetaObject::connectSlotsByName( this );
}

void pShortcutsEditor::on_twShortcuts_itemSelectionChanged()
{
	// get selected item
	QTreeWidgetItem* it = twShortcuts->selectedItems().value( 0 );
	bool b = it && it->parent();

	// set button state according to item is an action
	pbRestore->setEnabled( b );
	pbClear->setEnabled( b );

	// if not an action cancel
	if ( !b )
		return;

	// get action
	QAction* a = reinterpret_cast<QAction*>( it->data( 0, Qt::UserRole ).toUInt() );

	// show action information
	if ( a )
	{
		leShortcut->setEnabled( true );
		leShortcut->setText( a->shortcut().toString() );
		pbSet->setEnabled( false );
	}
}

void pShortcutsEditor::on_pbRestore_clicked()
{
	// get selected item
	QTreeWidgetItem* it = twShortcuts->selectedItems().value( 0 );

	// get action
	pAction* a = reinterpret_cast<pAction*>( it->data( 0, Qt::UserRole ).toUInt() );

	if ( a )
	{
		// get default action shortcut
		QString s = a->property( "Default Shortcut" ).toString();

		// try assigning new shortcut
		QString r = pActionManager::instance()->setShortcutForAction( a, QKeySequence( s ) );

		// if assigned ok
		if ( r.toLower() == "ok" )
		{
			it->setText( 1, s );
			leShortcut->setText( s );
			pbSet->setEnabled( false );

			// remove old shortcut entry
			if ( pActionManager::instance()->settings() )
				pActionManager::instance()->settings()->remove( QString( "ShortcutsManager/%1/%2" ).arg( a->actionGroup()->objectName() ).arg( a->objectName() ) );
		}
		// show warning
		else
		{
			QMessageBox::warning( window(), tr( "Error" ), tr( "Key Sequence '%1' already assigned to the '%2'" ).arg( s ).arg( r ), QMessageBox::Close );
			leShortcut->setText( a->shortcut().toString() );
		}
	}
}

void pShortcutsEditor::on_leShortcut_textChanged( const QString& )
{
	pbSet->setEnabled( true );
}

void pShortcutsEditor::on_pbSet_clicked()
{
	// get selected item
	QTreeWidgetItem* it = twShortcuts->selectedItems().value( 0 );

	// get action
	QAction* a = reinterpret_cast<QAction*>( it->data( 0, Qt::UserRole ).toUInt() );

	if ( a )
	{
		QString s = leShortcut->text();
		QString r = pActionManager::instance()->setShortcutForAction( qobject_cast<pAction*>( a ), QKeySequence( s ) );

		if ( r.toLower() == "ok" )
		{
			it->setText( 1, s );
			leShortcut->setText( s );
			pActionManager::instance()->settings()->setValue( QString( "ShortcutsManager/%1/%2" ).arg( a->actionGroup()->objectName() ).arg( a->objectName() ), s );
			pbSet->setEnabled( false );
		}
		else
		{
			QMessageBox::warning( window(), tr( "Error" ), tr( "Key Sequence '%1' already assigned to the '%2'" ).arg( s ).arg( r ), QMessageBox::Close );
			leShortcut->setText( a->shortcut().toString() );
		}
	}
}
