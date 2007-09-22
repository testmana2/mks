#include <QFileDialog>

#include "UINoProjectProjectSettings.h"


UINoProjectProjectSettings::UINoProjectProjectSettings(NoProjectProjectItem* pproject): QDialog()
{
	project = pproject;
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( "Project Settings");
	projectName->setText (project->getValue());
	projectPath->setText (project->canonicalPath());
	targets = project->targets;
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
	project->targets = targets;
	project->updateBuildMenu ();
	QDialog::accept();
}

void UINoProjectProjectSettings::pathDialogRequested ()
{
	QString path = QFileDialog::getExistingDirectory(this,
     tr("Select project path"), projectPath->text());
	projectPath->setText (path);
}

void UINoProjectProjectSettings::selectedRowChanged(int actionnum)
{
	actionName->setText(targets[actionnum].text);
	commandsEdit->setText (targets[actionnum].command);
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
	targets.append ( (NoProjectProjectItem::Target){"","",NULL});
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
