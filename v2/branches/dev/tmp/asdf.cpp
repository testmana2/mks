#include "asdf.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QListWidget>
#include <QList>
#include <QAction>

pStringListEditor::pStringListEditor (QWidget* parent) : QGroupBox (parent)
{
    QVBoxLayout* vbox = new QVBoxLayout (this);
	QList<QString> names; names << "add"<<"remove"<<"clear"<<"up"<<"down";
	QList<QString> hints; hints << tr("Add item")<<tr("Remove item")<<tr ("Clean list")<<tr("Move item up")<<tr("Move item down");
	QList<QString> icons; icons << ":/icons/add.png" << ":/icons/remove.png" << ":/icons/update.png" << ":/icons/up.png" << ":/icons/down.png";
	
    vbox->setSpacing(0);
    QToolBar* buttons = new QToolBar (this);
	buttons->setIconSize (QSize(16,16));
	for (int i =0; i < names.size(); i++)
	{
		QAction* act = buttons->addAction(QIcon (icons[i]),names[i]);
		act->setToolTip (hints[i]);
	}
	
    connect (buttons, SIGNAL (actionTriggered (QAction*)), this, SLOT (onActionTriggered (QAction*)));
    vbox->addWidget (buttons);
	
    list = new QListWidget (this);
    vbox->addWidget (list);
}

pStringListEditor::~pStringListEditor ()
{
}

void pStringListEditor::onActionTriggered (QAction* act)
{
	if (act->text () == "add")
	{
		list->addItem(tr("New item"));
		QListWidgetItem* it = list->item( list->count() -1 );
		list->setCurrentItem( it );
		list->scrollToItem( it );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		list->editItem (it);
	}
	else if (act->text() == "remove")
	{
		delete list->selectedItems().value( 0 ); 
	}
	else if (act->text() == "clear")
	{
		list->clear();
	}
	else if (act->text() == "up")
	{
		if ( QListWidgetItem* it = list->selectedItems().value( 0 ) )
		{
			int i = list->row( it );
			if ( i != 0 )
				list->insertItem( i -1, list->takeItem( i ) );
			list->setCurrentItem( it );
		}
	}
	else if (act->text() == "down")
	{
		if ( QListWidgetItem* it = list->selectedItems().value( 0 ) )
		{
			int i = list->row( it );
			if ( i != list->count() -1 )
				list->insertItem( i +1, list->takeItem( i ) );
			list->setCurrentItem( it );
		}
	}
}
