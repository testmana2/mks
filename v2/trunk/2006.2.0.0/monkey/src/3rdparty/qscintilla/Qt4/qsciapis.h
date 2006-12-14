// This module defines interface to the QsciAPIs class.
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


#ifndef QSCIAPIS_H
#define QSCIAPIS_H


#include <qmap.h>
#include <qobject.h>
#include <qpair.h>
#include <qstringlist.h>

#include <QList>

#include <qsciglobal.h>


class QsciAPIsWorker;
class QsciLexer;


//! \brief The QsciAPIs class represents the textual API information used in
//! call tips and for auto-completion.
//!
//! This class is internal and not part of the QScintilla API.
class QsciAPIs : public QObject
{
    Q_OBJECT

public:
    //! \internal Constructs a QsciAPIs instance.
    QsciAPIs(QsciLexer *lexer);

    //! \internal Destroy the QsciAPIs instance.
    virtual ~QsciAPIs();

    //! \internal Add the single API entry \a entry to the current set.
    void add(const QString &entry);

    //! \internal Remove the single API entry \a entry from the current set.
    void remove(const QString &entry);

    //! \internal Load the API information from the file named \a fname, adding
    //! it to the current set.  Returns true if successful, otherwise false.
    bool load(const QString &fname);

    //! \internal Deletes all API information.
    void clear();

    //! \internal Ensures that the APIs are ready to be used.  This is a
    //! potentially time consuming operation.
    void prepare();

    //! \internal
    void autoCompletionList(const QStringList &words, bool cs, QStringList &wlist) const;

    //! \internal
    QString callTips(const QString &function, int maxnr, int commas);

    //! \internal
    QString callTipsNextPrev(int dir);

    //! \internal Reimplemented to receive termination events from the worker
    //! thread.
    virtual bool event(QEvent *e);

signals:
    //! \internal This signal is emitted when the worker thread starts.
    void workerStarted();

    //! \internal This signal is emitted when the worker thread finishes.
    void workerFinished();

private:
    friend class QsciAPIsWorker;

    const QsciLexer *lex;
    QsciAPIsWorker *worker;
    bool ready;
    int ctcursor;
    QStringList apis;
    QStringList ctlist;

    typedef QPair<int, int> WordIndex;
    typedef QList<WordIndex> WordIndexList;
    QMap<QString, WordIndexList> wdict;

    QsciAPIs(const QsciAPIs &);
    QsciAPIs &operator=(const QsciAPIs &);
};

#endif
