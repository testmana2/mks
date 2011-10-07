/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef QTDOCINSTALLER
#define QTDOCINSTALLER

#include <QtCore/QThread>
#include <QtCore/QMutex>

QT_BEGIN_NAMESPACE

class QHelpEngineCore;

class QtDocInstaller : public QThread
{
    Q_OBJECT

public:
    QtDocInstaller(const QString &collectionFile);
    ~QtDocInstaller();
    void installDocs();

signals:
    void errorMessage(const QString &msg);
    void docsInstalled(bool newDocsInstalled);

private:
    void run();
    bool installDoc(const QString &name,
        QHelpEngineCore *helpEngine);

    bool m_abort;
    QString m_collectionFile;
    QMutex m_mutex;
};

QT_END_NAMESPACE

#endif
