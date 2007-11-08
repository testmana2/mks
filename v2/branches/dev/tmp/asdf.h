/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : I am
** Project   : asdf
** FileName  : asdf.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef ASDF_H
#define ASDF_H

#include <QGroupBox>
#include <QListWidget>

class pStringListEditor : public QGroupBox
{
    Q_OBJECT
public:
	pStringListEditor(QWidget* parent = NULL);
	~pStringListEditor();

    QListWidget* list;

protected slots:
	void onActionTriggered (QAction*);
};

#endif // ASDF_H
