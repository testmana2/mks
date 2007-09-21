#include "NoProjectProjectItem.h"
#include <QDebug>
#include "ProjectPlugin.h"
#include "UIMain.h"
#include "UINoProjectProjectSettings.h"
#include "pCommand.h"
#include "pConsoleManager.h"

NoProjectProjectItem::NoProjectProjectItem ()
	: ProjectItem()
{
	setType( ProjectsModel::ProjectType );
	setValue( "Project" );
	setFilePath ("toto.noproject");
	targets.append ( (target){"Build","",NULL});
	targets.append ( (target){"Clean","",NULL});
	targets.append ( (target){"Distclean","",NULL});
	targets.append ( (target){"Execute","",NULL});
	//editSettings();
	qWarning ()<<"constructor";
}

NoProjectProjectItem::~NoProjectProjectItem ()
{
	qWarning ()<<"destructor";
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
	QAction* act = dynamic_cast<QAction*>(sender());
	UIMain* main = UIMain::instance();
	if (act == main->buildActions.build)
		makeTarget (aBuild);
	else if (act == main->buildActions.clean)
		makeTarget (aClean);
	else if (act == main->buildActions.reBuild)
	{
		makeTarget (aClean);
		makeTarget (aBuild);
	}
	else if (act == main->buildActions.distClean)
	{
		makeTarget (aDistClean);
		makeTarget (aBuild);
	}
	else if (act == main->buildActions.execute)
		makeTarget (aExecute);
	else if (act == main->buildActions.buildExecute)
	{		makeTarget (aBuild);
			makeTarget (aExecute);
	}
	else
	{
		for( int i = 0; i< targets.size(); i++)
			if (targets[i].action == act)
				makeTarget (i);
	}
};

void NoProjectProjectItem::makeTarget (int targetIndex)
{
	pCommand com;
	com.setCommand (targets[targetIndex].command);
	pConsoleManager::instance()->addCommand (&com);
}

void NoProjectProjectItem::removeSelfFromMenu (QMenu* menu)
{

}

void NoProjectProjectItem::addSelfToMenu (QMenu* menu)
{

}
