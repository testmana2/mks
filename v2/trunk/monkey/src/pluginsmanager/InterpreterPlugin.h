/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : InterpreterPlugin.h
** Date      : 2007-11-04T22:45:43
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef INTERPRETERPLUGIN_H
#define INTERPRETERPLUGIN_H

#include "BasePlugin.h"

class Q_MONKEY_EXPORT InterpreterPlugin : public BasePlugin
{
};

Q_DECLARE_INTERFACE( InterpreterPlugin, "org.monkeystudio.MonkeyStudio.InterpreterPlugin/1.0" )

#endif // INTERPRETERPLUGIN_H
