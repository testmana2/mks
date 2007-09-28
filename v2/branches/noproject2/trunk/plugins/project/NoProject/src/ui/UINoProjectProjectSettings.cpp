/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : NoProject project plugin
** FileName  : NoProjectProjectSettings.cpp
** Date      : 2007-09-28
** License   : GPL
** Comment   : NoProject project settings
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QFileDialog>

#include "UINoProjectProjectSettings.h"
#include "pMenuBar.h"

UINoProjectProjectSettings::UINoProjectProjectSettings(NoProjectProjectItem* pproject): QDialog()
{
	project = pproject;
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( tr("Project Settings"));
	projectName->setText (project->getValue());
	projectPath->setText (project->projectPath);
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
	project->projectPath = projectPath->text();
	project->targets = targets;
	project->removeSelfFromMenu ();
	project->addSelfToMenu ();
	project->setModified (true);
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
