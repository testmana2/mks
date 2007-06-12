#include "pSettings.h"

#include <QDir>
#include <QFile>
#include <QMainWindow>

pSettings::pSettings( const QString& pName, const QString& pVersion, QObject* o )
	: QSettings( QDir::convertSeparators( QString( "%1/.%2/%3.ini" ).arg( QDir::homePath(), pName, pName ) ), QSettings::IniFormat, o )
{
	mProgramName = pName;
	mProgramVersion = pVersion;
	beginGroup( mProgramVersion );
}

pSettings::~pSettings()
{
	endGroup();
}

QString pSettings::programName() const
{
	return mProgramName;
}

QString pSettings::programVersion() const
{
	return mProgramVersion;
}

void pSettings::restoreState( QMainWindow* w )
{
	if ( !value( "MainWindow/Maximized", true ).toBool() )
		w->setGeometry( value( "MainWindow/Geometry" ).toRect() );
	else
		w->showMaximized();
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
}

void pSettings::saveState( QMainWindow* w )
{
	setValue( "MainWindow/Maximized", w->isMaximized() );
	setValue( "MainWindow/Geometry", w->geometry() );
	setValue( "MainWindow/State", w->saveState() );
}
