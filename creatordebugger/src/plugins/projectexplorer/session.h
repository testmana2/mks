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

#ifndef SESSION_H
#define SESSION_H

#include <QtCore/QObject>

namespace Core {
class ICore;
}


namespace ProjectExplorer {

namespace Internal {
class SessionFile;
} // namespace Internal


// This could be improved.
class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = 0);
	
    // creates a new default session and switches to it
    void createAndLoadNewDefaultSession();
	
    bool save();
    bool clear();
	
	// Let other plugins store persistent values within the session file
	void setValue(const QString &name, const QVariant &value);
	QVariant value(const QString &name);
	
private:
	bool createImpl(const QString &fileName);
    void updateName(const QString &session);
	QString sessionNameToFileName(const QString &session);
	
	Internal::SessionFile *m_file;
    Core::ICore *m_core;
	
    QString m_sessionName;
};

} // namespace ProjectExplorer

#endif // SESSION_H
