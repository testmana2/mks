#include <QDir>

#include "UITemplatesWizard.h"
#include "pFileManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"

#include "ProjectsModel.h"
#include "ProjectsProxy.h"
#include "UIProjectsManager.h"
#include "VariablesManager.h"

using namespace pMonkeyStudio;

UITemplatesWizard::UITemplatesWizard( QWidget* w )
    : QDialog( w )
{
    setupUi( this );
	mTemplates = pTemplatesManager::instance()->getTemplates();
    setAttribute( Qt::WA_DeleteOnClose );
    //fill available languages
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
	cbLanguages->addItems(langs);
	cbTypes->addItems (types);
	
	cbLanguages->addItem (tr("Any"));
	cbTypes->addItem (tr("Any"));
	
    // assign projects combobox
    mProjects = UIProjectsManager::instance()->model();
    cbProjects->setModel( mProjects->scopesProxy() );
    ProjectItem* p = UIProjectsManager::instance()->currentProject();
    cbProjects->setCurrentIndex( mProjects->scopesProxy()->mapFromSource( p ? p->index() : QModelIndex() ) );
    // show correct page
    //on_swPages_currentChanged( 0 );
    // restore infos
    pSettings* s = pSettings::instance();
    cbLanguages->setCurrentIndex( cbLanguages->findText( s->value( "Recents/FileWizard/Language", "C++" ).toString() ) );
    leDestination->setText( s->value( "Recents/FileWizard/Destination" ).toString() );
    //cbOpen->setChecked( s->value( "Recents/FileWizard/Open", true ).toBool() );
    //
    cbOperators->addItems( QStringList() << "=" << "+=" << "-=" << "*=" << "~=" );
	
	connect (lwTemplates, SIGNAL (itemPressed(QListWidgetItem*)), this, SLOT (onTemplateSelected(QListWidgetItem*)));
	connect (cbLanguages, SIGNAL (currentIndexChanged(int)), this, SLOT (onFiltersChanged()));
	connect (cbTypes, SIGNAL (currentIndexChanged(int)), this, SLOT (onFiltersChanged()));
	onFiltersChanged ();
}

void UITemplatesWizard::setType (QString t)
{
    cbTypes->setCurrentIndex( cbTypes->findData( t ) ); 
}

void UITemplatesWizard::onFiltersChanged()
{
	QString type = cbTypes->currentText();
	QString language = cbLanguages->currentText();
	
    // clear lwTemplates
    lwTemplates->clear();
    // create blank file
    QListWidgetItem* it = new QListWidgetItem( lwTemplates );
    it->setIcon( QIcon( ":/templates/icons/templates/empty.png" ) );
    it->setToolTip( tr( "Blank File" ) );
    it->setText( tr( "Blank" ) );
	it->setData( Qt::UserRole+1, -1);
    // create tempaltes
    for (int i = 0; i < mTemplates.size(); i++)
    {
		pTemplate t = mTemplates[i];
        if (  (	language == tr("Any") || t.Language == language) && 
				cbTypes->currentText() == tr("Any") || t.Type == type )
        {
            it = new QListWidgetItem( lwTemplates );
			QString icon = t.Icon.isEmpty() ? ":/templates/icons/templates/empty.png" : (t.DirPath + t.Icon);
            it->setIcon( QIcon(icon) );
            it->setToolTip( t.Description );
            it->setText( t.Name );
            it->setData( Qt::UserRole+1, i);
        }
    }
	gbInformations->setEnabled (false);
}

void UITemplatesWizard::onTemplateSelected (QListWidgetItem* it)
{
	while (mLabels.size())
	{
		delete mLabels.takeAt(0);
		delete mCombos.takeAt(0);
	}
	int i = it->data (Qt::UserRole+1).toInt();
	if (i == -1)
		return;
	int row = 1;
	foreach (QString var, mTemplates[i].Variables.keys())
	{
		QLabel* label = new QLabel (var);
		gridLayout->addWidget (label, row, 0);
		mLabels << label;
		QComboBox* combo = new QComboBox ();
		combo->setEditable (true);
		combo->addItems (mTemplates[i].Variables[var]);
		gridLayout->addWidget (combo, row++, 1);
		mCombos << combo;
	}
	gbInformations->setEnabled (true);
}

void UITemplatesWizard::on_tbDestination_clicked()
{
	QString s = getExistingDirectory( tr( "Select the file(s) destination" ), leDestination->text(), window() );
	if ( !s.isNull() )
		leDestination->setText( s );
}

bool UITemplatesWizard::checkTemplate()
{
    if ( ! QDir (leDestination->text()).exists())
    {
        information( tr( "Destination..." ), tr( "Destination dirrectory not right" ), this );
        return false;
    }
    return true;
}

void UITemplatesWizard::on_pbCreate_clicked()
{
	VariablesManager::Dictionary variables;
	variables["Destination"]=leDestination->text();
	int i = 0;
	
	while (i < mLabels.size())
	{
		variables [mLabels[i++]->text()] =  mCombos[i]->currentText();
	}
	int index = lwTemplates->currentItem ()->data( Qt::UserRole).toInt();	
	if ( ! pTemplatesManager::instance()->realiseTemplate (mTemplates[index], variables))
		return;
    // remember some infos
    pSettings* s = pSettings::instance();
    s->setValue( "Recents/FileWizard/Language", cbLanguages->currentText() );
    s->setValue( "Recents/FileWizard/Destination", leDestination->text() );
    s->setValue( "Recents/FileWizard/Open", cbOpen->isChecked() );
    // close dialog
    QDialog::accept();	
}
