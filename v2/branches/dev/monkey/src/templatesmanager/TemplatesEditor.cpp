/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : UITemplatesEditor
** FileName  : UITemplatesEditor.cpp
** Date      : 2007-11-11T01:25:28
** License   : GPL
** Comment   : Your comment here
** Home Page : 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "TemplatesEditor.h"
#include "pTemplatesManager.h"

TemplatesEditor::TemplatesEditor(QWidget* parent) :QDialog (parent)
{
	createGUI();
}

TemplatesEditor::~TemplatesEditor()
{
}

void TemplatesEditor::createGUI()
{
	QVBoxLayout* vbox = new QVBoxLayout ();
	
	QLabel* pathLabel = new QLabel (tr ("&Templates path"));
	vbox->addWidget (pathLabel);
	
	mTemplatesPath = new QComboBox ();
	pathLabel->setBuddy (mTemplatesPath);
	vbox->addWidget (mTemplatesPath);
	
	mTemplatesList = new pStringListEditor(tr("T&emplates list"));
	vbox->addWidget (mTemplatesList);
	
	mEditSpace = new QGroupBox (tr("Te&mplate"));
	QVBoxLayout* vbox1 = new QVBoxLayout ();
		QHBoxLayout* hbox1 = new QHBoxLayout ();
			QVBoxLayout* nameLayout = new QVBoxLayout ();
				QLabel* nameLabel = new QLabel (tr("&Name"));
				nameLayout->addWidget (nameLabel);

				mName = new QLineEdit ();
				nameLabel->setBuddy (mName);	
				nameLayout->addWidget (mName);
			hbox1->addLayout (nameLayout);
		
			QVBoxLayout* dirLayout = new QVBoxLayout ();
				QLabel* dirLabel = new QLabel (tr("&Directory name"));
				dirLayout->addWidget (dirLabel);
		
				mDirectoryName = new QLineEdit ();
				dirLabel->setBuddy (mDirectoryName);
				dirLayout->addWidget (mDirectoryName);
			hbox1->addLayout (dirLayout);
			
		vbox1->addLayout (hbox1);
		
		QLabel* iconLabel = new QLabel (tr("&Template icon"));
		vbox1->addWidget (iconLabel);
			
		QHBoxLayout* iconLayout = new QHBoxLayout ();
			mIcon = new QLineEdit ();
			iconLabel->setBuddy (mIcon);
			iconLayout->addWidget (mIcon);
			
			mIconBtn = new QPushButton ();
			mIconBtn->setIcon (QIcon(":/file/icons/file/open.png"));
			iconLayout->addWidget (mIconBtn);
		vbox1->addLayout (iconLayout);
		
		QHBoxLayout* hbox3 = new QHBoxLayout ();
			QVBoxLayout* descBox = new QVBoxLayout ();
				QLabel* descLabel = new QLabel (tr("Template description"));
				descBox->addWidget (descLabel);

				mDesctiption = new QTextEdit ();
				descBox->addWidget (mDesctiption);
			hbox3->addLayout (descBox);
		
			QVBoxLayout* filesBox = new QVBoxLayout ();
				mFiles = new pFileListEditor (tr("Template file(s)"), QFileDialog::ExistingFiles, "*");
				filesBox->addWidget (mFiles);
			hbox3->addLayout (filesBox);
		vbox1->addLayout (hbox3);
		
		QHBoxLayout* hbox4 = new QHBoxLayout ();
			mVariables= new pStringListEditor (tr("Template variables"));
			hbox4->addWidget (mVariables);
			
			paramsBox = new QGroupBox (tr("Variable parametres"));
			QVBoxLayout* varParamsLayout = new QVBoxLayout ();
				QLabel* fullNameLabel = new QLabel(tr("&Human readible name"));
				varParamsLayout->addWidget (fullNameLabel);

				mFullName = new QLineEdit ();
				fullNameLabel->setBuddy (mFullName);
				varParamsLayout->addWidget (mFullName);
				
				mValues= new pStringListEditor (tr("Variable values"));
				varParamsLayout->addWidget (mValues);
			paramsBox->setLayout (varParamsLayout);
			hbox4->addWidget (paramsBox);
		vbox1->addLayout (hbox4);

		QLabel* scriptLabel = new QLabel (tr("&Sctipt for configuring template"));
		vbox1->addWidget (scriptLabel);
		
		QHBoxLayout* scriptLayout = new QHBoxLayout ();
			mScript = new QLineEdit ();
			scriptLabel->setBuddy (mScript);
			scriptLayout->addWidget (mScript);
			
			mScriptBtn = new QPushButton ();
			mScriptBtn->setIcon (QIcon(":/file/icons/file/open.png"));
			scriptLayout->addWidget (mScriptBtn);
		vbox1->addLayout (scriptLayout);
		
	mEditSpace->setLayout (vbox1);
	mEditSpace->setEnabled (false);
	vbox->addWidget (mEditSpace);
	
	setLayout (vbox);
	
	//initialisation
	mTemplatesPath->addItems (pTemplatesManager::instance()->getTemplatesPath());
}

void TemplatesEditor::on_pathSelect (QString dir)
{
	TemplateList tl = pTemplatesManager::instance()->getTemplatesFromDir (dir);
	foreach (pTemplate t, tl)
		mTemplatesList->list->addItem (t.Name);
}

void TemplatesEditor::on_TemplateSelect (QString)
{
	
}



int TemplatesEditor::exec ()
{
	return QDialog::exec ();
}
