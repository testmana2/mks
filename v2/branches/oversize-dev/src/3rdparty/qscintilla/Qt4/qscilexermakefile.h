// This defines the interface to the QsciLexerMakefile class.
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


#ifndef QSCILEXERMAKEFILE_H
#define QSCILEXERMAKEFILE_H


#include <qobject.h>

#include <qsciglobal.h>
#include <qscilexer.h>


//! \brief The QsciLexerMakefile class encapsulates the Scintilla
//! Makefile lexer.
class QSCINTILLA_EXPORT QsciLexerMakefile : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Makefile lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A pre-processor directive.
        Preprocessor = 2,

        //! A variable.
        Variable = 3,

        //! An operator.
        Operator = 4,

        //! A target.
        Target = 5,

        //! An error.
        Error = 9
    };

    //! Construct a QsciLexerMakefile with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerMakefile(QObject *parent = 0);

    //! Destroys the QsciLexerMakefile instance.
    virtual ~QsciLexerMakefile();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa paper()
    QColor color(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool eolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont font(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Returns the background colour of the text for style number
    //! \a style.
    //!
    //! \sa color()
    QColor paper(int style) const;

private:
    QsciLexerMakefile(const QsciLexerMakefile &);
    QsciLexerMakefile &operator=(const QsciLexerMakefile &);
};

#endif
