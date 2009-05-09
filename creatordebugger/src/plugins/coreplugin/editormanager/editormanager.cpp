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
#include <QDebug>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>
#include <QtCore/QFileInfo>

#include <coreplugin/editormanager/ieditorfactory.h>
#include <extensionsystem/pluginmanager.h>

#include "editormanager.h"
#include "ieditor.h"
#include "icore.h"
#include "mimedatabase.h"

using namespace Core;

enum { debugEditorManager=0 };

static inline ExtensionSystem::PluginManager *pluginManager()
{
    return ExtensionSystem::PluginManager::instance();
}

EditorManager *EditorManager::m_instance = 0;

EditorManager::EditorManager(ICore *core, QWidget *parent) :
    QWidget(parent),
	m_core(core)
{
    m_instance = this;
}

IEditor *EditorManager::openEditor(const QString &fileName, const QString &editorKind,
                                   bool ignoreNavigationHistory)
{
    if (debugEditorManager)
        qDebug() << Q_FUNC_INFO << fileName << editorKind;

    if (fileName.isEmpty())
        return 0;

    const QList<IEditor *> editors = editorsForFileName(fileName);
    if (!editors.isEmpty()) {
        setCurrentEditor(editors.first(), ignoreNavigationHistory);
        return editors.first();
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    IEditor *editor = createEditor(editorKind, fileName);
    if (!editor || !editor->open(fileName)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(m_core->mainWindow(), tr("Opening File"), tr("Cannot open file %1!").arg(fileName));
        delete editor;
        editor = 0;
        return 0;
    }
    insertEditor(editor, ignoreNavigationHistory);
    QApplication::restoreOverrideCursor();
    setCurrentEditor(editor);
    return editor;
}

IEditor *EditorManager::createEditor(const QString &editorKind,
                                     const QString &fileName)
{
    typedef QList<IEditorFactory*> FactoryList;
    if (debugEditorManager)
        qDebug() << Q_FUNC_INFO << editorKind << fileName;


    EditorFactoryList factories;
    if (editorKind.isEmpty()) {
        // Find by mime type
        const MimeType mimeType = m_core->mimeDatabase()->findByFile(QFileInfo(fileName));
        if (!mimeType) {
            qWarning("%s unable to determine mime type of %s/%s.",
                     Q_FUNC_INFO, fileName.toUtf8().constData(), editorKind.toUtf8().constData());
            return 0;
        }
        factories = editorFactories(mimeType, true);
    } else {
        // Find by editor kind
        const EditorFactoryList allFactories = pluginManager()->getObjects<IEditorFactory>();
        const EditorFactoryList::const_iterator acend = allFactories.constEnd();
        for (EditorFactoryList::const_iterator ait = allFactories.constBegin(); ait != acend; ++ait) {
            if (editorKind == (*ait)->kind()) {
                factories.push_back(*ait);
                break;
            }
        }
    }
    if (factories.empty()) {
        qWarning("%s: unable to find an editor factory for the file '%s', editor kind '%s'.",
                 Q_FUNC_INFO, fileName.toUtf8().constData(), editorKind.toUtf8().constData());
        return 0;
    }

    IEditor *editor = factories.front()->createEditor(this);
    if (editor)
        connect(editor, SIGNAL(changed()), this, SLOT(updateActions()));
    if (editor)
        emit editorCreated(editor, fileName);
    return editor;
}

void EditorManager::insertEditor(IEditor *editor,
                                 bool ignoreNavigationHistory,
                                 EditorGroup *group)
{
    if (!editor)
        return;
    m_d->m_core->addContextObject(editor);
    registerEditor(editor);
    if (group)
        group->addEditor(editor);
    else
        m_d->m_splitter->currentGroup()->addEditor(editor);

    setCurrentEditor(editor, ignoreNavigationHistory);
    emit editorOpened(editor);
}

QList<IEditor *> EditorManager::editorsForFileName(const QString &filename) const
{
    QList<IEditor *> found;
    QString fixedname = FileManager::fixFileName(filename);
    foreach (IEditor *editor, openedEditors()) {
        if (fixedname == FileManager::fixFileName(editor->file()->fileName()))
            found << editor;
    }
    return found;
}

void EditorManager::setCurrentEditor(IEditor *editor, bool ignoreNavigationHistory)
{
    if (debugEditorManager)
        qDebug() << Q_FUNC_INFO << currentEditor() << "-->" << editor
                << (m_d->m_suppressEditorChanges?"suppressed":"")
                << "ignore history?" << ignoreNavigationHistory;
    if (m_d->m_suppressEditorChanges)
        return;
    if (editor) {
        bool addToHistory = (!ignoreNavigationHistory && editor != currentEditor());
        if (debugEditorManager)
            qDebug() << Q_FUNC_INFO << (addToHistory ? "adding to history" : "not adding to history");
        if (addToHistory)
            addCurrentPositionToNavigationHistory(true);
        EditorGroup *group = groupOfEditor(editor);
        if (!group)
            return;
        m_d->m_suppressEditorChanges = true;
        m_d->m_splitter->setCurrentGroup(group);
        group->setCurrentEditor(editor);
        m_d->m_suppressEditorChanges = false;
        if (addToHistory)
            addCurrentPositionToNavigationHistory();
    }
    editorChanged(editor);
}

IEditor *EditorManager::currentEditor() const
{
	qDebug () << __FILE__ << __FUNCTION__;
    return NULL;    
}

QList<IEditor*> EditorManager::openedEditors() const
{
    QList<IEditor*> editors;
	qDebug () << __FILE__ << __FUNCTION__;
	return editors;
}

EditorManagerPlaceHolder::EditorManagerPlaceHolder(Core::IMode *, QWidget *)
{
	qDebug () << __FILE__ << __FUNCTION__;
}

QByteArray EditorManager::saveState() const
{
	qDebug () << __FILE__ << __FUNCTION__;
	return QByteArray();
}

bool EditorManager::restoreState(const QByteArray &state)
{
	qDebug () << __FILE__ << __FUNCTION__;
	return false;
}

EditorManager::EditorFactoryList
    EditorManager::editorFactories(const MimeType &mimeType, bool bestMatchOnly) const
{
    EditorFactoryList rc;
    const EditorFactoryList allFactories = pluginManager()->getObjects<IEditorFactory>();
    mimeTypeFactoryRecursion(m_d->m_core->mimeDatabase(), mimeType, allFactories, bestMatchOnly, &rc);
    if (debugEditorManager)
        qDebug() << Q_FUNC_INFO << mimeType.type() << " returns " << rc;
    return rc;
}
