/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pStringListEditor.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PSTRINGLISTEDITOR__H
#define PSTRINGLISTEDITOR__H

#include <QGroupBox>
#include <QListWidget>

class pStringListEditor : public QGroupBox
{
    Q_OBJECT
public:
	pStringListEditor(QString title, QWidget* parent = NULL);
	virtual ~pStringListEditor();

    QListWidget* list;

protected slots:
	virtual void onActionTriggered (QAction*);

signals:
	void edited ();
};

#endif // PSTRINGLISTEDITOR__H
