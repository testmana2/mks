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

#include <projectexplorer/projectexplorerconstants.h>

#include "mainwindow.h"
#include "actioncontainer.h"
#include "actionmanager_p.h"
#include "basemode.h"
#include "coreimpl.h"
#include "coreconstants.h"
#include "editorgroup.h"
#include "editormanager.h"
#include "fancytabwidget.h"
#include "filemanager.h"
//#include "generalsettings.h"
#include "ifilefactory.h"
#include "messagemanager.h"
#include "modemanager.h"
#include "mimedatabase.h"
#include "newdialog.h"
#include "outputpane.h"
#include "plugindialog.h"
#include "progressmanager_p.h"
#include "progressview.h"
#include "shortcutsettings.h"
#include "vcsmanager.h"

#include "scriptmanager_p.h"
#include "settingsdialog.h"
#include "stylehelper.h"
#include "variablemanager.h"
//#include "versiondialog.h"
//#include "viewmanager.h"
#include "uniqueidmanager.h"
//#include "manhattanstyle.h"
#include "dialogs/iwizard.h"
#include "navigationwidget.h"
#include "rightpane.h"
#include "editormanager/ieditorfactory.h"
#include "baseview.h"
#include "basefilewizard.h"

#include <coreplugin/findplaceholder.h>
#include <extensionsystem/pluginmanager.h>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QtPlugin>
#include <QtCore/QUrl>

#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QMenu>
#include <QtGui/QPixmap>
#include <QtGui/QPrinter>
#include <QtGui/QShortcut>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWizard>

using namespace Core;
using namespace Core::Internal;

enum { debugMainWindow = 0 };

MainWindow::MainWindow() :
    QMainWindow(),
    m_coreImpl(new CoreImpl(this)),
    m_uniqueIDManager(new UniqueIDManager()),
	m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Nokia"), QLatin1String("QtCreator"), this)),
    m_printer(0),
    m_actionManager(new ActionManagerPrivate(this)),
    m_editorManager(0),
    m_fileManager(new FileManager(this)),
    m_progressManager(new ProgressManagerPrivate()),
    m_scriptManager(new ScriptManagerPrivate(this)),
    m_variableManager(new VariableManager(this)),
    m_vcsManager(new VCSManager()),
    m_viewManager(0),
    m_modeManager(0),
    m_mimeDatabase(new MimeDatabase)
{
	m_editorManager = new EditorManager(m_coreImpl, this);
	
	actionManager()->createMenu ("ProjectExplorer.Menu.Debug");
	
	// from projectexplorer.cpp
    // debug menu
	Core::ICore *core = Core::ICore::instance();
	Core::ActionManager *am = core->actionManager();
    Core::ActionContainer *menubar = am->createMenuBar(Constants::MENU_BAR);
	setMenuBar (menubar->menuBar());
    Core::ActionContainer *mdebug =
        am->createMenu(ProjectExplorer::Constants::M_DEBUG);
    mdebug->menu()->setTitle("&Debug");
    menubar->addMenu(mdebug);
#if 0
    Core::ActionContainer *mview =
        am->createMenu(ProjectExplorer::Constants::M_VIEW);
    mview->menu()->setTitle("&View");
    menubar->addMenu(mview);
#endif
}

MainWindow::~MainWindow()
{
    hide();
    delete m_settings;
    m_settings = 0;
    delete m_printer;
    m_printer = 0;

    delete m_uniqueIDManager;
    delete m_editorManager;
    delete m_vcsManager;
    m_vcsManager = 0;
    delete m_modeManager;
    delete m_coreImpl;
}

IContext *MainWindow::contextObject(QWidget *widget)
{
    return m_contextWidgets.value(widget);
}

