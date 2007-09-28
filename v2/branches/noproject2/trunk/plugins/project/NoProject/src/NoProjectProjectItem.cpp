/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : NoProject project type 
** FileName  : NoProjectProjectItem.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : NoProject project class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QMenu>
#include <QMessageBox>

#include "NoProjectProjectItem.h"
#include "ProjectPlugin.h"
#include "UINoProjectProjectSettings.h"
#include "pCommand.h"
#include "pConsoleManager.h"
#include "PluginsManager.h"
#include "pMenuBar.h"


NoProjectProjectItem::NoProjectProjectItem () : ProjectItem()
{
	setType( ProjectsModel::ProjectType );
	setModified (false);
}

NoProjectProjectItem::~NoProjectProjectItem ()
{
	removeSelfFromMenu ();
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}

void NoProjectProjectItem::close()
{
	model()->removeRow( row(), index().parent() );
}

void NoProjectProjectItem::save (bool)
{
	QSettings settings (getFilePath(), QSettings::IniFormat);
	settings.setValue ("projectName", getValue());
	settings.setValue ("projectPath", projectPath);
	settings.setValue ("targetsCount", targets.size());
	for ( int i = 0; i < targets.size(); i++)
	{
		settings.setValue (QString ("target%1text").arg(i), targets[i].text);
		settings.setValue (QString ("target%1command").arg(i), targets[i].command);
	}
	setModified (false);
}

void NoProjectProjectItem::buildMenuTriggered ()
{
	foreach ( Target t, targets)
		if ( t.action == sender())
		{
				pCommand* cmd = new pCommand;
				cmd->setDefaultCommand (t.command);
				cmd->setWorkingDirectory (canonicalPath());
                cmd->setSkipOnError (false);
				cmd->setTryAllParsers (true);
				pConsoleManager::instance()->addCommand(cmd);
				return;
		}
};

void NoProjectProjectItem::removeSelfFromMenu (QMenu* menu)
{
	foreach ( Target t, targets)
		if (t.action)
        {
			t.action->setVisible (false);
            t.action = NULL;
        }
}

void NoProjectProjectItem::addSelfToMenu (QMenu* menu)
{
	if ( !menu)
		menu = pMenuBar::instance()->menu( "mBuild" );
	menu->setEnabled (true);	
	for (int i = 0; i < targets.size(); i++)
	{
		if (targets[i].action)
			targets[i].action->setVisible (true)
		else
        {	targets[i].action = pMenuBar::instance()->action(QString("mBuild/aAction%1").arg(i),targets[i].text);  // !!! I can not see    way to delete action from menu, so, deleting it and creating again. hlamer
			connect ( targets[i].action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
		}
	targets[i].action->setEnabled ( !targets[i].command.isEmpty() );
	targets[i].action->setText (targets[i].text);
	}
}

ProjectPlugin* NoProjectProjectItem::getParentPlugin ()
{
	return PluginsManager::instance()->plugin<ProjectPlugin*>( "NoProject" );
}

void NoProjectProjectItem::setValue (QString s)
{
	ProjectItem::setValue (s);
	setText (s);
}

bool NoProjectProjectItem::openProject( const QString& s)
{
	setFilePath (s);
	QSettings settings (s, QSettings::IniFormat);
	if ( settings.status() == QSettings::AccessError)
	{
		QMessageBox::warning (QApplication::activeWindow(),tr("Error"),tr("Access denided for a file %1").arg (s));
		return false;
	}
	else if ( settings.status() == QSettings::FormatError)
	{
		QMessageBox::warning (QApplication::activeWindow(),tr("Error"),tr("Wrong file %1").arg (s));
		return false;
	}

	setValue( settings.value ("projectName", "Project").toString());
	projectPath = settings.value ("projectPath", canonicalFilePath()).toString();

	int targetsCount = settings.value ("targetsCount").toInt();
	QString text, command;
	for ( int i = 0; i < targetsCount; i++)
	{
		text = settings.value (QString ("target%1text").arg(i),"").toString();
		command = settings.value (QString ("target%1command").arg(i),"").toString();
		targets.append ( (Target){text, command,NULL});
	}

	return true;
}
