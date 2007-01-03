// This module implements the QsciLexer class.
//
// Copyright (c) 2006
// 	Riverbank Computing Limited <info@riverbankcomputing.co.uk>
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


#include "Qsci/qscilexer.h"

#include <qapplication.h>
#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

#include "Qsci/qsciapis.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qsciscintillabase.h"


// The ctor.
QsciLexer::QsciLexer(QObject *parent, const char *name)
    : QObject(parent, name),
      autoIndStyle(-1), apiSet(0)
{
#if defined(Q_OS_WIN)
    defFont = QFont("Verdana",10);
#else
    defFont = QFont("Bitstream Vera Sans",9);
#endif

    // Set the default fore and background colours.
    QColorGroup cg = QApplication::palette().active();
    defColor = cg.text();
    defPaper = cg.base();
}


// Set the APIs associated with the lexer.
void QsciLexer::setAPIs(QsciAPIs *apis)
{
    apiSet = apis;
}


// Return a pointer to the current APIs if there are any.
QsciAPIs *QsciLexer::apis() const
{
    return apiSet;
}


// Default implementation to return the set of fill up characters that can end
// auto-completion.
const char *QsciLexer::autoCompletionFillups() const
{
    return "(";
}


// Default implementation to return the list of character sequences that can
// separate auto-completion words.
QStringList QsciLexer::autoCompletionWordSeparators() const
{
    return QStringList();
}


// Default implementation to return the list of keywords that can start a
// block.
const char *QsciLexer::blockStartKeyword(int *) const
{
    return 0;
}


// Default implementation to return the list of characters that can start a
// block.
const char *QsciLexer::blockStart(int *) const
{
    return 0;
}


// Default implementation to return the list of characters that can end a
// block.
const char *QsciLexer::blockEnd(int *) const
{
    return 0;
}


// Default implementation to return the style used for braces.
int QsciLexer::braceStyle() const
{
    return -1;
}


// Default implementation to return the number of lines to look back when
// auto-indenting.
int QsciLexer::blockLookback() const
{
    return 20;
}


// Default implementation to return the case sensitivity of the language.
bool QsciLexer::caseSensitive() const
{
    return true;
}


// Default implementation to return the characters that make up a word.
const char *QsciLexer::wordCharacters() const
{
    return 0;
}


