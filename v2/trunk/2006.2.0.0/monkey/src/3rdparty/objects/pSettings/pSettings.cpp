/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : lundi 30 janvier 2006 - 17h48
 * AUTHOR       : Nox P@sNox ( pasnox@hotmail.com )
 * FILENAME     : pSettings.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pSettings.h"
//
#include <QDir>
#include <QFile>
#include <QMainWindow>
//
QPointer<pSettings> pSettings::mSelf = 0L;
//
pSettings* pSettings::current( const QString& pName, const QString& pVersion )
{
	if ( !mSelf )
		mSelf = new pSettings( pName, pVersion );
	return mSelf;
}
//
pSettings::pSettings( const QString& pName, const QString& pVersion )
	: QSettings( QDir::convertSeparators( QString( "%1/.%2/%3.ini" ).arg( QDir::homePath(), pName, pName ) ), QSettings::IniFormat )
{
	mProgramName = pName;
	mProgramVersion = pVersion;
	beginGroup( mProgramVersion );
}
//
pSettings::~pSettings()
{
	endGroup();
}
//
QString pSettings::programName() const
{
	return mProgramName;
}
//
QString pSettings::programVersion() const
{
	return mProgramVersion;
}
//
void pSettings::restoreState( QMainWindow* w )
{
	if ( !value( "MainWindow/Maximized", true ).toBool() )
		w->setGeometry( value( "MainWindow/Geometry" ).toRect() );
	else
		w->showMaximized();
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
}
//
void pSettings::saveState( QMainWindow* w )
{
	setValue( "MainWindow/Maximized", w->isMaximized() );
	setValue( "MainWindow/Geometry", w->geometry() );
	setValue( "MainWindow/State", w->saveState() );
}
//
void pSettings::setDefaultSettings()
{
	// must be reimplemented
}
