#include <QMenu>
#include <QDebug>

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
	setValue( "Project" );
	setFilePath ("toto.noproject");
	pMenuBar* mb = pMenuBar::instance();
	targets.append ( (Target){"Build","",mb->action("mBuild/mBuild/aCurrent")});
	targets.append ( (Target){"Rebuild","",mb->action("mBuild/mRebuild/aCurrent")});
	targets.append ( (Target){"Clean","",mb->action("mBuild/mClean/aCurrent")});
	targets.append ( (Target){"Distclean","",mb->action("mBuild/mDistClean/aCurrent")});
	targets.append ( (Target){"Execute","",mb->action("mBuild/aExecute")});
	targets.append ( (Target){"Build & execute","",mb->action("mBuild/aBuildExecute")});
	editSettings();
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
