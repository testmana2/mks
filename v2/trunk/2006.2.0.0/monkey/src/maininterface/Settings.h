/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : lundi 30 janvier 2006 - 17h47
 * AUTHOR       : Nox P@sNox ( pasnox@hotmail.com )
 * FILENAME     : Settings.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H
//
#include "pSettings.h"
#include "MonkeyExport.h"
//
#include <QStringList>
//
class Q_MONKEY_EXPORT Settings : public pSettings
{
	Q_OBJECT
	//
public:
	static Settings* current();
	//
	virtual void setDefaultSettings();
	//
	QStringList languages() const;
	//
private:
	Settings();
	//
	static QPointer<Settings> mSelf;
	//
};
//
#endif // PSETTINGS_H
