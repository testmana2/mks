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


#include "Qsci/qsciapis.h"

#include <qapplication.h>
#include <qevent.h>
#include <qfile.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <qthread.h>

#include "Qsci/qscilexer.h"


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

            QStringList words = proxy->apiWords(a, wsep);

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
    : QObject(lexer), lex(lexer), worker(0), ready(true), ctcursor(0),
      origin_len(0)
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
void QsciAPIs::autoCompletionList(const QStringList &words, QStringList &wlist)
{
    // Do nothing if the worker is still running.
    if (worker)
        return;

    // Get the list of words (while handling the lexer's case sensitivity) and
    // see if the context is the same as last time we were called.
    QStringList new_words;
    bool same_context = (prev_words.count() > 0 && prev_words.count() < words.count());

    for (int i = 0; i < words.count(); ++i)
    {
        QString new_word = words.at(i);

        if (!lex->caseSensitive())
            new_word = new_word.toLower();

        if (i < prev_words.count() && prev_words.at(i) != new_word)
            same_context = false;

        new_words << new_word;
    }

    // If the context has changed then reset the origin.
    if (!same_context)
        origin_len = 0;

    // If we have a current origin (ie. the user made a specific selection in
    // the current context) then adjust the origin to include the last complete
    // word as the user may have entered more parts of the name without using
    // auto-completion.
    if (origin_len > 0)
    {
        QStringList wseps = lex->autoCompletionWordSeparators();
        const QString &wsep = wseps.first();

        int start_new = prev_words.count();
        int end_new = new_words.count();

        if (new_words.last().isEmpty())
            --end_new;

        QString fixed = *origin;
        fixed.truncate(origin_len);
        fixed.append(wsep);

        QString path = fixed;

        while (start_new < end_new)
        {
            // Add this word to the current path.
            path.append(new_words.at(start_new));

            // Check this is not the last partial word.
            if (start_new + 1 < end_new || end_new < new_words.count())
            {
                origin_len = path.size();
                path.append(wsep);
            }

            // Skip entries in the current origin that don't match the path.
            while (origin != apis.end())
            {
                const QString &owords = *origin;

                // See if the current origin has come to an end.
                if (!owords.startsWith(fixed))
                {
                    origin = apis.end();
                    break;
                }

                if (owords.startsWith(path))
                    break;

                ++origin;
            }

            if (origin == apis.end())
                break;

            ++start_new;
        }

        // If the new text wasn't recognised then reset the origin.
        if (start_new < end_new)
            origin_len = 0;
        else
        {
            QList<QString>::const_iterator it = origin;

            while (it != apis.end())
            {
                QString base = apiBaseName(*it);

                if (!base.startsWith(path))
                    break;

                // Get the word we are interested in (ie. the one after the
                // current origin in path).
                QString w = base.mid(origin_len + wsep.size()).split(wsep).at(0);

                // Append the origin.
                QString full = QString("%1 (%2)").arg(w).arg(base.left(origin_len));

                if (!wlist.contains(full))
                    wlist.append(full);

                ++it;
            }
        }
    }

    // If we haven't got a list so far then resort to the last word entered.
    if (origin_len == 0)
        if (new_words.last().isEmpty())
            lastCompleteWord(new_words.at(new_words.count() - 2), wlist);
        else
            lastPartialWord(new_words.last(), wlist);

    // Save the "committed" words for next time.
    prev_words = new_words;
    prev_words.removeLast();
}


// Add auto-completion words based on the last complete word entered.
void QsciAPIs::lastCompleteWord(const QString &word, QStringList &wlist) const
{
    // Get the possible API entries if any.
    const WordIndexList &wl = wdict[word];

    if (wl.isEmpty())
        return;

    addAPIEntries(wl, true, wlist);
}


// Add auto-completion words based on the last partial word entered.
void QsciAPIs::lastPartialWord(const QString &word, QStringList &wlist) const
{
    QMap<QString, WordIndexList>::const_iterator it = wdict.lowerBound(word);

    while (it != wdict.end())
    {
        if (!it.key().startsWith(word))
            break;

        addAPIEntries(it.value(), false, wlist);

        ++it;
    }
}


// Handle the selection of an entry in the auto-completion list.
void QsciAPIs::autoCompletionSelected(const QString &sel)
{
    // If the selection is an API (ie. it has a space separating the selected
    // word and the optional origin) then remember the origin.
    QStringList lst = sel.split(' ');

    if (lst.count() != 2 || lst.at(1).isEmpty())
    {
        // It's not an API entry nor it's the first word of one.
        origin_len = 0;
        return;
    }

    // Remove the parenthesis from the origin.
    QString owords = lst.at(1).mid(1, lst.at(1).count() - 2);

    origin = qLowerBound(apis, owords);
    origin_len = owords.size();
}


// Add auto-completion words for a particular word (defined by where it appears
// in the APIs) and depending on whether the word was complete (when it's
// actually the next word in the API entry that is of interest) or not.
void QsciAPIs::addAPIEntries(const WordIndexList &wl, bool complete, QStringList &wlist) const
{
    QStringList wseps = lex->autoCompletionWordSeparators();
    const QString &wsep = wseps.first();

    for (int w = 0; w < wl.count(); ++w)
    {
        const WordIndex &wi = wl.at(w);

        QStringList api_words = apiWords(wi.first, wsep);

        int idx = wi.second;

        if (complete)
        {
            // Skip if this is the last word.
            if (++idx >= api_words.count())
                continue;
        }

        // Add the next word to the list if it isn't already there.
        QString api_word = addOrigin(api_words, idx, wsep);

        if (!wlist.contains(api_word))
            wlist.append(api_word);
    }
}


// Convert a word in a list of words that make up an API entry into what the
// user sees by appending the origin of the word (ie. all the words that
// precede it in the API entry).  We have to append it because of the way
// Scintilla works.  (This might be a case where we patch the Scintilla code to
// allow, for example, the complete API entry to be displayed with the embedded
// word of interest highlighted in some way.)
QString QsciAPIs::addOrigin(const QStringList &api_words, int idx,
        const QString &wsep) const
{
    const QString &api_word = api_words.at(idx);

    // Always have a space so that we can tell if the selected entry was an
    // API or not.
    if (idx == 0)
        return api_word + ' ';

    QStringList origin = api_words.mid(0, idx);

    return QString("%1 (%2)").arg(api_word).arg(origin.join(wsep));
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


// Return a particular API entry as a list of words.
QStringList QsciAPIs::apiWords(int api_idx, const QString &wsep) const
{
    QString base = apiBaseName(apis[api_idx]);

    return base.split(wsep);
}


// Return the name of an API function, ie. without the arguments.
QString QsciAPIs::apiBaseName(const QString &api)
{
    QString base = api;
    int tail = base.indexOf('(');

    if (tail >= 0)
        base.truncate(tail);

    return base;
}
