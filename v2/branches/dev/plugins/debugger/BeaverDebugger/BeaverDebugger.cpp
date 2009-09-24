/****************************************************************************
**
** Authors   : Andrei KOPATS aka hlamer <hlamer@tut.by>
** Project   : Beaver Debugger plugin
** FileName  : BeaverDebugger.h
** Date      : 
** License   : GPL
** Comment   : 
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Andrei KOPATS & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
/*!
	\file BeaverDebugger.h
	\date 2008-01-14T00:40:08
	\author Andrei KOPATS
	\brief Header file for BeaverDebugger plugin
*/

#include "BeaverDebugger.h"
#include "BeaverDebuggerSettings.h"

#include <QDebug>
#include <QIcon>

#include "pMonkeyStudio.h"

void BeaverDebugger::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Beaver Debugger" );
	mPluginInfos.Description = tr( "Plugin for use Beaver Debugger together with MkS" );
	mPluginInfos.Author = "Andei Kopats aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iDebugger;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = true;
	mPluginInfos.Pixmap = QPixmap( ":/icons/beaverdbg.png" );
}

/*!
	Install plugin to the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool BeaverDebugger::install()
{
	// create action
	MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setVisible( true ); // FIXME is it good?
	MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setEnabled( true ); // FIXME is it good?
	
	if (0) // FIXME debugger found
	{
		mWhyCannot = NULL;
	}
	else // debugger not found
	{
		mWhyCannot = MonkeyCore::menuBar()->action( "mDebugger/aWhyCannot",  
													tr( "Why can't I debug my app" ), 
													QIcon( ":/icons/beaverdbg.png" ), 
													"", // shortcut
													"Check Beaver Debugger status" );
	}
	connect( mWhyCannot, SIGNAL( triggered() ), this, SLOT( explainWhyCannot() ) );
	
	return true;
}

/*!
	Unnstall plugin from the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool BeaverDebugger::uninstall()
{
	if (mWhyCannot)
		delete mWhyCannot;
	
	return true;
}

/*!
	Get settings widget of plugin
	\return Pointer to created settings widget for plugin
*/
QWidget* BeaverDebugger::settingsWidget()
{
	return new BeaverDebuggerSettings(this);
}

QString BeaverDebugger::beaverPath()
{
	if (mBeaverPath.isNull())
		mBeaverPath = "beaverdbg";
	
	return mBeaverPath;
}

void BeaverDebugger::setBeaverPath(const QString& path)
{
	mBeaverPath = path;
}

/*!
	Shows Beaver Debugger detection dialog
*/

void BeaverDebugger::explainWhyCannot() const
{
	bool try_again = true;
	
	while (try_again)
	{
		try_again = false;
		
		TryFindResult res = tryFindBeaver();
		
		QString fullText;
		
		switch (res)
		{
			case OK:
				fullText += "Beaver Debugger found!\n"
							"You can use it now.\n";
			break;
			case NOT_FINISHED:
				fullText += tr("Failed to identify Beaver Debugger. "
							   "System is too slow, or applications works incorrectly.\n");
			break;
			case FAILED_TO_START:
				fullText += tr("Failed to start Beaver Debugger. Executable file not found, "
								"or you have no permissions to execute it.\n\n");
	#ifdef Q_OS_LINUX	
				fullText += tr("Beaver Debugger might be included to your Linux distribution."
							   "Package name probably is 'beaverdbg'.\n");
	#endif
				fullText += tr("For install it using official release, download installer or sources from "
							   "http://beaverdbg.googlecode.com and follow installation instructions.\n");
				fullText += "\n";
				fullText += tr("If Beaver Debugger is installed, but not found, "
							   "go to plugin configuration dialog and configure path to it.\n");
				fullText += "\n";
			break;
			case CRASHED:
				fullText += tr("Beaver Debugger crashed during atempt to start it.\n");
			break;
			case UNKNOWN_ERROR:
				fullText += tr("Unknown error.\n");
			break;
			case NOT_BEAVER:
				fullText += tr("Beaver Debugger executable found, but not identified as Beaver Debugger. "
							   "It might be not a Beaver Debugger, or version is unsupported.\n");
			break;
		}
		
		if (res != OK)
		{
			fullText += "\n";
			fullText += tr("Press Retry for try to detect debugger again, or Open for open configuration dialog");
			
			QMessageBox::StandardButton answer  = 
						QMessageBox::information(NULL,
												tr("Beaver Debugger"),
												fullText,
												QMessageBox::Retry | QMessageBox::Open | QMessageBox::Cancel);
			switch (answer)
			{
				case QMessageBox::Retry:
					try_again = true;
				break;
				case QMessageBox::Open:
					QMessageBox::information(NULL, "Dialog", "Configuration dialog");
					try_again = true;
				break;
				default:
				break;
			}
		}
		else // found, OK
		{
			QMessageBox::information(NULL,
									 tr("Beaver Debugger"),
									 fullText,
									 QMessageBox::Ok);
		
		}
	}
}

BeaverDebugger::TryFindResult BeaverDebugger::tryFindBeaver() const
{
	QProcess beaver(NULL);
	beaver.start("beaverdbg", QStringList() << "--version");
	beaver.waitForFinished(200);
	if (beaver.state() != QProcess::NotRunning) // hmm, strange
	{
		beaver.close();
		return NOT_FINISHED;
	}
	
	switch (beaver.error())
	{
		case QProcess::FailedToStart:
			return FAILED_TO_START;
		case QProcess::Crashed:
			return CRASHED;
		case QProcess::UnknownError: // It's OK
		break;
		default:
			return UNKNOWN_ERROR;
	}
	
#if 0
    do something
		return NOT_BEAVER;
#endif
	
	return OK;
}

Q_EXPORT_PLUGIN2( BaseBeaverDebugger, BeaverDebugger )