/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
****************************************************************************/
/*!
	\file gdbCore.1.3.h
	\date 2008-01-14T00:27:39
	\author xiantia
	\version 1.3.2
	\brief Derived by all Addon
*/

#ifndef GDBCORE_H
#define GDBCORE_H

#include <QTimer>

#include "gdbConnectTemplate.h"
#include "gdbInterpreter.h"
#include "gdbParser.h"
#include "gdbProcess.h"


#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

/*!
\details Enum for show message to the status bar
*/
enum SHOW{ _WARNING_,  _CRITICAL_, _INFO_};


/*!
 \brief Derived by all AddOn
 \details It is the basic class proposing the basic functions.
 this class gathers the basic functions. These function cuts out into two big family.
 Events generated by the target (targetStopped, targetRunning, targetExited,…) and 
 those by Gdb (gdbStarted, gdbFinished, gdbExited,…)
*/

class GdbCore : public QObject
{
	Q_OBJECT

public:

	GdbCore(QObject * parent = 0);
	~GdbCore();

	/*!
	 * \details Get the GdbParser class
	 * \retval Pointer to GdbParser class
	 */
	QPointer<GdbParser> Parser() {  return mParser;}
	
	/*!
	 * \details Get the GdbProcess class
	 * \retval Pointer to GdbProcess class
	 */
	QPointer<GdbProcess> Process() {  return mProcess;}


public slots:

	/*!
	 * \details Calling when Gdb is started 
	 */
	virtual void gdbStarted() = 0;


	/*!
	 * \details Calling when Gdb is finished
	 */
	virtual void gdbFinished() = 0;

	/*!
	 * \details Calling when Gdb has generate an error. Gdb crash for example.
	 */
	virtual void gdbError() = 0;

	/*!
	 * \details Calling when target is loaded correctly 
	 */
	virtual void targetLoaded(const int & , const QString & ) = 0;

	/*!
	 * \details Calling when target is not loaded (format of target not correct).
 	 */
	virtual void targetNoLoaded(const int &, const QString &) = 0;

	/*!
	 * \details Calling when target running.
 	 */
	virtual void targetRunning(const int &, const QString &) = 0;
	
	/*!
	 * \details Calling when target is stopped. Reason of stopped can be breakpoint or step over command.
 	 */
	virtual void targetStopped(const int &, const QString &) = 0;
	
	/*!
	 * \details Calling when target exited.
 	 */
	virtual void targetExited(const int &, const QString &) = 0;

	/*!
	 * \details Calling when GdbParser class found an error.
 	 */
 	virtual void error(const int &, const QString &)= 0;
	
	/*!
	 * \details Calling when GdbParser class can't found string in parse file
 	 */
	virtual void done(const int &, const QString &) = 0;

	/*!
	 * \details Calling when GdbParser class found an information. Generaly this function is call when GdbParser 
	 * class has found string in parse file.
 	 */
	virtual void info(const int &, const QString &) = 0;

	/*!
	 * \details Calling when GdbParser class found prompt from Gdb.
 	 */
	virtual void prompt(const int &, const QString &) = 0;

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Name of your AddOn, it's can show in GNU debugger TabWidget
	*/
	virtual QString name() = 0; 

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Widget container of AddOn, it's can show in GNU debugger TabWidget
	 * \note If AddOn not have widget container, this funtion can return NULL.
	*/
	virtual QPointer<QWidget> widget() = 0;

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Icon of your AddOn, it's can show in GNU debugger TabWidget
	 * \note If widget() function return NULL, this function is not call.
	*/
	virtual QIcon icon() = 0;
	virtual void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ) = 0;
	void setEnabled(const bool &  );
	bool isEnabled() ;
	static QString findValue(const QString & , const QString &);
	static void showMessage(QString, int, SHOW);
	void setWatchDogTime(int);

	/*!
	 * \details Return the current state wantAllMessages.
	 */
	bool wantAllMessages(){ return mWantAllMessages;}
	
	/*!
	 * \details Set Addon want all datas from all AddOn
	 *
	 * By default AddOn does not receive that the data in answer has your command. 
	 * You can receive the data of other AddOn by specifying it by this function. 
	 * That can be interesting if you want create a log.
	 *
	 * Default is false
	 * \param b is true if you want receive all data from Gdb
	 */
	void setWantAllMessages(bool b){ mWantAllMessages = b;}

protected :

	void setWaitEndProcess(const bool & );
	bool isWaitEndProcess();


private slots:

	void onTimer();
	
private : 
	/*!
		\details Private var, pointer to GdbParser, use Parser() function.
	*/
	QPointer<GdbParser> mParser;

	/*!
		\details Private var, pointer to GdbProcess, use Process() function.
	*/
	QPointer<GdbProcess> mProcess;
	
	/*!
		\details Private var, AddOn is enbaled, use isEnabled()function.
	*/
	bool mEnabled;

	/*!
		\details Private var, AddOn is waiting answer, use isWaitEndProcess() function.
	*/
	bool mWaitEndProcess;
	QTimer watchDog;

	/*!
		\details Private var, AddOn wan all data from Gdb, use wantAllMessages() function.
	*/
	bool mWantAllMessages;

	/*!
		\details time for watchDog
	*/
	int mTime;
};

#endif
