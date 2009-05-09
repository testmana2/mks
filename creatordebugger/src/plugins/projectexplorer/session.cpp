/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
**************************************************************************/

#include "session.h"

#include "projectexplorerconstants.h"
#include "persistentsettings.h"

#include <coreplugin/icore.h>
#include <coreplugin/ifile.h>
#include <coreplugin/editormanager/editormanager.h>

#include <QDebug>

#include <QtCore/QVariant>
#include <QtCore/QFuture>
#include <QtGui/QApplication>
#include <QStringList>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>

using namespace ProjectExplorer;

namespace {
    bool debug = false;
}


/* SessionFile definitions */
namespace ProjectExplorer {
namespace Internal {

class SessionFile : public Core::IFile
{
    Q_OBJECT

public:
    SessionFile();

    bool load(const QString &fileName);
    bool save(const QString &fileName = QString());

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString defaultPath() const;
    QString suggestedFileName() const;
    virtual QString mimeType() const;

    bool isModified() const;
    bool isReadOnly() const;
    bool isSaveAsAllowed() const;

    void modified(Core::IFile::ReloadBehavior *behavior);

public slots:
    void sessionLoadingProgress();


private:
    const QString m_mimeType;
    Core::ICore *m_core;

    QString m_fileName;
    QMap<QString, QStringList> m_depMap;

    QMap<QString, QVariant> m_values;

    QFutureInterface<void> future;
    friend class ProjectExplorer::SessionManager;
};

} // namespace Internal
} // namespace ProjectExplorer


using namespace ProjectExplorer;
using Internal::SessionFile;


