// This module implements the QsciLexerCSharp class.
//
// Copyright (c) 2008 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// Please review the following information to ensure GNU General
// Public Licensing requirements will be met:
// http://trolltech.com/products/qt/licenses/licensing/opensource/. If
// you are unsure which license is appropriate for your use, please
// review the following information:
// http://trolltech.com/products/qt/licenses/licensing/licensingoverview
// or contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
// granted herein.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexercsharp.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerCSharp::QsciLexerCSharp(QObject *parent)
    : QsciLexerCPP(parent)
{
}


// The dtor.
QsciLexerCSharp::~QsciLexerCSharp()
{
}


// Returns the language name.
const char *QsciLexerCSharp::language() const
{
    return "C#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerCSharp::defaultColor(int style) const
{
    if (style == VerbatimString)
        return QColor(0x00,0x7f,0x00);

    return QsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerCSharp::defaultEolFill(int style) const
{
    if (style == VerbatimString)
        return true;

    return QsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerCSharp::defaultFont(int style) const
{
    if (style == VerbatimString)
#if defined(Q_OS_WIN)
        return QFont("Courier New",10);
#else
        return QFont("Bitstream Vera Sans Mono",9);
#endif

    return QsciLexerCPP::defaultFont(style);
}


// Returns the set of keywords.
const char *QsciLexerCSharp::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "abstract as base bool break byte case catch char checked "
           "class const continue decimal default delegate do double else "
           "enum event explicit extern false finally fixed float for "
           "foreach goto if implicit in int interface internal is lock "
           "long namespace new null object operator out override params "
           "private protected public readonly ref return sbyte sealed "
           "short sizeof stackalloc static string struct switch this "
           "throw true try typeof uint ulong unchecked unsafe ushort "
           "using virtual void while";
}


// Returns the user name of a style.
QString QsciLexerCSharp::description(int style) const
{
    if (style == VerbatimString)
        return tr("Verbatim string");

    return QsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor QsciLexerCSharp::defaultPaper(int style) const
{
    if (style == VerbatimString)
        return QColor(0xe0,0xff,0xe0);

    return QsciLexer::defaultPaper(style);
}
