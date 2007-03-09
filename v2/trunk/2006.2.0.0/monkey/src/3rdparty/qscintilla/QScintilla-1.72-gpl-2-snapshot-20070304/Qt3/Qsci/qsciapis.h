// This module defines interface to the QsciAPIs class.
//
// Copyright (c) 2007
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


#include <qobject.h>
#include <qpair.h>
#include <qstringlist.h>

#include <qvaluelist.h>

#include <Qsci/qsciglobal.h>


class QsciAPIsPrepared;
class QsciAPIsWorker;
class QsciLexer;


//! \brief The QsciAPIs class represents the textual API information used in
//! call tips and for auto-completion.  API information is specific to a
//! particular language lexer but can be shared by multiple instances of the
//! lexer.
//!
//! Raw API information is read from one or more files.  Each API function is
//! described by a single line of text comprising the function's name, followed
//! by the function's optional comma separated parameters enclosed in
//! parenthesis, and finally followed by optional explanatory text.
//!
//! A function name may be followed by a `?' and a number.  The number is used
//! by auto-completion to display a registered QPixmap with the function name.
//!
//! All function names are used by auto-completion, but only those that include
//! function parameters are used in call tips.
//!
//! QScintilla only deals with prepared API information and not the raw
//! information described above.  This is done so that large APIs can be
//! handled while still being responsive to user input.  The conversion of raw
//! information to prepared information is time consuming (think tens of
//! seconds) and implemented in a separate thread.  Processed information can
//! be quickly saved to and loaded from files.  Such files are portable between
//! different architectures.
//!
//! QScintilla based applications that want to support large APIs would
//! normally provide the user with the ability to specify a set of, possibly
//! project specific, raw API files and convert them to prepared files that are
//! loaded quickly when the application is invoked.
class QSCINTILLA_EXPORT QsciAPIs : public QObject
{
    Q_OBJECT

public:
    //! Constructs a QsciAPIs instance attached to lexer \a lexer and with name
    //! \a name.  \a lexer becomes the instance's parent object although the
    //! instance can also be subsequently attached to other lexers.
    QsciAPIs(QsciLexer *lexer, const char *name = 0);

    //! Destroy the QsciAPIs instance.
    virtual ~QsciAPIs();

    //! Add the single raw API entry \a entry to the current set.
    //!
    //! \sa clear(), load(), remove()
    void add(const QString &entry);

    //! Deletes all raw API information.
    //!
    //! \sa add(), load(), remove()
    void clear();

    //! Load the API information from the file named \a fname, adding it to the
    //! current set.  Returns true if successful, otherwise false.
    bool load(const QString &fname);

    //! Remove the single raw API entry \a entry from the current set.
    //!
    //! \sa add(), clear(), load()
    void remove(const QString &entry);

    //! Convert the current raw API information to prepared API information.
    //! This is implemented by a separate thread.
    //!
    //! \sa cancelPreparation()
    void prepare();

    //! Cancel the conversion of the current raw API information to prepared
    //! API information.
    //!
    //! \sa prepare()
    void cancelPreparation();

    //! Return the default name of the prepared API information file.  It is
    //! based on the name of the associated lexer and in the directory defined
    //! by the QSCIDIR environment variable.  If the environment variable isn't
    //! set then $HOME/.qsci is used.
    QString defaultPreparedName() const;

    //! Check to see is a prepared API information file named \a fname exists.
    //! If \a fname is empty then the value returned by defaultPreparedName()
    //! is used.  Returns true if successful, otherwise false.
    //!
    //! \sa defaultPreparedName()
    bool isPrepared(const QString &fname = QString()) const;

    //! Load the prepared API information from the file named \a fname.  If
    //! \a fname is empty then a name is constructed based on the name of the
    //! associated lexer and saved in the directory defined by the QSCIDIR
    //! environment variable.  If the environment variable isn't set then
    //! $HOME/.qsci is used.  Returns true if successful, otherwise false.
    bool loadPrepared(const QString &fname = QString());

    //! Save the prepared API information to the file named \a fname.  If
    //! \a fname is empty then a name is constructed based on the name of the
    //! associated lexer and saved in the directory defined by the QSCIDIR
    //! environment variable.  If the environment variable isn't set then
    //! $HOME/.qsci is used.  Returns true if successful, otherwise false.
    bool savePrepared(const QString &fname = QString()) const;

    //! \internal Updates an auto-completion list with API entries based on a
    //! set of words from the user.
    void autoCompletionList(const QStringList &words, QStringList &wlist);

    //! \internal Handle the selection of an entry in the auto-completion list.
    void autoCompletionSelected(const QString &sel);

    //! \internal
    QString callTips(const QString &function, int maxnr, int commas);

    //! \internal
    QString callTipsNextPrev(int dir);

    //! \internal Reimplemented to receive termination events from the worker
    //! thread.
    virtual bool event(QEvent *e);

    //! Return a list of the installed raw API file names for the associated
    //! lexer.
    QStringList installedAPIFiles() const;

signals:
    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information has been cancelled.
    //!
    //! \sa apiPreparationFinished(), apiPreparationStarted()
    void apiPreparationCancelled();

    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information starts and can be used to give some visual
    //! feedback to the user.
    //!
    //! \sa apiPreparationCancelled(), apiPreparationFinished()
    void apiPreparationStarted();
    
    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information has finished.
    //!
    //! \sa apiPreparationCancelled(), apiPreparationStarted()
    void apiPreparationFinished();

private:
    friend class QsciAPIsPrepared;
    friend class QsciAPIsWorker;

    // This indexes a word in a set of raw APIs.  The first part indexes the
    // entry in the set, the second part indexes the word within the entry.
    typedef QPair<Q_UINT32, Q_UINT32> WordIndex;

    // This is a list of word indexes.
    typedef QValueList<WordIndex> WordIndexList;

    const QsciLexer *lex;
    QsciAPIsWorker *worker;
    int ctcursor;
    QStringList prev_words;
    QStringList::const_iterator origin;
    int origin_len;
    QStringList apis;
    QsciAPIsPrepared *prep;
    QStringList ctlist;

    void lastCompleteWord(const QString &word, QStringList &wlist) const;
    void lastPartialWord(const QString &word, QStringList &wlist) const;
    void addAPIEntries(const WordIndexList &wl, bool complete,
            QStringList &wlist) const;
    QString addOrigin(const QStringList &api_words, int idx,
            const QString &wsep) const;
    QString prepName(const QString &fname, bool mkpath = false) const;
    void deleteWorker();

    QsciAPIs(const QsciAPIs &);
    QsciAPIs &operator=(const QsciAPIs &);
};

#endif
