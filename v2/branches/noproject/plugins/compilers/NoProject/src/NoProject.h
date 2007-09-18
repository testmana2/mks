#ifndef NOPROJECT_H
#define NOPROJECT_H

#include "ProjectPlugin.h"

class NoProject : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	NoProject();
	virtual ~NoProject();
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual void editSettings( ProjectItem* );

};

#endif // QMAKE_H
