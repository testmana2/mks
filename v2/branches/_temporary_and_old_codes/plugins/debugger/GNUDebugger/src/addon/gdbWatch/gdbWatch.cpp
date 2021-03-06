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
    \file gdbWatch.cpp
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief  This class implements all functions for see user values. This class is an AddOn for GNU debugger 
*/
/*

    GdbBacktrace class


    Xiantia@gmail.com

    for Debugger v1.3.0
*/

#include "gdbWatch.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QToolTip>

/*!
    \details Create new object
    \param parent of this object
*/
GdbWatch::GdbWatch(QObject * parent) : GdbCore(parent)
{
    // new connexion 

    setEnabled(true);
    setWaitEndProcess(false);

    GdbCore::Parser()->addRestoreLine(name(),
        "^\\$\\d+\\s+=\\s+.*" , 
        ".*\\(gdb\\)\\s*$");


    interpreterWhatis = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^whatis .*"),
        QRegExp("^type\\s+=\\s+.*"),
        "^info,interpreter=\"" + name() + "\",event=\"WatchType\",answerGdb=\"");

    interpreterAddress = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^p .*"), // use p and not print command because interpreterValue use this
        QRegExp("^\\$\\d+\\s+=\\s+.*"),
        "^info,interpreter=\"" + name() + "\",event=\"WatchAddress\",answerGdb=\"");

    interpreterValue = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^print .*"), // use print and not p because interpreterAddress use this
        QRegExp("^\\$\\d+\\s+=\\s+.*"),
        "^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");

    // connect interpreters to functions
    Connect.add(this, interpreterWhatis, &GdbWatch::onType);
    Connect.add(this, interpreterValue, &GdbWatch::onValue);
    Connect.add(this, interpreterAddress, &GdbWatch::onAddress);

    // create sequencer 
    Sequencer = new GdbSequencer(this);

    // create Ui
    mWidget = UIGdbWatch::self();
    mWidget->treeWidget->setAlternatingRowColors(true);

    connect(mWidget, SIGNAL(userAddVar(QString)), this , SLOT(onUserAddVar(QString)));

}

//
/*!
    \details Delete container.
*/
GdbWatch::~GdbWatch()
{
    delete mWidget;
}

//

QString GdbWatch::name()
{
    return "GdbWatch";
}

//

QPointer<QWidget> GdbWatch::widget()
{
    return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbWatch::icon()
{
    return QIcon(":/icons/watch.png");
}

//

void GdbWatch::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
    Connect.call( i, id, s);
}

// Gdb status

void GdbWatch::gdbStarted()
{
    setWaitEndProcess(false);
    mWidget->treeWidget->clear();
}

//

void GdbWatch::gdbFinished()
{
    setWaitEndProcess(false);
    mWidget->treeWidget->clear();
}

//

void GdbWatch::gdbError(){}

// Taget status

void GdbWatch::targetLoaded(const int &, const QString &){}
void GdbWatch::targetNoLoaded(const int &, const QString &){}
void GdbWatch::targetRunning(const int & id, const QString & s){}

//

void GdbWatch::targetStopped(const int & id, const QString & s)
{
//  setWaitEndProcess(true);
    currentVar = 0;
    createSequencer();
    Sequencer->start();
}

//

void GdbWatch::targetExited(const int & id, const QString & s)
{
    mWidget->treeWidget->clear();
}

// Parser status

void GdbWatch::error(const int &, const QString & s)
{
    currentVar++;
    Sequencer->start(currentVar * 3);
}

//

void GdbWatch::done(const int &, const QString & s){}
void GdbWatch::info(const int &, const QString &){}

//

void GdbWatch::prompt(const int &, const QString & s)
{
    setWaitEndProcess(false);
}

// Interpreters
/*!
    \details Calling when Gdb print the type of the var.
    \param s is the string from GdbParser.
*/
void GdbWatch::onType(int , QString s)
{
    QRegExp r("^type\\s+=\\s+(.*)$");

    if(r.exactMatch(findValue(s,"answerGdb")))
    {
        QStringList l = r.capturedTexts();
        
        QTreeWidgetItem * i;

        if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
        {
            i = mWidget->treeWidget->topLevelItem ( currentVar);
            showColor(i, 1, l.at(1));

        }
        else i = new QTreeWidgetItem(mWidget->treeWidget);

        i->setText(1,l.at(1));

        // get if is a pointer
        if( isPointer(i->text(0)) )
            Sequencer->change("printAdress "+ i->text(0),"p "+ i->text(0).remove("*"));
    }
    // execute next command if have
    Sequencer->loop(); // adress
}

