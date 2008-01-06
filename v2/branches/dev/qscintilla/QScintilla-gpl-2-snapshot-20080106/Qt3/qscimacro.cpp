// This module implements the QsciMacro class.
//
// Copyright (c) 2008
// 	Phil Thompson <phil@river-bank.demon.co.uk>
// 
// This file is part of QScintilla.
// 
// This copy of QScintilla is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option) any
// later version.
// 
// QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// QScintilla; see the file LICENSE.  If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include "Qsci/qscimacro.h"

#include <qstringlist.h>

#include "Qsci/qsciscintilla.h"


static int fromHex(unsigned char ch);



// The ctor.
QsciMacro::QsciMacro(QsciScintilla *parent, const char *name)
    : QObject(parent, name), qsci(parent)
{
}


// The ctor that initialises the macro.
QsciMacro::QsciMacro(const QString &asc, QsciScintilla *parent,
        const char *name)
    : QObject(parent, name), qsci(parent)
{
    load(asc);
}


// The dtor.
QsciMacro::~QsciMacro()
{
}


// Clear the contents of the macro.
void QsciMacro::clear()
{
    macro.clear();
}


// Read a macro from a string.
bool QsciMacro::load(const QString &asc)
{
    bool ok = true;

    macro.clear();

    QStringList fields = QStringList::split(' ', asc);

    int f = 0;

    while (f < fields.size())
    {
        Macro cmd;
        unsigned len;

        // Extract the 3 fixed fields.
        if (f + 3 > fields.size())
        {
            ok = false;
            break;
        }

        cmd.msg = fields[f++].toUInt(&ok);

        if (!ok)
            break;

        cmd.wParam = fields[f++].toULong(&ok);

        if (!ok)
            break;

        len = fields[f++].toUInt(&ok);

        if (!ok)
            break;

        // Extract any text.
        if (len)
        {
            if (f + 1 > fields.size())
            {
                ok = false;
                break;
            }

            cmd.text.resize(len);

            const char *sp = fields[f++].ascii();

            char *dp = cmd.text.data();

            if (!sp)
            {
                ok = false;
                break;
            }

            while (len--)
            {
                unsigned char ch;

                ch = *sp++;

                if (ch == '"' || ch <= ' ' || ch >= 0x7f)
                {
                    ok = false;
                    break;
                }

                if (ch == '\\')
                {
                    int b1, b2;

                    if ((b1 = fromHex(*sp++)) < 0 ||
                        (b2 = fromHex(*sp++)) < 0)
                    {
                        ok = false;
                        break;
                    }

                    ch = (b1 << 4) + b2;
                }

                *dp++ = ch;
            }

            if (!ok)
                break;
        }

        macro.append(cmd);
    }
        
    if (!ok)
        macro.clear();

    return ok;
}


// Write a macro to a string.
QString QsciMacro::save() const
{
    QString ms;

    QValueList<Macro>::const_iterator it;

    for (it = macro.begin(); it != macro.end(); ++it)
    {
        if (!ms.isEmpty())
            ms += ' ';

        unsigned len = (*it).text.size();
        QString m;

        ms += m.sprintf("%u %lu %u", (*it).msg, (*it).wParam, len);

        if (len)
        {

            ms += ' ';

            const char *cp = (*it).text.data();

            while (len--)
            {
                unsigned char ch = *cp++;

                if (ch == '\\' || ch == '"' || ch <= ' ' || ch >= 0x7f)
                {
                    QString buf;

                    ms += buf.sprintf("\\%02x", ch);
                }
                else
                    ms += ch;
            }
        }
    }

    return ms;
}


// Play the macro.
void QsciMacro::play()
{
    if (!qsci)
        return;

    QValueList<Macro>::const_iterator it;

    for (it = macro.begin(); it != macro.end(); ++it)
        qsci->SendScintilla((*it).msg, (*it).wParam, (*it).text.data());
}


// Start recording.
void QsciMacro::startRecording()
{
    if (!qsci)
        return;

    macro.clear();

    connect(qsci, SIGNAL(SCN_MACRORECORD(unsigned int, unsigned long, long)),
            SLOT(record(unsigned int, unsigned long, long)));

    qsci->SendScintilla(QsciScintillaBase::SCI_STARTRECORD);
}


// End recording.
void QsciMacro::endRecording()
{
    if (!qsci)
        return;

    qsci->SendScintilla(QsciScintillaBase::SCI_STOPRECORD);
    qsci->disconnect(this);
}


// Record a command.
void QsciMacro::record(unsigned int msg, unsigned long wParam, long lParam)
{
    Macro m;

    m.msg = msg;
    m.wParam = wParam;

    // Determine commands which need special handling of the parameters.
    switch (msg)
    {
    case QsciScintillaBase::SCI_ADDTEXT:
        m.text.duplicate(reinterpret_cast<const char *>(lParam), wParam);
        break;

    case QsciScintillaBase::SCI_REPLACESEL:
        if (!macro.isEmpty() && macro.last().msg == QsciScintillaBase::SCI_REPLACESEL)
        {
            // This is the command used for ordinary user input so it's a
            // significant space reduction to append it to the previous
            // command.

            macro.last().text.append(reinterpret_cast<const char *>(lParam));
            return;
        }

        /* Drop through. */

    case QsciScintillaBase::SCI_INSERTTEXT:
    case QsciScintillaBase::SCI_APPENDTEXT:
    case QsciScintillaBase::SCI_SEARCHNEXT:
    case QsciScintillaBase::SCI_SEARCHPREV:
        m.text.append(reinterpret_cast<const char *>(lParam));
        break;
    }

    macro.append(m);
}


// Return the given hex character as a binary.
static int fromHex(unsigned char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    return -1;
}
