#include <QMenu>
#include <QDebug>
#include <QMessageBox>

#include "NoProjectProjectItem.h"
#include "ProjectPlugin.h"
#include "UINoProjectProjectSettings.h"
#include "pCommand.h"
#include "pConsoleManager.h"
#include "PluginsManager.h"
#include "pMenuBar.h"


NoProjectProjectItem::NoProjectProjectItem ()
	: ProjectItem()
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
			delete t.action;  // !!! I can not see way to delete action from menu, so, deleting it and cleating again
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
        targets[i].action = pMenuBar::instance()->action(QString("mBuild/aAction%1").arg(i),targets[i].text);  // !!! I can not see    way to delete action from menu, so, deleting it and creating again. hlamer
		connect ( targets[i].action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
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
    qWarning () <<"Opening project "<<s;
	setFilePath (s);
	QSettings settings (s, QSettings::IniFormat);
	if ( settings.status() == QSettings::AccessError)
	{
		QMessageBox::warning (QApplication::activeWindow(),"Error",QString("Access denided for a file %1").arg (s));
		return false;
	}
	else if ( settings.status() == QSettings::FormatError)
	{
		QMessageBox::warning (QApplication::activeWindow(),"Error",QString("Wrong file %1").arg (s));
		return false;
	}
	int targetsCount = settings.value ("targetsCount").toInt();
	if (targetsCount > 0 and targetsCount < 6)
	{
		QMessageBox::warning (QApplication::activeWindow(),"Error",QString("Wrong file %1 (Too less actions described").arg (s));
		return false;
	}
	if ( targetsCount > 0)
	{ //opening existing project
		setValue( settings.value ("projectName", "Project").toString());
		projectPath = settings.value ("projectPath", canonicalFilePath()).toString();
		
		QString text, command;
		for ( int i = 0; i < targetsCount; i++)
		{
			text = settings.value (QString ("target%1text").arg(i),"").toString();
			command = settings.value (QString ("target%1command").arg(i),"").toString();
			targets.append ( (Target){text, command,NULL});
		}
	}
	else
	{ //creating new
		setValue( "Project" );
		projectPath = canonicalPath ();
	}
	
	if ( targetsCount == 0) //new project created
		editSettings ();
    qWarning ("successfully opened");
	return true;
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