// Default implementation to return the style used for whitespace.
int QsciLexer::defaultStyle() const
{
    return 0;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexer::color(int) const
{
    return defaultColor();
}


// Returns the end-of-line fill for a style.
bool QsciLexer::eolFill(int) const
{
    return false;
}


// Returns the font for a style.
QFont QsciLexer::font(int) const
{
    return defaultFont();
}


// Returns the set of keywords.
const char *QsciLexer::keywords(int) const
{
    return 0;
}


// Returns the background colour of the text for a style.
QColor QsciLexer::paper(int) const
{
    return defaultPaper();
}


// Returns the default font for all styles.
QFont QsciLexer::defaultFont() const
{
    return defFont;
}


// Sets the default font for all styles.
void QsciLexer::setDefaultFont(const QFont &f)
{
    defFont = f;
}


// Returns the default text colour for all styles.
QColor QsciLexer::defaultColor() const
{
    return defColor;
}


// Sets the default text colour for all styles.
void QsciLexer::setDefaultColor(const QColor &c)
{
    defColor = c;
}


// Returns the default paper colour for all styles.
QColor QsciLexer::defaultPaper() const
{
    return defPaper;
}


// Sets the default paper colour for all styles.
void QsciLexer::setDefaultPaper(const QColor &c)
{
    defPaper = c;
}


// Read properties from the settings.
bool QsciLexer::readProperties(QSettings &,const QString &)
{
    return true;
}


// Refresh all properties.
void QsciLexer::refreshProperties()
{
}


// Write properties to the settings.
bool QsciLexer::writeProperties(QSettings &,const QString &) const
{
    return true;
}


// Restore the user settings.
bool QsciLexer::readSettings(QSettings &qs,const char *prefix)
{
    bool ok, flag, rc = true;
    int num;
    QString key;

    // Read the styles.
    for (int i = 0; i < 128; ++i)
    {
        // Ignore invalid styles.
        if (description(i).isEmpty())
            continue;

        key.sprintf("%s/%s/style%d/",prefix,language(),i);

        // Read the foreground colour.
        num = qs.readNumEntry(key + "color", 0, &ok);

        if (ok)
            emit colorChanged(QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i);
        else
            rc = false;

        // Read the end-of-line fill.
        flag = qs.readBoolEntry(key + "eolfill", 0, &ok);

        if (ok)
            emit eolFillChanged(flag, i);
        else
            rc = false;

        // Read the font
        QStringList fdesc;

        fdesc = qs.readListEntry(key + "font", ',', &ok);

        if (ok && fdesc.count() == 5)
        {
            QFont f;

            f.setFamily(fdesc[0]);
            f.setPointSize(fdesc[1].toInt());
            f.setBold(fdesc[2].toInt());
            f.setItalic(fdesc[3].toInt());
            f.setUnderline(fdesc[4].toInt());

            emit fontChanged(f, i);
        }
        else
            rc = false;

        // Read the background colour.
        num = qs.readNumEntry(key + "paper", 0, &ok);

        if (ok)
            emit paperChanged(QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i);
        else
            rc = false;
    }

    // Read the properties.
    key.sprintf("%s/%s/properties/",prefix,language());

    if (!readProperties(qs,key))
        rc = false;

    refreshProperties();

    // Read the rest.
    key.sprintf("%s/%s/",prefix,language());

    num = qs.readNumEntry(key + "autoindentstyle", 0, &ok);

    if (ok)
        autoIndStyle = num;
    else
        rc = false;

    return rc;
}


// Save the user settings.
bool QsciLexer::writeSettings(QSettings &qs,const char *prefix) const
{
    bool rc = true;
    QString key;

    // Write the styles.
    for (int i = 0; i < 128; ++i)
    {
        // Ignore invalid styles.
        if (description(i).isEmpty())
            continue;

        int num;
        QColor c;

        key.sprintf("%s/%s/style%d/",prefix,language(),i);

        // Write the foreground colour.
        c = color(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        if (!qs.writeEntry(key + "color", num))
            rc = false;

        // Write the end-of-line fill.
        if (!qs.writeEntry(key + "eolfill", eolFill(i)))
            rc = false;

        // Write the font
        QStringList fdesc;
        QString fmt("%1");
        QFont f;

        f = font(i);

        fdesc += f.family();
        fdesc += fmt.arg(f.pointSize());

        // The casts are for Borland.
        fdesc += fmt.arg((int)f.bold());
        fdesc += fmt.arg((int)f.italic());
        fdesc += fmt.arg((int)f.underline());

        if (!qs.writeEntry(key + "font", fdesc, ','))
            rc = false;

        // Write the background colour.
        c = paper(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        if (!qs.writeEntry(key + "paper", num))
            rc = false;
    }

    // Write the properties.
    key.sprintf("%s/%s/properties/",prefix,language());

    if (!writeProperties(qs,key))
        rc = false;

    // Write the rest.
    key.sprintf("%s/%s/",prefix,language());

    if (!qs.writeEntry(key + "autoindentstyle", autoIndStyle))
        rc = false;

    return rc;
}


// Return the auto-indentation style.
int QsciLexer::autoIndentStyle()
{
    // We can't do this in the ctor because we want the virtuals to work.
    if (autoIndStyle < 0)
        autoIndStyle = (blockStartKeyword() || blockStart() || blockEnd()) ?
                    0 : QsciScintilla::AiMaintain;

    return autoIndStyle;
}


// Set the auto-indentation style.
void QsciLexer::setAutoIndentStyle(int autoindentstyle)
{
    autoIndStyle = autoindentstyle;
}


// Set the foreground colour for a style.
void QsciLexer::setColor(const QColor &c, int style)
{
    if (style >= 0)
        emit colorChanged(c, style);
    else
        for (int i = 0; i < 128; ++i)
            if (!description(i).isEmpty())
                emit colorChanged(c, i);
}


// Set the end-of-line fill for a style.
void QsciLexer::setEolFill(bool eolfill, int style)
{
    if (style >= 0)
        emit eolFillChanged(eolfill, style);
    else
        for (int i = 0; i < 128; ++i)
            if (!description(i).isEmpty())
                emit eolFillChanged(eolfill, i);
}


// Set the font for a style.
void QsciLexer::setFont(const QFont &f, int style)
{
    if (style >= 0)
        emit fontChanged(f, style);
    else
        for (int i = 0; i < 128; ++i)
            if (!description(i).isEmpty())
                emit fontChanged(f, i);
}


// Set the background colour for a style.
void QsciLexer::setPaper(const QColor &c, int style)
{
    if (style >= 0)
        emit paperChanged(c, style);
    else
    {
        for (int i = 0; i < 128; ++i)
            if (!description(i).isEmpty())
                emit paperChanged(c, i);

        emit paperChanged(c, QsciScintillaBase::STYLE_DEFAULT);
    }
}
