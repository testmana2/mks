#ifndef CLASSBROUSERSETTINGS_H
#define CLASSBROUSERSETTINGS_H

#include <QCheckBox>

#include "ClassBrouser.h"
const int typesCount = 16;

class ClassBrouserSettings : public QWidget
{
Q_OBJECT
public:
	ClassBrouserSettings (ClassBrouser* tcb,  int projectMask, int fileMask);
	~ClassBrouserSettings ();	
private:
	QCheckBox*  projectFlags[typesCount];
	QCheckBox*  fileFlags[typesCount];
	QStringList names;
	ClassBrouser* classBrouser;
private slots:
	void setSettings();
};

#endif //CLASSBROUSERSETTINGS_H