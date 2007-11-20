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
#include <QScrollArea>
#include <QDebug>

#include "TemplatesEditor.h"
#include "pTemplatesManager.h"

TemplatesEditor::TemplatesEditor() :pAbstractChild()
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
	mTemplatesList->setFixedHeight (170);
	vbox->addWidget (mTemplatesList);
	
	mEditSpace = new QGroupBox (tr("Te&mplate"));
	QGridLayout* box = new QGridLayout ();

/*		QHBoxLayout* hbox1 = new QHBoxLayout ();
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
*/		
		QLabel* typeLabel = new QLabel (tr("Template type"));
		box->addWidget (typeLabel,0,0,1,2);
				
		mType = new QComboBox ();
		mType->setEditable (true);
		typeLabel->setBuddy (mType);
		box->addWidget (mType,1,0,1,2);	
		
		QLabel* languageLabel = new QLabel (tr("Template language"));
		box->addWidget (languageLabel,0,2,1,2);
				
		mLanguage = new QComboBox ();
		mLanguage->setEditable (true);
		languageLabel->setBuddy (mLanguage);
		box->addWidget (mLanguage,1,2,1,2);		
		
		QLabel* iconLabel = new QLabel (tr("&Template icon"));
		box->addWidget (iconLabel,2,0);
				
		mIcon = new QLineEdit ();
		iconLabel->setBuddy (mIcon);
		box->addWidget (mIcon,3,0);
				
		mIconBtn = new QPushButton ();
		mIconBtn->setIcon (QIcon(":/file/icons/file/open.png"));
		box->addWidget (mIconBtn,3,1);
			
		QLabel* scriptLabel = new QLabel (tr("&Sctipt for configuring template"));
		box->addWidget (scriptLabel,2,2);

		mScript = new QLineEdit ();
		scriptLabel->setBuddy (mScript);
		box->addWidget (mScript,3,2);
				
		mScriptBtn = new QPushButton ();
		mScriptBtn->setIcon (QIcon(":/file/icons/file/open.png"));
		box->addWidget (mScriptBtn,3,3);
		
		QLabel* descLabel = new QLabel (tr("Template description"));
		box->addWidget (descLabel,4,0);

		mDescription = new QTextEdit ();
		box->addWidget (mDescription,5,0,1,2);
		
		mFiles = new pFileListEditor (tr("Template file(s)"), QFileDialog::ExistingFiles, "*");
		box->addWidget (mFiles,4,2,2,2);
		
		mVariables= new pStringListEditor (tr("Template variables"));
		box->addWidget (mVariables,6,0,1,2);
			
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
		box->addWidget (paramsBox,6,2,1,2);
	mEditSpace->setLayout (box);
	QScrollArea* scroll = new QScrollArea ();
	scroll->setWidget (mEditSpace);
	mEditSpace->setEnabled (false);
	vbox->addWidget (scroll);

	setLayout (vbox);
	
	//connect (mName, SIGNAL (textEdited(QString)), this, SLOT (on_TemplateEditing()));	
	connect (mIcon, SIGNAL (textEdited(QString)), this, SLOT (on_TemplateEditing()));	
	connect (mDescription, SIGNAL (textChanged ()), this, SLOT (on_TemplateEditing()));	
	connect (mLanguage, SIGNAL (activated (int)), this, SLOT (on_TemplateEditing()));	
	connect (mType, SIGNAL (activated (int)), this, SLOT (on_TemplateEditing()));	
	connect (mFiles, SIGNAL (edited()), this, SLOT (on_TemplateEditing()));	
	connect (mVariables, SIGNAL (edited()), this, SLOT (on_TemplateEditing()));	
	connect (mValues, SIGNAL (edited()), this, SLOT (on_TemplateEditing()));	
	connect (mFullName, SIGNAL (textEdited(QString)), this, SLOT (on_TemplateEditing()));	
	connect (mScript, SIGNAL (textEdited(QString)), this, SLOT (on_TemplateEditing()));	

	
	connect (mTemplatesPath, SIGNAL (activated(QString)), this, SLOT (on_pathSelect (QString)));	
	connect (mTemplatesList->list, SIGNAL (currentTextChanged(QString)), this, SLOT (on_TemplateSelect (QString)));	
	//initialisation
	mTemplatesPath->addItems (pTemplatesManager::instance()->getTemplatesPath());
	on_pathSelect (mTemplatesPath->currentText());
	
	connect (this, SIGNAL (ch_undoAvailableChanged(bool)), SIGNAL (undoAvailableChanged(bool)));
	connect (this, SIGNAL (ch_modifiedChanged(bool)), SIGNAL (modifiedChanged(bool)));
	
	QStringList langs;
	QStringList types;
	TemplateList tl = pTemplatesManager::instance()->getTemplates ();
	foreach (pTemplate t, tl)
	{
		if (not langs.contains (t.Language))
			langs << t.Language;
		if (not types.contains (t.Type))
			types << t.Type;
	}
	mLanguage->addItems (langs);
	mType->addItems (types);
}

void TemplatesEditor::on_pathSelect (QString dir)
{
	qWarning () << "On template select";
	mTemplatesList->list->clear();
	foreach (QString name, pTemplatesManager::instance()->getTemplatesNames (dir))
		mTemplatesList->list->addItem (name);
}

void TemplatesEditor::on_TemplateSelect (QString name)
{
	pTemplate templ = pTemplatesManager::instance()->
		getTemplate (mTemplatesPath->currentText() + "/" + name);
	mEditSpace->setEnabled (true);
	mIcon->setText (templ.Icon);
	mScript->setText (templ.Script);
	mDescription->setText (templ.Description);
	mFiles->list->clear();
	mFiles->list->addItems (templ.Files);
	mVariables->list->clear();
	mVariables->list->addItems (templ.Variables.keys());
}

void TemplatesEditor::on_TemplateEditing ()
{
	emit ch_modifiedChanged(true);
	emit ch_undoAvailableChanged(true);
}

void TemplatesEditor::saveCurrentFile()
{
	QStringList files;
	for (int i = 0; i< mFiles->list->count(); i++)
	{
		files << mFiles->list->item(i)->text();
	}
	
	pTemplate t	 ={ mTemplatesList->list->currentItem()->text(),
					mLanguage->currentText (),
					mType->currentText(),
					mDescription->toPlainText(),
					mIcon->text(),
					mScript->text(),
					mTemplatesPath->currentText(),
					files,
					QHash <QString, QStringList>()
	};
	qWarning () << mDescription->toPlainText();
	qWarning () << t.Description;
	
	
	qWarning () << "Save template";
	
	pTemplatesManager::instance()->setTemplate (t);

	emit ch_modifiedChanged(false);
	emit ch_undoAvailableChanged(false);	
}
