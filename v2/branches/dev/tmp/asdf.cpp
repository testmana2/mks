#include "asdf.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QListWidget>

pStringListEditor::pStringListEditor (QWidget* parent) : QGroupBox (parent)
{
    QVBoxLayout* vbox = new QVBoxLayout (this);
    vbox->setSpacing(3);
    vbox->setContentsMargins(5, 5, 5, 5);
    QToolBar* buttons = new QToolBar (this);
    vbox->addWidget (buttons);
    QToolButton* addBtn = new QToolButton ();
    addBtn->setIcon (QIcon (":/icons/add.png"));
    connect (addBtn, SIGNAL (clicked()), this, SLOT (on_Add_clicked ()));
    buttons->addWidget (addBtn);
    QToolButton* removeBtn = new QToolButton ();
    connect (removeBtn, SIGNAL (clicked()), this, SLOT (on_Remove_clicked ()));
    removeBtn->setIcon (QIcon (":/icons/remove.png"));
    buttons->addWidget (removeBtn);    
    QToolButton* clearBtn = new QToolButton ();
    clearBtn->setIcon (QIcon (":/icons/update.png"));
    connect (clearBtn, SIGNAL (clicked()), this, SLOT (on_Clear_clicked ()));
    buttons->addWidget (clearBtn);
    QToolButton* upBtn = new QToolButton ();
    upBtn->setIcon (QIcon (":/icons/up.png"));
    connect (upBtn, SIGNAL (clicked()), this, SLOT (on_Up_clicked ()));
    buttons->addWidget (upBtn);
    QToolButton* downBtn = new QToolButton ();
    downBtn->setIcon (QIcon (":/icons/down.png"));
    connect (downBtn, SIGNAL (clicked()), this, SLOT (on_Down_clicked ()));
    buttons->addWidget (downBtn);

    list = new QListWidget (this);
    vbox->addWidget (list);
}

pStringListEditor::~pStringListEditor ()
{
}

void pStringListEditor::on_Add_clicked()
{
	list->addItem(tr("New item"));
	QListWidgetItem* it = list->item( list->count() -1 );
	list->setCurrentItem( it );
	list->scrollToItem( it );
	it->setFlags( it->flags() | Qt::ItemIsEditable );
}

void pStringListEditor::on_Remove_clicked()
{ delete list->selectedItems().value( 0 ); }

void pStringListEditor::on_Clear_clicked()
{ list->clear(); }

void pStringListEditor::on_Up_clicked()
{
	if ( QListWidgetItem* it = list->selectedItems().value( 0 ) )
	{
		int i = list->row( it );
		if ( i != 0 )
			list->insertItem( i -1, list->takeItem( i ) );
		list->setCurrentItem( it );
	}
}

void pStringListEditor::on_Down_clicked()
{
	if ( QListWidgetItem* it = list->selectedItems().value( 0 ) )
	{
		int i = list->row( it );
		if ( i != list->count() -1 )
			list->insertItem( i +1, list->takeItem( i ) );
		list->setCurrentItem( it );
	}
}
