// This is the SIP interface definition for QsciLexerProperties.
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


class QsciLexerProperties : QsciLexer
{
%TypeHeaderCode
#include <Qsci/qscilexerproperties.h>
%End

public:
    enum {
        Default,
        Comment,
        Section,
        Assignment,
        DefaultValue
    };

%If (Qsci_Qt4)
    QsciLexerProperties(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexerProperties(QObject *parent /TransferThis/ = 0,
            const char *name = 0);
%End

    virtual ~QsciLexerProperties();

    const char *language() const;
    const char *lexer() const;
    QColor color(int style) const;
    bool eolFill(int style) const;
    QFont font(int style) const;
    QString description(int style) const;
    QColor paper(int style) const;

    void refreshProperties();
    bool foldCompact() const;

public slots:
    virtual void setFoldCompact(bool fold);

protected:
    bool readProperties(QSettings &qs, const QString &prefix);
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    QsciLexerProperties(const QsciLexerProperties &);
};
