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

#include "modemanager.h"
#include "mainwindow.h"
#include <extensionsystem/pluginmanager.h>

#include <QtCore/QDebug>

using namespace Core;
using namespace Core::Internal;

ModeManager *ModeManager::m_instance = 0;

ModeManager::ModeManager(Internal::MainWindow *mainWindow):
	m_debugmode (0)
{
    m_instance = this;
	QObject::connect(ExtensionSystem::PluginManager::instance(), SIGNAL(objectAdded(QObject*)),
                     this, SLOT(objectAdded(QObject*)));
}


ModeManager* ModeManager::instance()
{
	return m_instance;
}

IMode *ModeManager::currentMode() const
{
	qDebug () << __FILE__ << __FUNCTION__;
    return NULL;
}

IMode *ModeManager::mode(const QString &/*id*/) const
{
    qDebug () << __FILE__ << __FUNCTION__;
    return NULL;
}

void ModeManager::activateMode(const QString &/*id*/)
{
	qDebug () << __FILE__ << __FUNCTION__;
}

void ModeManager::objectAdded(QObject *obj)
{
    IMode *mode = Aggregation::query<IMode>(obj);
    if (!mode)
        return;
	
	m_debugmode = mode;
	qDebug () << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ registered mode";
    //m_mainWindow->addContextObject(mode);
}
