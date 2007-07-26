#include "UITemplatesWizard.h"
#include "pQScintilla.h"
#include "pTemplatePreviewer.h"
#include "pEditor.h"
#include "pFileManager.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>

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
	QListWidgetItem* it;

	// create blank file
	it = new QListWidgetItem( lwTemplates );
	it->setIcon( QIcon( ":/file/icons/file/new.png" ) );
	it->setToolTip( tr( "Blank File" ) );
	it->setText( tr( "Blank" ) );
	it->setData( Qt::UserRole +1, QString() );

	foreach ( pTemplate* t, tl )
	{
		if ( t->Language == s )
		{
			it = new QListWidgetItem( lwTemplates );
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

void UITemplatesWizard::on_tbDestination_clicked()
{
	QString s = QFileDialog::getExistingDirectory( window(), tr( "Select the file(s) destination" ), leDestination->text() );
	if ( !s.isNull() )
		leDestination->setText( s );
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

	if ( leClassName->text().isEmpty() )
	{
		QMessageBox::information( this, tr( "Class Name..." ), tr( "Choose a class name for your file(s)." ) );
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
	// delete all existing preview
	qDeleteAll( sView->findChildren<pTemplatePreviewer*>() );

	// create new preview
	foreach ( QListWidgetItem* it, lwTemplates->selectedItems() )
	{
		QString s = pQScintilla::instance()->templatesPath().replace( "%HOME%", QDir::homePath() );
		s = QString( it->data( Qt::UserRole +1 ).toString() ).replace( "%TEMPLATE_PATH%", s );
		pTemplatePreviewer* p = new pTemplatePreviewer;
		if ( it->data( Qt::UserRole +1 ).toString().isEmpty() )
		{
			p->setFileName( leClassName->text() );
			if ( it->text() == tr( "Blank" ) )
				p->setOpen( true );
		}
		else
		{
			p->setFileName( leClassName->text().append( QString( ".%1" ).arg( QFileInfo( s ).suffix() ) ) );
			p->open( s );
		}
		p->setDestination( leDestination->text() );

		// replace keywords
		QString c = p->editor()->text();
		// %PROJECTNAME%
		c.replace( "%PROJECTNAME%", cbProjects->currentText(), Qt::CaseInsensitive );
		// %AUTHOR%
		c.replace( "%AUTHOR%", leAuthor->text(), Qt::CaseInsensitive );
		// %FILENAME%
		c.replace( "%FILENAME%", p->fileName(), Qt::CaseInsensitive );
		// %DATE%
		c.replace( "%DATE%", QDateTime::currentDateTime().toString( "yyyy/MM/dd" ), Qt::CaseInsensitive );
		// %TIME%
		c.replace( "%TIME%", QDateTime::currentDateTime().toString( "hh:mm" ), Qt::CaseInsensitive );
		// %LICENSE%
		c.replace( "%LICENSE%", cbLicenses->currentText(), Qt::CaseInsensitive );
		// %COMMENT%
		c.replace( "%COMMENT%", tr( "Your comment here" ), Qt::CaseInsensitive );
		// %CLASSNAME%
		c.replace( "%CLASSNAME%", leClassName->text(), Qt::CaseInsensitive );
		// %UPPERCLASSNAME%
		c.replace( "%UPPERCLASSNAME%", leClassName->text().toUpper(), Qt::CaseInsensitive );
		// %HERITS%
		c.replace( "%HERITS%", cbHerits->currentText(), Qt::CaseInsensitive );
		// set text
		p->editor()->setText( c.isEmpty() ? "\n" : c );
		// add widget to splitter
		sView->addWidget( p );
	}
}

void UITemplatesWizard::accept()
{
	// create files
	foreach ( pTemplatePreviewer* p, sView->findChildren<pTemplatePreviewer*>() )
	{
		// if open
		if ( p->open() )
		{
			// set filename
			QString s = QDir::cleanPath( p->destination().append( "/" ).append( p->fileName() ) );
			// if can save
			if ( p->editor()->saveFile( s ) )
			{
				// open file in ide
				pFileManager::instance()->openFile( QFileInfo( s ).canonicalFilePath() );
				// add to project if needed
				if ( cbAddToProject->isChecked() )
				{
					//
				}
			}
		}
	}
	
	QDialog::accept();
}
