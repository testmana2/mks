/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbCli
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
/*!
	\file gdbCli.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
 	\brief Implements all functions for send user command to Gdb.This class is an AddOn for GNU debugger
*/

/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbCli.h"
#include <QMessageBox>
#include <QFileInfo>

/*!
	\details Create new object
	\param parent of this object
*/
GdbCli::GdbCli(QObject * parent) : GdbCore(parent)
{
	setEnabled(true);
	setWaitEndProcess(false);

	mWidget = UIGdbCli::self(0);
	connect(mWidget->bSend, SIGNAL(clicked()), this , SLOT(onSend()));
}

//

/*!
	\details Delete container
*/
GdbCli::~GdbCli()
{
	delete mWidget;
}

//

QString GdbCli::name()
{
	return "GdbCli";
}

//

QPointer<QWidget> GdbCli::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbCli::icon()
{
	return QIcon(":/icons/cli.png");
}


void GdbCli::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
}

// Gdb status

void GdbCli::gdbStarted(){}

//

void GdbCli::gdbFinished(){}

//

void GdbCli::gdbError(){
	mWidget->bSend->setEnabled(false);
}

// Taget status

void GdbCli::targetLoaded(const int &, const QString &){
	mWidget->bSend->setEnabled(true);
}

//

void GdbCli::targetNoLoaded(const int &, const QString &){
	mWidget->bSend->setEnabled(false);
}

//

void GdbCli::targetRunning(const int & id, const QString & s){
	mWidget->bSend->setEnabled(false);
}

//

void GdbCli::targetStopped(const int & id, const QString & s){
	mWidget->bSend->setEnabled(true);
}

//

void GdbCli::targetExited(const int & id, const QString & s){
	mWidget->bSend->setEnabled(false);
}

// Parser status

void GdbCli::error(const int &, const QString & s){
	mWidget->textEdit->append(findValue(s, "answerGdb"));
}

void GdbCli::done(const int &, const QString &s){
	mWidget->textEdit->append(findValue(s, "answerGdb"));
}

void GdbCli::info(const int &, const QString &s){
	mWidget->textEdit->append(findValue(s, "answerGdb"));
}

void GdbCli::prompt(const int &, const QString & s){
	mWidget->textEdit->append(findValue(s, "answerGdb"));
	setWaitEndProcess(false);
}

/*!
	\details Calling when user click on "Send" button. This function send user command to Gdb.
*/
void GdbCli::onSend()
{
	QString s = mWidget->lineEdit->text();
	GdbCore::Parser()->setNextCommand(name() , s);
	GdbCore::Process()->sendRawData(s);
	setWaitEndProcess(true);
}
