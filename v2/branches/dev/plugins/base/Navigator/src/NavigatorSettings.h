#ifndef CLASSBROUSERSETTINGS_H
#define CLASSBROUSERSETTINGS_H

#include <QCheckBox>

#include "Navigator.h"

const int typesCount = 16;

class NavigatorSettings : public QWidget
{
Q_OBJECT
public:
	NavigatorSettings (Navigator* tcb,  int projectMask, int fileMask);
	~NavigatorSettings ();	
private:
	QCheckBox*  projectFlags[typesCount];
	QCheckBox*  fileFlags[typesCount];
	QStringList names;
	Navigator* classBrouser;
private slots:
	void setSettings();
};

#endif //CLASSBROUSERSETTINGS_H