//

/*!
    \details Slot when GdbScript has finished translate Qt class
    \param s is the value.
*/
void GdbWatch::onScriptFinishedTranslate(const QString & s)
{
    Sequencer->skipLoop();
    onValue(0, "answerGdb=\"$1 = " + s + "\"");
}

//
/*!
    \details Calling when Gdb print the value of the var.
    \param s is the string from GdbParser.
*/
void GdbWatch::onValue(int, QString s)
{
    QRegExp r("^\\$\\d+\\s+=\\s+(.*)$");

    if(r.exactMatch(findValue(s,"answerGdb")))
    {
        QStringList l = r.capturedTexts();
        
        QTreeWidgetItem * i;

        if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
        {
            i = mWidget->treeWidget->topLevelItem ( currentVar);
            showColor(i, 4, l.at(1));
        }
        else i = new QTreeWidgetItem(mWidget->treeWidget);

        i->setText(4,l.at(1));

        // decompil struct 
        WatchStruct w;
        w.decompilStrut(i,l.at(1)); 
    }
    currentVar++;
    Sequencer->loop();
}

//
/*!
    \details Calling when Gdb print adress of the var.
    \param s is the string from GdbParser.
*/
void GdbWatch::onAddress(int , QString s)
{
    QRegExp r("^\\$\\d+\\s+=\\s+(.*)$");

    if(r.exactMatch(findValue(s,"answerGdb")))
    {
        QStringList l = r.capturedTexts();
        
        QTreeWidgetItem * i;

        if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
        {
            i = mWidget->treeWidget->topLevelItem ( currentVar);
            showColor(i, 2, l.at(1));
        }
        else i = new QTreeWidgetItem(mWidget->treeWidget);

        i->setText(2,l.at(1));

        // use script
        if(GdbScript::tryTranslate(i->text(1)))
        {
            i->setText(4,tr("Waitting translate .."));
            emit requestScriptTranslate(i->text(1),i->text(0)); 
            return; // get value from script
        }
    }
    Sequencer->loop(); // value
}

// Tools
/*!
    \details toggle color when the value has changed
    \param p is the pointer to QTreeWidgetItem
    \param index is the index of p
    \param a is the new value
*/
void GdbWatch::showColor(QTreeWidgetItem *p, int index, QString a)
{
    // toggle color (black / red if value in treeWidget is not egal than new value
    if(p->text(index) != a)
        p->setForeground( index, QBrush(Qt::red));
    else
        p->setForeground( index , QBrush(Qt::black));
}

/*!
    \details Create sequencer with the var present in Ui
*/
void GdbWatch::createSequencer()
{
    QStringList l = mWidget->getAllvar();
    QList<SequencerCmd> s;

    for(int i=0; i < l.count(); i++)
        // optimize, do not send whatis (no change)
        s << /*SequencerCmd("printType "+l.at(i), "whatis " + l.at(i)) <<*/ SequencerCmd("printAdress "+l.at(i), "p &" + l.at(i)) << SequencerCmd("printValue "+l.at(i), "print "+ l.at(i)) ; 
    Sequencer->add(name() , s);
}

// when user drag and drop var in treewidget
/*!
    \details User drag and drop the new var in Ui
    \param n is the name of var
*/
void GdbWatch::onUserAddVar(QString n)
{
    QList<SequencerCmd> s;
    s << SequencerCmd("printType", "whatis " + n) << SequencerCmd("printAdress", "p &" + n) << SequencerCmd("printValue", "print " + n) ; 
    Sequencer->add(name() , s);
    currentVar = mWidget->treeWidget->topLevelItemCount() - 1;
    Sequencer->start();
}

//
/*!
    \details Get if var is a pointer or not
    \param s is the name of var
    \retval  true if var is a pointer

*/
bool GdbWatch::isPointer(const QString & s)
{
    if(s.contains("*")) return true;
    return false;
}