void MainWindow::addContextObject(IContext *context)
{
    if (!context)
        return;
    QWidget *widget = context->widget();
    if (m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.insert(widget, context);
}

void MainWindow::removeContextObject(IContext *context)
{
    if (!context)
        return;

    QWidget *widget = context->widget();
    if (!m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.remove(widget);
    if (m_activeContext == context)
        updateContextObject(0);
}

void MainWindow::updateContextObject(IContext  *context)
{
    IContext *oldContext = m_activeContext;
    m_activeContext = context;
    if (!context || oldContext != m_activeContext) {
        emit m_coreImpl->contextAboutToChange(context);
        updateContext();
        if (debugMainWindow)
            qDebug() << "new context object =" << context << (context ? context->widget() : 0)
                     << (context ? context->widget()->metaObject()->className() : 0);
        emit m_coreImpl->contextChanged(context);
    }
}

void MainWindow::resetContext()
{
    updateContextObject(0);
}

ActionManager *MainWindow::actionManager() const
{
    return m_actionManager;
}

FileManager *MainWindow::fileManager() const
{
    return m_fileManager;
}

UniqueIDManager *MainWindow::uniqueIDManager() const
{
    return m_uniqueIDManager;
}

MessageManager *MainWindow::messageManager() const
{
    return m_messageManager;
}

VCSManager *MainWindow::vcsManager() const
{
    return m_vcsManager;
}

EditorManager *MainWindow::editorManager() const
{
    return m_editorManager;
}

ProgressManager *MainWindow::progressManager() const
{
    return m_progressManager;
}

ScriptManager *MainWindow::scriptManager() const
{
     return m_scriptManager;
}

VariableManager *MainWindow::variableManager() const
{
     return m_variableManager;
}

ModeManager *MainWindow::modeManager() const
{
    return m_modeManager;
}

MimeDatabase *MainWindow::mimeDatabase() const
{
    return m_mimeDatabase;
}

IContext *MainWindow::currentContextObject() const
{
    return m_activeContext;
}

QStatusBar *MainWindow::statusBar() const
{
    return m_modeStack->statusBar();
}

static QList<IFileFactory*> getNonEditorFileFactories()
{
    const QList<IFileFactory*> allFileFactories =
        ExtensionSystem::PluginManager::instance()->getObjects<IFileFactory>();
    QList<IFileFactory*> nonEditorFileFactories;
    foreach (IFileFactory *factory, allFileFactories) {
        if (!qobject_cast<IEditorFactory *>(factory))
            nonEditorFileFactories.append(factory);
    }
    return nonEditorFileFactories;
}

static IFileFactory *findFileFactory(const QList<IFileFactory*> &fileFactories,
                                     const MimeDatabase *db,
                                     const QFileInfo &fi)
{
    if (const MimeType mt = db->findByFile(fi)) {
        const QString type = mt.type();
        foreach (IFileFactory *factory, fileFactories) {
            if (factory->mimeTypes().contains(type))
                return factory;
        }
    }
    return 0;
}

// opens either an editor or loads a project
void MainWindow::openFiles(const QStringList &fileNames)
{
    bool needToSwitchToEditor = false;
    QList<IFileFactory*> nonEditorFileFactories = getNonEditorFileFactories();

    foreach (const QString &fileName, fileNames) {
        const QFileInfo fi(fileName);
        const QString absoluteFilePath = fi.absoluteFilePath();
        if (IFileFactory *fileFactory = findFileFactory(nonEditorFileFactories, mimeDatabase(), fi)) {
            fileFactory->open(absoluteFilePath);
        } else {
            IEditor *editor = editorManager()->openEditor(absoluteFilePath);
            if (editor)
                needToSwitchToEditor = true;
        }
    }
    if (needToSwitchToEditor)
        editorManager()->ensureEditorManagerVisible();
}

void MainWindow::addAdditionalContext(int context)
{
    if (context == 0)
        return;

    if (!m_additionalContexts.contains(context))
        m_additionalContexts.prepend(context);
}

void MainWindow::removeAdditionalContext(int context)
{
    if (context == 0)
        return;

    int index = m_additionalContexts.indexOf(context);
    if (index != -1)
        m_additionalContexts.removeAt(index);
}

bool MainWindow::hasContext(int context) const
{
    return m_actionManager->hasContext(context);
}

QStringList MainWindow::showNewItemDialog(const QString &title,
                                          const QList<IWizard *> &wizards,
                                          const QString &defaultLocation)
{
    QString defaultDir = defaultLocation;
    if (defaultDir.isEmpty()) {
        if (!m_coreImpl->fileManager()->currentFile().isEmpty())
            defaultDir = QFileInfo(m_coreImpl->fileManager()->currentFile()).absolutePath();
    }

    // Scan for wizards matching the filter and pick one. Don't show
    // dialog if there is only one.
    IWizard *wizard = 0;
    switch (wizards.size()) {
    case 0:
        break;
    case 1:
        wizard = wizards.front();
        break;
    default: {
        NewDialog dlg(this);
        dlg.setWizards(wizards);
        dlg.setWindowTitle(title);
        wizard = dlg.showDialog();
    }
        break;
    }

    if (!wizard)
        return QStringList();
    return wizard->runWizard(defaultDir, this);
}

void MainWindow::showOptionsDialog(const QString &category, const QString &page)
{
    emit m_coreImpl->optionsDialogRequested();
    SettingsDialog dlg(this, category, page);
    dlg.exec();
}

void MainWindow::updateContext()
{
    QList<int> contexts;

    if (m_activeContext)
        contexts += m_activeContext->context();
    IEditor *editor = m_editorManager->currentEditor();
    if (editor && (EditorManagerPlaceHolder::current() != 0)) {
        contexts += editor->context();
    }

    contexts += m_additionalContexts;

    QList<int> uniquecontexts;
    for (int i = 0; i < contexts.size(); ++i) {
        const int c = contexts.at(i);
        if (!uniquecontexts.contains(c))
            uniquecontexts << c;
    }

    m_actionManager->setContext(uniquecontexts);
}

QPrinter *MainWindow::printer() const
{
    if (!m_printer)
        m_printer = new QPrinter(QPrinter::HighResolution);
    return  m_printer;
}
