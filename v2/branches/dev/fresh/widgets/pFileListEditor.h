/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pFileListEditor.h
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PFILELISTEDITOR__H
#define PFILELISTEDITOR__H

#include <QFileDialog>

#include "pStringListEditor.h"

class pFileListEditor: public pStringListEditor
{
    Q_OBJECT
public:
	pFileListEditor(QString title, QFileDialog::FileMode fm, QString filter, QWidget* parent = NULL);
	virtual  ~pFileListEditor();
	void setDir (QString);

protected slots:
	void onActionTriggered (QAction*);
private:
	QString mFilter;
	QString mDir;
	QFileDialog::FileMode mFileMode;
};

#endif //PFILELISTEDITOR__H
