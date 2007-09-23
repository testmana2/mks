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
	qDebug ()<<"oops";
	model()->removeRow( row(), index().parent() ); // just a little segfault
}

void NoProjectProjectItem::editSettings()
{
	new UINoProjectProjectSettings (this);
}

void NoProjectProjectItem::close()
{
	delete (this);
}

void NoProjectProjectItem::buildMenuTriggered ()
{
	foreach ( Target t, targets)
		if ( t.action == sender())
		{
				pCommand* com = new pCommand;
				com->setCommand (t.command);
				qDebug ()<< t.command;
				pConsoleManager::instance()->addCommand (com);
				return;
		}
};

void NoProjectProjectItem::makeTarget (int targetIndex)
{
	pCommand* com = new pCommand;
	com->setCommand (targets[targetIndex].command);
	qDebug ()<< targets[targetIndex].command;
	pConsoleManager::instance()->addCommand (com);
}

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
