/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : TemplatesEditor.h
** FileName  : TemplatesEditor.h.h
** Date      : 2007-11-11T01:24:12
** License   : GPL
** Comment   : Your comment here
** Home Page : 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef TEMPLATESEDITOR_H
#define TEMPLATESEDITOR_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QGroupBox>
#include "pFileListEditor.h"
#include "pStringListEditor.h"
#include "pTemplatesManager.h"
#include "pAbstractChild.h"

class TemplatesEditor : public pAbstractChild
{
Q_OBJECT
public:
	TemplatesEditor();
	~TemplatesEditor();

private:
	QComboBox* mTemplatesPath;
	pStringListEditor* mTemplatesList;
	QGroupBox* mEditSpace;
	QLineEdit* mName;
	QLineEdit* mIcon;
	QPushButton* mIconBtn;
	QTextEdit* mDescription;
	QComboBox* mLanguage;
	QComboBox* mType;
	pFileListEditor* mFiles;
	QGroupBox* paramsBox;
	pStringListEditor* mVariables;
	pStringListEditor* mValues;
	QLineEdit* mFullName;
	QLineEdit* mScript;
	QPushButton* mScriptBtn;

	void createGUI ();
private slots:
	void on_pathSelect (QString);
	void on_TemplateSelect (QString);
};

#endif // TEMPLATESEDITOR_H
