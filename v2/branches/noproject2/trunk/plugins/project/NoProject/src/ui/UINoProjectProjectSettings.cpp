#include <QFileDialog>

#include "UINoProjectProjectSettings.h"
#include "pMenuBar.h"

UINoProjectProjectSettings::UINoProjectProjectSettings(NoProjectProjectItem* pproject): QDialog()
{
	project = pproject;
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( "Project Settings");
	projectName->setText (project->getValue());
	projectPath->setText (project->canonicalPath());
	targets = project->targets;
	foreach (NoProjectProjectItem::Target t, targets)
		actionsList->addItem (t.text);
	connect (acceptBtn, SIGNAL (clicked()), this, SLOT (accept()));
	connect (pathDialogBtn, SIGNAL (clicked()), this, SLOT (pathDialogRequested()));
	connect (actionsList, SIGNAL (currentRowChanged(int)), this, SLOT(selectedRowChanged(int)));
	connect (actionName, SIGNAL (textEdited(QString)), this, SLOT(actionNameEdited(QString)));
	connect (addActionBtn, SIGNAL (clicked()), this, SLOT (addAction()));
	connect (removeActionBtn, SIGNAL (clicked()), this, SLOT (removeAction()));
	connect (commandsEdit, SIGNAL (textChanged()), this, SLOT (actionCommandEdited ()));
	exec();
}

void UINoProjectProjectSettings::accept()
{
	project->setValue(projectName->text());
	project->setText(projectName->text());
	project->setFilePath( projectPath->text()+".noproject");
	targets[ProjectItem::aReBuild].command = targets[ProjectItem::aClean].command+"\n"+targets[ProjectItem::aBuild].command;
	targets[ProjectItem::aBuildExecute].command = targets[ProjectItem::aBuild].command+"\n"+targets[ProjectItem::aExecute].command;
	project->targets = targets;
	project->removeSelfFromMenu ();
	project->addSelfToMenu ();
	QDialog::accept();
}

void UINoProjectProjectSettings::pathDialogRequested ()
{
	QString path = QFileDialog::getExistingDirectory(this,
     tr("Select project path"), projectPath->text());
	projectPath->setText (path);
}

void UINoProjectProjectSettings::selectedRowChanged(int actionNum)
{
	actionName->setText(targets[actionNum].text);
	commandsEdit->setText (targets[actionNum].command);
	commandsEdit->setEnabled (true);
	if ( actionsList->currentRow() >=ProjectItem::aLast )
	{	
		actionName->setEnabled (true);
		removeActionBtn->setEnabled (true);
	}
	else
	{
		actionName->setEnabled (false);
		removeActionBtn->setEnabled (false);
	}
}

void UINoProjectProjectSettings::actionNameEdited(QString name)
{
	actionsList->currentItem()->setText(name);
	targets[actionsList->currentRow()].text = name;
}

void UINoProjectProjectSettings::addAction()
{
	targets.append ( (NoProjectProjectItem::Target){"","",NULL});
	actionsList->addItem ("");
	actionsList->setCurrentRow (actionsList->count()-1);
	actionName->setFocus(Qt::OtherFocusReason);
}

void UINoProjectProjectSettings::removeAction()
{
	delete actionsList->takeItem (actionsList->currentRow());
}

void UINoProjectProjectSettings::actionCommandEdited ()
{
	targets[actionsList->currentIndex().row()].command = commandsEdit->toPlainText();
}
