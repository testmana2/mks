#include "NavigatorSettings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

NavigatorSettings::NavigatorSettings (Navigator* tcb, int projectMask, int fileMask)
{
/*	classBrouser = tcb;
	names <<"Namespaces"<<	"Classes"<<"Constructors"<<"Destrutors";
	names <<"Enums"<<"Enumerators"<<"Externs"<<"Functions";
	names <<"Macroses"<<"Members"<<"Prototypes"<<"Structures";
	names <<"Typedefs"<<"Unions"<<"Variables"<<	"Uncknown";

	QVBoxLayout* vbox = new QVBoxLayout (this);
	QHBoxLayout* settingsBox = new QHBoxLayout (this);
	vbox->addLayout (settingsBox);
	QVBoxLayout* projectBox = new QVBoxLayout ();
	QVBoxLayout* fileBox = new QVBoxLayout ();
	projectBox->addWidget ( new QLabel ("Display in project tree:"));
	fileBox->addWidget ( new QLabel ("Display in file tree:"));
	for (int i = 0; i < typesCount; i++)
	{
		projectFlags[i] = new QCheckBox(names[i],this);
		projectBox->addWidget (projectFlags[i]);
		projectFlags[i]->setChecked ( projectMask & (1<<i));
		fileFlags[i] = new QCheckBox(names[i],this);
		fileBox->addWidget (fileFlags[i]);
		fileFlags[i]->setChecked ( fileMask & (1<<i));
	}
	settingsBox->addLayout (projectBox);

	settingsBox->addLayout (fileBox);	
	QHBoxLayout* applyBox = new QHBoxLayout (this);
	vbox->addLayout (applyBox);
	QPushButton* applyBtn = new QPushButton (tr("Apply"), this);
	applyBox->addWidget (applyBtn, 0, Qt::AlignRight);
	connect ( applyBtn, SIGNAL ( clicked()), this, SLOT (setSettings()));
*/
}

NavigatorSettings::~NavigatorSettings ()
{
	
}

void NavigatorSettings::setSettings()
{
/*	int projectMask = 0;
	int fileMask = 0;
	for (int i = 0; i < typesCount; i++)
	{
		projectMask |=  (projectFlags[i]->isChecked())<<i;
		fileMask |=  (fileFlags[i]->isChecked())<<i;
	}
	classBrouser->setProjectMask ( projectMask );
	classBrouser->setFileMask ( fileMask );	
*/
}