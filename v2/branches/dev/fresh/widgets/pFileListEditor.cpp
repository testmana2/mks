#include <QAction>
#include <QFileDialog>

#include "pFileListEditor.h"

pFileListEditor::pFileListEditor(QString title, QFileDialog::FileMode fm, QString filter, QWidget* parent):
	pStringListEditor(title, parent), mFileMode (fm), mFilter (filter)
{}

pFileListEditor::~pFileListEditor()
{};

void pFileListEditor::onActionTriggered (QAction* act)
{
	if (act->text () == "add")
	{
		list->addItems(QFileDialog::getOpenFileNames ( this, tr ("Choose file(s)"), mDir, mFilter));
		QListWidgetItem* it = list->item( list->count() -1 );
		list->setCurrentItem( it );
		list->scrollToItem( it );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
	}
	else
		return pStringListEditor::onActionTriggered (act);
}

void pFileListEditor::setDir (QString s)
{
	mDir = s;
}
