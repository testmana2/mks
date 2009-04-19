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

QT_BEGIN_NAMESPACE
class QSettings;
class QShortcut;
QT_END_NAMESPACE


namespace Core {

class ActionManager;
class EditorManager;
class ModeManager;
class UniqueIDManager;

namespace Internal {

class CoreImpl;
class ActionManagerPrivate;

class CORE_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    
    
    Core::ActionManager *actionManager() const;
    Core::UniqueIDManager *uniqueIDManager() const;
    Core::EditorManager *editorManager() const;
    Core::ModeManager *modeManager() const;

    inline QSettings *settings() const { return m_settings; }
    
    void addAdditionalContext(int context);
    void removeAdditionalContext(int context);

    void updateContext();

private:
	QSettings *m_settings;
    CoreImpl *m_coreImpl;
    UniqueIDManager *m_uniqueIDManager;
    ActionManagerPrivate *m_actionManager;
    EditorManager *m_editorManager;
    ModeManager *m_modeManager;
    
};

} // namespace Internal
} // namespace Core

#endif // MAINWINDOW_H
