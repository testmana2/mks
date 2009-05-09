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

#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QtGui/QWidget>

namespace Core {

class ICore;
class IEditor;
class IMode;

class EditorManager : public QWidget
{
    Q_OBJECT

public:
	explicit EditorManager(ICore *core, QWidget *parent);
	
	static EditorManager *instance() { return m_instance; }
    
    IEditor *currentEditor() const;
    QList<IEditor*> openedEditors() const;	
	
	QByteArray saveState() const;
    bool restoreState(const QByteArray &state);

signals:
    void editorOpened(Core::IEditor *editor);
    void editorAboutToClose(Core::IEditor *editor);
	
private:
    static EditorManager *m_instance;
};

class EditorManagerPlaceHolder : public QWidget
{
    Q_OBJECT
public:
    EditorManagerPlaceHolder(Core::IMode *mode, QWidget *parent = 0);
    
};

} // namespace Core

#endif // EDITORMANAGER_H
