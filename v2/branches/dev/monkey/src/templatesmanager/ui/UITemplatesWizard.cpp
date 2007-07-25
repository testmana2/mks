#include "UITemplatesWizard.h"
#include "pQScintilla.h"
#include "pTemplatePreviewer.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

UITemplatesWizard::UITemplatesWizard( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	// fill available languages
	cbLanguages->addItems( pQScintilla::instance()->languages() );

	// show correct page
	on_swPages_currentChanged( 0 );
}

UITemplatesWizard::~UITemplatesWizard()
{
}

void UITemplatesWizard::on_cbLanguages_currentIndexChanged( const QString& s )
{
	// clear lwTemplates
	lwTemplates->clear();
	// get language template
	QList<pTemplate*> tl = pQScintilla::instance()->templates();
	foreach ( pTemplate* t, tl )
	{
		if ( t->Language == s )
		{
			QListWidgetItem* it = new QListWidgetItem( lwTemplates );
			it->setIcon( QIcon( t->Icon ) );
			it->setToolTip( t->Description );
			it->setText( t->Name );
			it->setData( Qt::UserRole +1, t->FileName );
		}
	}
	// clear pointer
	qDeleteAll( tl );
}

void UITemplatesWizard::on_swPages_currentChanged( int i )
{
	switch ( i )
	{
		case 0:
			lInformations->setText( tr( "Templates" ) );
			break;
		case 1:
			lInformations->setText( tr( "Templates Preview" ) );
			generatePreview();
			break;
	}

	// set correct text/state to buttons
	pbPrevious->setEnabled( i != 0 );
	pbNext->setText( tr( "Next" ) );
	if ( swPages->currentIndex() == swPages->count() -1 )
		pbNext->setText( tr( "Finish" ) );
}

void UITemplatesWizard::on_pbPrevious_clicked()
{
	swPages->setCurrentIndex( swPages->currentIndex() -1 );
}

void UITemplatesWizard::on_pbNext_clicked()
{
	if ( !checkTemplates() )
		return;

	int i = swPages->currentIndex();

	if ( i < swPages->count() -1 )
		swPages->setCurrentIndex( i +1 );

	if ( i == swPages->count() -1 )
		accept();
}

bool UITemplatesWizard::checkTemplates()
{
	if ( !lwTemplates->selectedItems().count() )
	{
		QMessageBox::information( this, tr( "Templates..." ), tr( "Choose at least one template to continue." ) );
		return false;
	}

	if ( leBaseName->text().isEmpty() )
	{
		QMessageBox::information( this, tr( "Base Name..." ), tr( "Choose a base name for your file(s)." ) );
		return false;
	}

	if ( leDestination->text().isEmpty() )
	{
		QMessageBox::information( this, tr( "Destination..." ), tr( "Choose a destination for your file(s)." ) );
		return false;
	}

	return true;
}

void UITemplatesWizard::generatePreview()
{
	foreach ( QListWidgetItem* it, lwTemplates->selectedItems() )
	{
		QString s = pQScintilla::instance()->templatesPath().replace( "%HOME%", QDir::homePath() );
		s = QString( it->data( Qt::UserRole +1 ).toString() ).replace( "%TEMPLATE_PATH%", s );
		pTemplatePreviewer* p = new pTemplatePreviewer;
		p->setLanguage( cbLanguages->currentText() );
		p->setName( it->text() );
		p->setText( leBaseName->text().append( QString( ".%1" ).arg( QFileInfo( s ).suffix() ) ) );
		p->open( s );
		sView->addWidget( p );
	}
}

void UITemplatesWizard::accept()
{
	QDialog::accept();
}
