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
    buttons->addWidget (addBtn);
    QToolButton* removeBtn = new QToolButton ();
    removeBtn->setIcon (QIcon (":/icons/remove.png"));
    buttons->addWidget (removeBtn);    
    QToolButton* clearBtn = new QToolButton ();
    clearBtn->setIcon (QIcon (":/icons/update.png"));
    buttons->addWidget (clearBtn);
    QToolButton* upBtn = new QToolButton ();
    upBtn->setIcon (QIcon (":/icons/up.png"));
    buttons->addWidget (upBtn);
    QToolButton* downBtn = new QToolButton ();
    downBtn->setIcon (QIcon (":/icons/down.png"));
    buttons->addWidget (downBtn);

    list = new QListWidget (this);
    vbox->addWidget (list);
}

pStringListEditor::~pStringListEditor ()
{
}