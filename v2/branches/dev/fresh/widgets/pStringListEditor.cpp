/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pStringListEditor.cpp
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pStringListEditor.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QListWidget>
#include <QList>
#include <QAction>

pStringListEditor::pStringListEditor (QString title, QWidget* parent) : QGroupBox (title, parent)
{
    QVBoxLayout* vbox = new QVBoxLayout (this);
	QList<QString> names; names << "add"<<"remove"<<"clear"<<"up"<<"down";
	QList<QString> hints; hints << tr("Add item")<<tr("Remove item")<<tr ("Clean list")<<tr("Move item up")<<tr("Move item down");
	QList<QString> icons; icons << ":/tools/icons/tools/add.png" << ":/tools/icons/tools/remove.png" << ":/tools/icons/tools/update.png" << ":/tools/icons/tools/up.png" << ":/tools/icons/tools/down.png";
	
    vbox->setSpacing(0);
    QToolBar* buttons = new QToolBar (this);
	buttons->setIconSize (QSize(16,16));
	for (int i =0; i < names.size(); i++)
	{
		QAction* act = buttons->addAction(QIcon (icons[i]),names[i]);
		act->setToolTip (hints[i]);
	}
	
    vbox->addWidget (buttons);
	
    list = new QListWidget (this);
	list->setMinimumHeight (40);
    vbox->addWidget (list);
	
    connect (buttons, SIGNAL (actionTriggered (QAction*)), this, SLOT (onActionTriggered (QAction*)));
	connect (list, SIGNAL(itemChanged (QListWidgetItem*)), this, SIGNAL (edited()));
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
	emit edited ();
}
