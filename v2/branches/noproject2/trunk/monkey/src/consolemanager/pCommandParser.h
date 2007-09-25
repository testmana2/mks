/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : P@sNox
** Project   : pCommandParser
** FileName  : pCommandParser.h
** Date      : jeu. 23. août 16:18:37 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PCOMMANDPARSER_H
#define PCOMMANDPARSER_H

#include "MonkeyExport.h"
#include "pConsoleManager.h"

#include <QObject>


class Q_MONKEY_EXPORT pCommandParser : public QObject
{
Q_OBJECT

public:
	pCommandParser() {}
	virtual ~pCommandParser() {}

public:
	//will try to parse presended output, and return true, if it's succesfully parsed.
	virtual bool parse(const QByteArray* output) {return false;};
    
    virtual QString name() const {return QString::null;};
signals:
	void newMessageAvailible (const pConsoleManager::Message);
};

#endif // PCOMMANDPARSER_H
