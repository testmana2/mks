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
			disconnect ( t.action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
	for ( int i = ProjectItem::aLast; i < targets.size(); i++)
	{
		if (targets[i].action)
		{
			delete targets[i].action;  // !!! I can not see way to delete action from menu, so, deleting it and cleating again
			targets[i].action = NULL;
		}
	}
}

void NoProjectProjectItem::addSelfToMenu (QMenu* menu)
{
	if ( !menu)
		menu = pMenuBar::instance()->menu( "mBuild" );
	menu->setEnabled (true);	
	for ( int i = ProjectItem::aLast; i < targets.size(); i++)
	{
		targets[i].action = pMenuBar::instance()->action(QString("mBuild/aCustom%1").arg(i),targets[i].text);  // !!! I can not see way to delete action from menu, so, deleting it and creating again
	}
	foreach ( Target t, targets)
	{
		connect ( t.action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
		t.action->setEnabled ( !t.command.isEmpty() );
		t.action->setText (t.text);
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
		targets.append ( (Target){"Build","",NULL});
		targets.append ( (Target){"Rebuild","",NULL});
		targets.append ( (Target){"Clean","",NULL});
		targets.append ( (Target){"Distclean","",NULL});
		targets.append ( (Target){"Execute","",NULL});
		targets.append ( (Target){"Build & execute","",NULL});
	}
	
	pMenuBar* mb = pMenuBar::instance();
	targets[0].action = mb->action("mBuild/mBuild/aCurrent");
	targets[1].action = mb->action("mBuild/mRebuild/aCurrent");
	targets[2].action = mb->action("mBuild/mClean/aCurrent");
	targets[3].action = mb->action("mBuild/mDistClean/aCurrent");
	targets[4].action = mb->action("mBuild/aExecute");
	targets[5].action = mb->action("mBuild/aBuildExecute");
	
	if ( targetsCount == 0) //new project created
		editSettings ();

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
