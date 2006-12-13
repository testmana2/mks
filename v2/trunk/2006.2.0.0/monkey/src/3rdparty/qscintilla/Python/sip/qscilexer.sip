// This is the SIP interface definition for QsciLexer.
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


class QsciLexer : QObject
{
%TypeHeaderCode
#include <qscilexer.h>
%End

public:
%If (Qsci_Qt4)
    QsciLexer(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexer(QObject *parent /TransferThis/ = 0, const char *name = 0);
%End

    virtual const char *language() const = 0;
    virtual const char *lexer() const = 0;
    void apiAdd(const QString &entry);
    void apiClear();
    bool apiLoad(const QString &fname);
    void apiRemove(const QString &entry);
    int autoIndentStyle();
    virtual QColor color(int style) const;
    virtual bool eolFill(int style) const;
    virtual QFont font(int style) const;
    virtual const char *keywords(int set) const;
    virtual QString description(int style) const = 0;
    virtual QColor paper(int style) const;
    virtual QFont defaultFont() const;
    virtual QColor defaultColor() const;
    virtual QColor defaultPaper() const;
    virtual void refreshProperties();
    bool readSettings(QSettings &qs, const char * = "/Scintilla");
    bool writeSettings(QSettings &qs, const char * = "/Scintilla") const;

public slots:
    virtual void setAutoIndentStyle(int autoindentstyle);
    virtual void setColor(const QColor &c, int style = -1);
    virtual void setDefaultFont(const QFont &f);
    virtual void setDefaultColor(const QColor &c);
    virtual void setDefaultPaper(const QColor &c);
    virtual void setEolFill(bool eolfill, int style = -1);
    virtual void setFont(const QFont &f, int style = -1);
    virtual void setPaper(const QColor &c, int style = -1);

signals:
    void apiProcessingStarted();
    void apiProcessingFinished();
    void colorChanged(const QColor &c, int style);
    void eolFillChanged(bool eolfilled, int style);
    void fontChanged(const QFont &f, int style);
    void paperChanged(const QColor &c, int style);
    void propertyChanged(const char *prop, const char *val);

protected:
    virtual bool readProperties(QSettings &qs, const QString &prefix);
    virtual bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    QsciLexer(const QsciLexer &);

};
