#include <QAction>
#include <QFileDialog>

#include "pFileListEditor.h"

pFileListEditor::pFileListEditor(QString dir, QFileDialog::FileMode fm, QString filter, QWidget* parent):
	mDir (dir), mFileMode (fm), mFilter (filter), pStringListEditor (parent)
{}

pFileListEditor::~pFileListEditor()
{};

void pFileListEditor::onActionTriggered (QAction* act)
{
	if (act->objectName() == "add")
	{
		list->addItems(QFileDialog::getOpenFileNames ( this, tr ("Choose file(s)"), mDir, mFilter));
		QListWidgetItem* it = list->item( list->count() -1 );
		list->setCurrentItem( it );
		list->scrollToItem( it );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
	}
	else
		pStringListEditor::onActionTriggered (act);
}
