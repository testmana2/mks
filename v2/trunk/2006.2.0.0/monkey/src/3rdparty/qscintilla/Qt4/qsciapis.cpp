// This module implements the QsciAPIs class.
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


#include "qsciapis.h"

#include <qapplication.h>
#include <qevent.h>
#include <qfile.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <qthread.h>

#include "qscilexer.h"


// The user event type that signals that the worker thread has finished.
const QEvent::Type WorkerFinished = static_cast<QEvent::Type>(QEvent::User + 1012);


// This class is the worker thread that post-processes the API set.
class QsciAPIsWorker : public QThread
{
public:
    QsciAPIsWorker(QsciAPIs *apis);
    virtual ~QsciAPIsWorker();

    virtual void run();

private:
    QsciAPIs *proxy;
    bool abort;
};


// The worker thread ctor.
QsciAPIsWorker::QsciAPIsWorker(QsciAPIs *apis)
    : proxy(apis), abort(false)
{
}


// The worker thread dtor.
QsciAPIsWorker::~QsciAPIsWorker()
{
    // Tell the thread to stop.  There is no need to bother with a mutex.
    abort = true;

    // Wait for it to do so and hit it if it doesn't.
    if (!wait(500))
        terminate();
}


// The worker thread entry point.
void QsciAPIsWorker::run()
{
    // Sort the full list.
    proxy->apis.sort();

    QStringList wseps = proxy->lex->autoCompletionWordSeparators();

    if (wseps.count())
    {
        const QString &wsep = wseps.first();

        // Split each entry into separate words but ignoring any arguments.
        for (int a = 0; a < proxy->apis.count(); ++a)
        {
            // Check to see if we should stop.
            if (abort)
                return;

            QString path = proxy->apis[a];
            int tail = path.indexOf('(');

            if (tail >= 0)
                path.truncate(tail);

            QStringList words = path.split(wsep);

            for (int w = 0; w < words.count(); ++w)
            {
                const QString &word = words[w];

                // Add the word's position to any existing list for this word.
                QsciAPIs::WordIndexList wil = proxy->wdict[word];
                wil.append(QsciAPIs::WordIndex(a, w));
                proxy->wdict[word] = wil;
            }
        }
    }

    // Tell the main thread we have finished.
    QApplication::postEvent(proxy, new QEvent(WorkerFinished));
}


// The ctor.
QsciAPIs::QsciAPIs(QsciLexer *lexer)
    : QObject(lexer), lex(lexer), worker(0), ready(true), ctcursor(0)
{
    connect(this, SIGNAL(workerStarted()), lex, SIGNAL(apiProcessingStarted()));
    connect(this, SIGNAL(workerFinished()), lex, SIGNAL(apiProcessingFinished()));
}


// The dtor.
QsciAPIs::~QsciAPIs()
{
    if (worker)
    {
        delete worker;
        worker = 0;
    }
}


//! Handle termination events from the worker thread.
bool QsciAPIs::event(QEvent *e)
{
    if (e->type() != WorkerFinished)
        return false;

    delete worker;
    worker = 0;

    ready = true;

    emit workerFinished();

    return true;
}


// Clear out all API information.
bool QsciAPIs::load(const QString &fname)
{
    QFile f(fname);

    if (!f.open(QIODevice::ReadOnly))
        return false;

    QTextStream ts(&f);

    for (;;)
    {
        QString line = ts.readLine();

        if (line.isEmpty())
            break;

        apis.append(line);
    }

    ready = false;

    return true;
}


// Add a single API entry.
void QsciAPIs::add(const QString &entry)
{
    apis.append(entry);
    ready = false;
}


// Remove a single API entry.
void QsciAPIs::remove(const QString &entry)
{
    int idx = apis.indexOf(entry);

    if (idx >= 0)
        apis.removeAt(idx);

    ready = false;
}


// Add auto-completion words to an existing list.
void QsciAPIs::autoCompletionList(const QStringList &words, bool cs,
        QStringList &wlist) const
{
    // Do nothing if the worker is still running.
    if (worker)
        return;

    // For the moment assume that this is a new and we don't have a current
    // context.

#if 0
    QStringList::const_iterator it = apis.begin();

    // Find the first match in the sorted list.
    while (it != apis.end())
    {
        if ((*it).startsWith(starts, (cs ? Qt::CaseSensitive : Qt::CaseInsensitive)))
            break;

        ++it;
    }

    QString prev;

    while (it != apis.end())
    {
        if (!(*it).startsWith(starts, (cs ? Qt::CaseSensitive : Qt::CaseInsensitive)))
            break;

        QString w = (*it).section('(', 0, 0);

        if (w != prev)
        {
            if (!wlist.contains(w))
                wlist.append(w);

            prev = w;
        }

        ++it;
    }
#endif
}


// Return the call tip for a function.
QString QsciAPIs::callTips(const QString &function, int maxnr, int commas)
{
    QStringList::const_iterator it;

    // Find the first match in the sorted list.
    for (it = apis.begin(); it != apis.end(); ++it)
        if ((*it).startsWith(function))
            break;

    QStringList cts;
    QString prev;

    while (it != apis.end() && (*it).startsWith(function))
    {
        if (maxnr > 0 && maxnr == cts.count())
            break;

        QString w = *it;

        // Remove any image ID used by auto-completion.
        w.replace(QRegExp("\\?[^(]*"),"");

        if (w.indexOf('(') == function.length() && w.count(',') >= commas && w != prev)
        {
            cts.append(w);
            prev = w;
        }

        ++it;
    }

    // See if we want to add a down arrow.
    if (maxnr < 0 && cts.count() > 1)
    {
        // Remember the state so we can scroll through it later.
        ctlist = cts;
        ctcursor = 0;

        QString ct = cts[0];

        ct.prepend('\002');

        return ct;
    }

    ctlist.clear();

    return cts.join("\n");
}


// Return the next or previous call tip.
QString QsciAPIs::callTipsNextPrev(int dir)
{
    QString ct;

    // Get the call tip.
    if (dir == 1 && ctcursor > 0)
        ct = ctlist[--ctcursor];
    else if (dir == 2 && ctcursor < ctlist.count() - 1)
        ct = ctlist[++ctcursor];

    // Add the arrows.
    if (!ct.isEmpty())
    {
        if (ctcursor < ctlist.count() - 1)
            ct.prepend('\002');

        if (ctcursor > 0)
            ct.prepend('\001');
    }

    return ct;
}


// Ensure the list is ready.
void QsciAPIs::prepare()
{
    // Handle the trivial case.
    if (ready || worker)
        return;

    emit workerStarted();

    worker = new QsciAPIsWorker(this);
    worker->start();
}
