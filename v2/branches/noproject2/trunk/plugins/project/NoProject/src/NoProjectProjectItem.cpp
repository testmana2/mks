#include <QMenu>
#include <QDebug>

#include "NoProjectProjectItem.h"
#include "ProjectPlugin.h"
#include "UIMain.h"
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
	targets.append ( (Target){"Clean","",mb->action("mBuild/mClean/aCurrent")});
	targets.append ( (Target){"Distclean","",mb->action("mBuild/mDistClean/aCurrent")});
	targets.append ( (Target){"Execute","",mb->action("mBuild/aExecute")});
	editSettings();
}

NoProjectProjectItem::~NoProjectProjectItem ()
{
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
		disconnect ( t.action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
}

void NoProjectProjectItem::addSelfToMenu (QMenu* menu)
{
	if ( !menu)
		menu = pMenuBar::instance()->menu( "mBuild" );
	foreach ( Target t, targets)
		connect ( t.action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
}

void NoProjectProjectItem::updateBuildMenu (QMenu* menu)
{
	if ( !menu)
		menu = pMenuBar::instance()->menu( "mBuild" );
	menu->setEnabled (true);
	UIMain* main = UIMain::instance();
	foreach ( Target t, targets)
		t.action->setEnabled ( !t.command.isEmpty() );
}

ProjectPlugin* NoProjectProjectItem::getParentPlugin ()
{
	return PluginsManager::instance()->plugin<ProjectPlugin*>( "NoProject" );
}
