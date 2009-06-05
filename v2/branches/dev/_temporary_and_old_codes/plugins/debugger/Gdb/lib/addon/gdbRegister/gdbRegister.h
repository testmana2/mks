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
	\file gdbRegister.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for show values of cpu registers.This class is an AddOn for GNU debugger
*/
/*

	GdbResgiter class


	Xiantia@gmail.com

	for Debugger v1.4.0
*/

#ifndef GDBREGISTER_H
#define GDBREGISTER_H

#include <QObject>
#include <QTreeWidgetItem>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbAddonBase.h"
#include "../../kernel/gdbSequencer.h"
#include "../../kernel/gdbProcess.h"
#include "../../kernel/gdbKernelDispatcher.h"
class UIGdbRegister;



/*!
	\brief Implements all functions for show values of cpu registers.This class is an AddOn for GNU debugger
 	\details This class show in TabWidget all values of all cpu registers.

*/
class GdbRegister : public GdbAddonBase
{
	Q_OBJECT


public:

	GdbRegister(QObject * parent = 0);
	~GdbRegister();

public slots:

	void onRegister( int , QString );

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	/*!
		\sa GdbInterpreter
	*/
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	// gdb
	void gdbFinished();
	void gdbStarted();
	void gdbError();

	// target
	void targetLoaded(const int &, const QString &);
	void targetNoLoaded(const int &, const QString &);
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int &, const QString &);

	// Parser
	void done(const int &, const QString &);
	void prompt(const int &, const QString &);


private:

	/*!
	  \sa GdbConnectTemplate
	 */
	GdbConnectTemplate<GdbRegister> Connect;
	/*!
		\sa BaseInterpreter, GdbInterpreter
	 */
	QPointer<BaseInterpreter> interpreterRegister;

	/*!
	 \sa GdbSequencer
	 */
	QPointer<GdbSequencer> Sequencer;

	QPointer<UIGdbRegister> mWidget;

	int numRegister;

	void showColor(QTreeWidgetItem *, QString );
};

#endif
