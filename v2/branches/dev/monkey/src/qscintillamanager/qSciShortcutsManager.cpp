/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Kopats Andrei aka hlamer <hlamer at tut by>
** Project   : SciShortcutsManager
** FileName  : SciShortcutsManager.cpp
** Date      : 2007-10-27T10:52:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "qSciShortcutsManager.h"
#include "pMenuBar.h"
#include "pWorkspace.h"
#include "pEditor.h"
#include "pAbstractChild.h"

qSciShortcutsManager::qSciShortcutsManager ()
{
	//Fill with all availible QScintila actions
	sactions << (SciAction) { "mEdit/mAllCommands/SCI_LINEDOWN", tr( "Line down" ), QIcon(), QString(""), tr(""),QsciScintilla::SCI_LINEDOWN};
	foreach (SciAction sact, sactions)
	{
		QAction* qact = pMenuBar::instance()->action( sact.name, sact.text, sact.icon, sact.defaultShortcut, sact.toolTip);
		qact->setProperty ("messageCode", sact.messageCode);
		connect (qact, SIGNAL (triggered()), this, SLOT (keyBoardShortcutPressed ()));
	}
}

void qSciShortcutsManager::initEditor (pEditor* editor)
{
	foreach (SciAction sact, sactions)
	{
		//TODO Uncomment, when sactions will be filled
		//editor->SendScintilla (QsciScintilla::SCI_CLEARALLCMDKEYS);
	}
}

void qSciShortcutsManager::keyBoardShortcutPressed ()
{
	Q_ASSERT (sender());
	int messageCode = sender()->property("messageCode").toInt();
	Q_ASSERT (messageCode);
	pAbstractChild* child = pWorkspace::instance()->currentChild();
	if (child)
	{
		pEditor* editor = child->currentEditor ();
		if (editor)
		{
			editor->SendScintilla (messageCode);
		}
	}
}


