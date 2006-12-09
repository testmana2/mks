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
//
class Settings : public pSettings
{
	Q_OBJECT
	//
public:
	static Settings* current();
	virtual void setDefaultSettings();
	//
	int maxRecentFiles() const;
	int maxRecentProjects() const;
	QStringList languages() const;
	//
private:
	Settings();
	//
	static QPointer<Settings> mSelf;
	//
public slots:
	void setMaxRecentFiles( int );
	void setMaxRecentProjects( int );
	//
signals:
	void maxRecentFilesChanged( int );
	void maxRecentProjectsChanged( int );
	//
};
//
#endif // PSETTINGS_H
