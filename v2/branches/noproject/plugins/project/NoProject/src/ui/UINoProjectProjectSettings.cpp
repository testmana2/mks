#include <QFileDialog>

#include "UINoProjectProjectSettings.h"
#include "NoProjectProjectItem.h"

UINoProjectProjectSettings::UINoProjectProjectSettings(NoProjectProjectItem* pproject): QDialog()
{
	project = pproject;
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( "Project Settings");
	projectName->setText (project->getName());
	projectPath->setText (project->getFilePath());
	commands ["Build"] = project->buildCommand;
	commands ["Clean"] = project->cleanCommand;
	commands ["Distclean"] = project->distCleanCommand;
	commands ["Execute"] = project->executeCommand;
	connect (acceptBtn, SIGNAL (clicked()), this, SLOT (accept()));
	connect (pathDialogBtn, SIGNAL (clicked()), this, SLOT (pathDialogRequested()));
	connect (actionsList, SIGNAL (currentTextChanged(QString)), this, SLOT(selectedActionChanged(QString)));
	connect (actionName, SIGNAL (textEdited(QString)), this, SLOT(actionNameEdited(QString)));
	connect (addActionBtn, SIGNAL (clicked()), this, SLOT (addAction()));
	connect (removeActionBtn, SIGNAL (clicked()), this, SLOT (removeAction()));
	connect (commandsEdit, SIGNAL (textChanged()), this, SLOT (actionCommandEdited ()));
	exec();
}

void UINoProjectProjectSettings::accept()
{
	project->setName(projectName->text());
	project->setFilePath( projectPath->text());
	project->buildCommand = commands ["Build"];
	project->cleanCommand = commands ["Clean"];
	project->distCleanCommand = commands ["Distclean"];
	project->executeCommand = commands ["Execute"];
	QDialog::accept();
}

void UINoProjectProjectSettings::pathDialogRequested ()
{
	QString path = QFileDialog::getExistingDirectory(this,
     tr("Select project path"), projectPath->text());
	projectPath->setText (path);
}

void UINoProjectProjectSettings::selectedActionChanged(QString action)
{
	actionName->setText(action);
	commandsEdit->setText (commands[action]);
	commandsEdit->setEnabled (true);
	if ( actionsList->currentRow() >3 )
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
}

void UINoProjectProjectSettings::addAction()
{
	actionsList->addItem ("");
	actionsList->setCurrentRow (actionsList->count()-1);
	addActionBtn->clearFocus();
	actionName->setFocus(Qt::OtherFocusReason);
}

void UINoProjectProjectSettings::removeAction()
{
	delete actionsList->takeItem (actionsList->currentRow());
}

void UINoProjectProjectSettings::actionCommandEdited ()
{
	commands[actionsList->currentItem()->text()] = commandsEdit->toPlainText();
}
