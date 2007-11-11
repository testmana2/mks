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

		QLabel* descLabel = new QLabel (tr("&Description"));
		vbox1->addWidget (descLabel);

		mDesctiption = new QTextEdit ();
		vbox1->addWidget (mDesctiption);
		
		QHBoxLayout* hbox4 = new QHBoxLayout ();
			mFiles = new pFileListEditor (tr("Template file(s)"), QFileDialog::ExistingFiles, "*");
			hbox4->addWidget (mFiles);
			
			QVBoxLayout* varLayout = new QVBoxLayout ();
				mVariables= new pStringListEditor (tr("Template variables"));
				varLayout->addWidget (mVariables);
				
				QLabel* fullNameLabel = new QLabel(tr("&Human readible name"));
				varLayout->addWidget (fullNameLabel);
				
				mFullName = new QLineEdit ();
				fullNameLabel->setBuddy (mFullName);
				varLayout->addWidget (mFullName);
			hbox4->addLayout (varLayout);
		vbox1->addLayout (hbox4);
	mEditSpace->setLayout (vbox1);
	mEditSpace->setEnabled (false);
	vbox->addWidget (mEditSpace);
	
	setLayout (vbox);
	
	//initialisation
	mTemplatesPath->addItems (pTemplatesManager::instance()->getTemplatesPath());
}

void TemplatesEditor::on_pathSelect (QString dir)
{
	TemplatesList tl = pTemplatesManager::instance()->getTemplatesFromDir (dir);
	foreach (pTemplate, tl)
		mTemplatesList->addItem
}

void TemplatesEditor::on_TemplateSelect (QString)
{
	
}



int TemplatesEditor::exec ()
{
	return QDialog::exec ();
}
