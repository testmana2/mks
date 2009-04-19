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

#include <QtCore/QSettings>

#include <coreplugin/coreconstants.h>
#include <projectexplorer/projectexplorerconstants.h>

#include "mainwindow.h"
#include "actioncontainer.h"
#include "actionmanager_p.h"
#include "basemode.h"
#include "editormanager.h"
#include "modemanager.h"
#include "uniqueidmanager.h"
#include "coreimpl.h"

#include <QtCore/QDebug>

using namespace Core;
using namespace Core::Internal;

MainWindow::MainWindow() :
    QMainWindow(),
	m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Nokia"), QLatin1String("QtCreator"), this)),
    m_coreImpl(new CoreImpl(this)),
    m_uniqueIDManager(new UniqueIDManager()),

    m_actionManager(new ActionManagerPrivate(this)),
    m_editorManager(0),
    m_modeManager(0)
{
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
    menubar->addMenu(mdebug, Core::Constants::G_VIEW);

}

MainWindow::~MainWindow()
{
    hide();
    delete m_settings;
    delete m_uniqueIDManager;
    delete m_editorManager;
    delete m_modeManager;
    delete m_coreImpl;
}

ActionManager *MainWindow::actionManager() const
{
    return m_actionManager;
}

UniqueIDManager *MainWindow::uniqueIDManager() const
{
    return m_uniqueIDManager;
}

EditorManager *MainWindow::editorManager() const
{
    return m_editorManager;
}

ModeManager *MainWindow::modeManager() const
{
    return m_modeManager;
}

void MainWindow::addAdditionalContext(int context)
{
    qDebug () << __FILE__ << __FUNCTION__;
}

void MainWindow::removeAdditionalContext(int context)
{
    qDebug () << __FILE__ << __FUNCTION__;
}

void MainWindow::updateContext()
{
    qDebug () << __FILE__ << __FUNCTION__;
}
