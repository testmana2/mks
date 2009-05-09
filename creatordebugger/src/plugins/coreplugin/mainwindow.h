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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "core_global.h"

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QPointer>
#include <QtGui/QPrinter>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE
class QSettings;
class QShortcut;
QT_END_NAMESPACE

namespace Core {

class ActionManager;
class BaseMode;
class BaseView;
class EditorManager;
class FileManager;
class IContext;
class IWizard;
class MessageManager;
class MimeDatabase;
class ModeManager;
class ProgressManager;
class RightPaneWidget;
class ScriptManager;
class UniqueIDManager;
class VariableManager;
class VCSManager;
class ViewManagerInterface;

namespace Internal {

class ActionManagerPrivate;
class CoreImpl;
class FancyTabWidget;
class GeneralSettings;
class NavigationWidget;
class ProgressManagerPrivate;
class ShortcutSettings;
class ViewManager;
class VersionDialog;

class CORE_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    
    IContext *contextObject(QWidget *widget);
    void addContextObject(IContext *contex);
    void removeContextObject(IContext *contex);
    void resetContext();
	
    void openFiles(const QStringList &fileNames);
	
    Core::ActionManager *actionManager() const;
    Core::FileManager *fileManager() const;
    Core::UniqueIDManager *uniqueIDManager() const;
    Core::MessageManager *messageManager() const;
    Core::EditorManager *editorManager() const;
    Core::ProgressManager *progressManager() const;
    Core::ScriptManager *scriptManager() const;
    Core::VariableManager *variableManager() const;
    Core::ModeManager *modeManager() const;
    Core::MimeDatabase *mimeDatabase() const;

    VCSManager *vcsManager() const;	
    inline QSettings *settings() const { return m_settings; }
    virtual QPrinter *printer() const;
	
    IContext * currentContextObject() const;
    QStatusBar *statusBar() const;
    void addAdditionalContext(int context);
    void removeAdditionalContext(int context);
    bool hasContext(int context) const;

    void updateContext();

public slots:
    QStringList showNewItemDialog(const QString &title,
                                  const QList<IWizard *> &wizards,
                                  const QString &defaultLocation = QString());

    void showOptionsDialog(const QString &category = QString(), const QString &page = QString());


private:
    void updateContextObject(IContext *context);
	
    CoreImpl *m_coreImpl;
    UniqueIDManager *m_uniqueIDManager;
    QList<int> m_globalContext;
    QList<int> m_additionalContexts;
    QSettings *m_settings;
    mutable QPrinter *m_printer;
    ActionManagerPrivate *m_actionManager;
    EditorManager *m_editorManager;
    FileManager *m_fileManager;
    MessageManager *m_messageManager;
    ProgressManagerPrivate *m_progressManager;
    ScriptManager *m_scriptManager;
    VariableManager *m_variableManager;
    VCSManager *m_vcsManager;
    ViewManager *m_viewManager;
    ModeManager *m_modeManager;
    MimeDatabase *m_mimeDatabase;
    FancyTabWidget *m_modeStack;
    NavigationWidget *m_navigationWidget;
    RightPaneWidget *m_rightPaneWidget;
    Core::BaseView *m_outputView;
    VersionDialog *m_versionDialog;

    IContext * m_activeContext;

    QMap<QWidget *, IContext *> m_contextWidgets;
	
};

} // namespace Internal
} // namespace Core

#endif // MAINWINDOW_H
