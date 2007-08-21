/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : lundi 30 janvier 2006 - 17h47
 * AUTHOR       : Nox P@sNox ( pasnox@hotmail.com )
 * FILENAME     : pSettings.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PSETTINGS_H
#define PSETTINGS_H
//
#include <QSettings>
#include <QPointer>
#include "MonkeyExport.h"
//
class QMainWindow;
//
class Q_MONKEY_EXPORT pSettings : public QSettings
{
	Q_OBJECT
	//
public:
	static pSettings* current( const QString&, const QString& );
	pSettings( const QString&, const QString& );
	~pSettings();
	//
	QString programName() const;
	QString programVersion() const;
	//
	virtual void restoreState( QMainWindow* );
	virtual void saveState( QMainWindow* );
	//
	virtual void setDefaultSettings();
	//
private:
	QString mProgramName;
	QString mProgramVersion;
	//
	static QPointer<pSettings> mSelf;
	//
};
//
#endif // PSETTINGS_H
