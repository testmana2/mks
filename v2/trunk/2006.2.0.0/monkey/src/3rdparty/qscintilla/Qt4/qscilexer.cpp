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


#include "qscilexer.h"

#include <qapplication.h>
#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

#include "qsciapis.h"
#include "qsciscintilla.h"


// The ctor.
QsciLexer::QsciLexer(QObject *parent)
    : QObject(parent),
      autoIndStyle(-1), apiSet(0)
{
#if defined(Q_OS_WIN)
    defFont = QFont("Verdana",10);
#else
    defFont = QFont("Bitstream Vera Sans",9);
#endif

    // Set the default fore and background colours.
    QPalette pal = QApplication::palette();
    defColor = pal.text().color();
    defPaper = pal.base().color();
}


// Return a pointer to the prepared APIs if there are any.
QsciAPIs *QsciLexer::apis() const
{
    if (apiSet)
        apiSet->prepare();

    return apiSet;
}


// Clear the current API set.
void QsciLexer::apiClear()
{
    if (apiSet)
    {
        delete apiSet;
        apiSet = 0;
    }
}


// Add a single API to the set.
void QsciLexer::apiAdd(const QString &entry)
{
    if (!apiSet)
        apiSet = new QsciAPIs(this);

    apiSet->add(entry);
}


// Remove a single API from the set.
void QsciLexer::apiRemove(const QString &entry)
{
    if (!apiSet)
        return;

    apiSet->remove(entry);
}


// Load a file into the set.
bool QsciLexer::apiLoad(const QString &fname)
{
    if (!apiSet)
        apiSet = new QsciAPIs(this);

    return apiSet->load(fname);
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
        ok = qs.contains(key + "color");
        num = qs.value(key + "color", 0).toInt();

        if (ok)
            emit colorChanged(QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i);
        else
            rc = false;

        // Read the end-of-line fill.
        ok = qs.contains(key + "eolfill");
        flag = qs.value(key + "eolfill", false).toBool();

        if (ok)
            emit eolFillChanged(flag, i);
        else
            rc = false;

        // Read the font
        QStringList fdesc;

        ok = qs.contains(key + "font");
        fdesc = qs.value(key + "font").toStringList();

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
        ok = qs.contains(key + "paper");
        num = qs.value(key + "paper", 0).toInt();

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

    ok = qs.contains(key + "autoindentstyle");
    num = qs.value(key + "autoindentstyle", 0).toInt();

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

        qs.setValue(key + "color", num);

        // Write the end-of-line fill.
        qs.setValue(key + "eolfill", eolFill(i));

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

        qs.setValue(key + "font", fdesc);

        // Write the background colour.
        c = paper(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        qs.setValue(key + "paper", num);
    }

    // Write the properties.
    key.sprintf("%s/%s/properties/",prefix,language());

    if (!writeProperties(qs,key))
        rc = false;

    // Write the rest.
    key.sprintf("%s/%s/",prefix,language());

    qs.setValue(key + "autoindentstyle", autoIndStyle);

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
        for (int i = 0; i < 128; ++i)
            if (!description(i).isEmpty())
                emit paperChanged(c, i);
}