void SessionFile::sessionLoadingProgress()
{
    future.setProgressValue(future.progressValue() + 1);
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

SessionFile::SessionFile()
  : m_mimeType(QLatin1String(ProjectExplorer::Constants::SESSIONFILE_MIMETYPE)),
    m_core(Core::ICore::instance())
{
}

QString SessionFile::mimeType() const
{
    return m_mimeType;
}

bool SessionFile::load(const QString &fileName)
{
    Q_ASSERT(!fileName.isEmpty());

    if (debug)
        qDebug() << "SessionFile::load " << fileName;

    m_fileName = fileName;

    // NPE: Load the session in the background?
    // NPE: Let FileManager monitor filename
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    PersistentSettingsReader reader;
    if (!reader.load(m_fileName)) {
        qWarning() << "SessionManager::load failed!" << fileName;
        QApplication::restoreOverrideCursor();
        return false;
    }

    const QStringList &keys = reader.restoreValue(QLatin1String("valueKeys")).toStringList();
    foreach (const QString &key, keys) {
        QVariant value = reader.restoreValue("value-" + key);
        m_values.insert(key, value);
    }

    QStringList fileList =
        reader.restoreValue(QLatin1String("ProjectList")).toStringList();
    QString configDir = QFileInfo(m_core->settings()->fileName()).path();
    QMutableStringListIterator it(fileList);
    while (it.hasNext()) {
        const QString &file = it.next();
        if (QFileInfo(file).isRelative()) {
            // We used to write relative paths into the session file
            // relative to the session files, and those were stored in the
            // config dir
            it.setValue(configDir + "/" + file);
        }
    }

    int openEditorsCount = reader.restoreValue(QLatin1String("OpenEditors")).toInt();

    future.setProgressRange(0, fileList.count() + openEditorsCount + 2);
    future.setProgressValue(1);

    // indirectly adds projects to session

    sessionLoadingProgress();

    // convert the relative paths in the dependency map to absolute paths
    QMap<QString, QVariant> depMap = reader.restoreValue(QLatin1String("ProjectDependencies")).toMap();
    QMap<QString, QVariant>::const_iterator i = depMap.constBegin();
    while (i != depMap.constEnd()) {
        const QString &key = i.key();
        QStringList values;
        foreach (const QString &value, i.value().toStringList()) {
            values << value;
        }
        m_depMap.insert(key, values);
        ++i;
    }

    const QVariant &editorsettings = reader.restoreValue(QLatin1String("EditorSettings"));
    if (editorsettings.isValid()) {
        connect(m_core->editorManager(), SIGNAL(editorOpened(Core::IEditor *)),
                this, SLOT(sessionLoadingProgress()));
        m_core->editorManager()->restoreState(
            QByteArray::fromBase64(editorsettings.toByteArray()));
        disconnect(m_core->editorManager(), SIGNAL(editorOpened(Core::IEditor *)),
                   this, SLOT(sessionLoadingProgress()));
    }

    if (debug)
        qDebug() << "SessionFile::load finished" << fileName;


    future.reportFinished();
    QApplication::restoreOverrideCursor();
    return true;
}

bool SessionFile::save(const QString &fileName)
{
    if (!fileName.isEmpty())
        m_fileName = fileName;

    Q_ASSERT(!m_fileName.isEmpty());

    if (debug)
        qDebug() << "SessionFile - saving " << m_fileName;

    PersistentSettingsWriter writer;

    QStringList projectFiles;

    writer.saveValue(QLatin1String("ProjectList"), projectFiles);

    QMap<QString, QVariant> depMap;
    QMap<QString, QStringList>::const_iterator i = m_depMap.constBegin();
    while (i != m_depMap.constEnd()) {
        QString key = i.key();
        QStringList values;
        foreach (const QString &value, i.value()) {
            values << value;
        }
        depMap.insert(key, values);
        ++i;
    }
    writer.saveValue(QLatin1String("ProjectDependencies"), QVariant(depMap));

    writer.saveValue(QLatin1String("OpenEditors"),
                     m_core->editorManager()->openedEditors().count());
    writer.saveValue(QLatin1String("EditorSettings"),
                     m_core->editorManager()->saveState().toBase64());

    QMap<QString, QVariant>::const_iterator it, end;
    end = m_values.constEnd();
    QStringList keys;
    for (it = m_values.constBegin(); it != end; ++it) {
        writer.saveValue("value-" + it.key(), it.value());
        keys << it.key();
    }

    writer.saveValue("valueKeys", keys);


    if (writer.save(m_fileName, "QtCreatorSession"))
        return true;

    return false;
}

QString SessionFile::fileName() const
{
    return m_fileName;
}

void SessionFile::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

bool SessionFile::isModified() const
{
    return true;
}

bool SessionFile::isReadOnly() const
{
    return false;
}

bool SessionFile::isSaveAsAllowed() const
{
    return true;
}

void SessionFile::modified(Core::IFile::ReloadBehavior *)
{
}

QString SessionFile::defaultPath() const
{
    return QString();
}

QString SessionFile::suggestedFileName() const
{
    return tr("Untitled", "default file name to display");
}



SessionManager::SessionManager(QObject *parent)
  : QObject(parent),
	m_file(new SessionFile)
{
}

void SessionManager::setValue(const QString &name, const QVariant &value)
{
    m_file->m_values.insert(name, value);
}

QVariant SessionManager::value(const QString &name)
{
    QMap<QString, QVariant>::const_iterator it = m_file->m_values.find(name);
    if (it != m_file->m_values.constEnd())
        return *it;
    else
        return QVariant();
}

void SessionManager::createAndLoadNewDefaultSession()
{
    updateName("default");
    createImpl(sessionNameToFileName(m_sessionName));
}

bool SessionManager::createImpl(const QString &fileName)
{
    Q_ASSERT(!fileName.isEmpty());

    if (debug)
        qDebug() << "SessionManager - creating new session " << fileName << " ...";

    bool success = true;

    if (!m_file->fileName().isEmpty()) {
        if (!save() || !clear())
            success = false;
    }

    if (success) {
        delete m_file;
        m_file = new SessionFile;
        m_file->setFileName(fileName);
    }

    if (debug)
        qDebug() << "SessionManager - creating new session returns " << success;

    return success;
}

bool SessionManager::save()
{
    if (debug)
        qDebug() << "SessionManager - saving session" << m_sessionName;

    bool result = m_file->save();

    if (!result) {
        QMessageBox::warning(0, tr("Error while saving session"),
                                tr("Could not save session to file %1").arg(m_file->fileName()));
    }

    if (debug)
        qDebug() << "SessionManager - saving session returned " << result;

    return result;
}


/*!
  \fn bool SessionManager::clear()

  Returns whether the clear operation has been not cancelled & all projects could be closed.
  */
bool SessionManager::clear()
{
    if (debug)
        qDebug() << "SessionManager - clearing session ...";

    bool cancelled;

    bool success = !cancelled;

    if (debug)
        qDebug() << "SessionManager - clearing session result is " << success;

    return success;
}

void SessionManager::updateName(const QString &session)
{
    m_sessionName = session;
}

QString SessionManager::sessionNameToFileName(const QString &session)
{
    QString sn = session;
    return QFileInfo(m_core->settings()->fileName()).path() + "/qtcreator/" + sn + ".qws";
}

#include "session.moc"
