#include "TranslationDialog.h"
#include "ui_TranslationDialog.h"
#include "TranslationManager.h"
#include "pIconManager.h"

#include <QFileDialog>
#include <QDebug>

TranslationDialog::TranslationDialog( TranslationManager* translationManager, QWidget* parent )
	: QDialog( parent )
{
	Q_ASSERT( translationManager );
	mTranslationManager = translationManager;
	
	//setAttribute( Qt::WA_DeleteOnClose );
	
	ui = new Ui::TranslationDialog;
	ui->setupUi( this );
	ui->tbReload->click();
}

TranslationDialog::~TranslationDialog()
{
	delete ui;
}

QString TranslationDialog::selectedLocale() const
{
	const QTreeWidgetItem* currentItem = ui->twLocales->selectedItems().value( 0 );
	return currentItem ? currentItem->data( 0, Qt::UserRole ).toString() : mTranslationManager->currentLocale().name();
}

QString TranslationDialog::getLocale( TranslationManager* translationManager, QWidget* parent )
{
	TranslationDialog dlg( translationManager, parent );
	
	if ( dlg.exec() == QDialog::Accepted ) {
		return dlg.selectedLocale();
	}
	
	return QString::null;
}

void TranslationDialog::on_tbLocate_clicked()
{
	const QString path = QFileDialog::getExistingDirectory( this, tr( "Choose a folder containg your application translations" ), mTranslationManager->translationsPath() );
	
	if ( path.isNull() ) {
		return;
	}
	
	mTranslationManager->setTranslationsPath( path );
	ui->tbReload->click();
}

QTreeWidgetItem* TranslationDialog::newItem( const QLocale& locale )
{
	const QString language = QLocale::languageToString( locale.language() );
	const QString country = QLocale::countryToString( locale.country() );
	const QString countryCode = locale.name().section( '_', 1 );
	
	QTreeWidgetItem* item = new QTreeWidgetItem;
	item->setIcon( 0, pIconManager::icon( QString( "%1.png" ).arg( countryCode.toLower() ), ":/country-flags" ) );
	item->setText( 0, QString( "%1 (%2)" ).arg( language ).arg( country ) );
	item->setData( 0, Qt::UserRole, locale.name() );
	return item;
}

QTreeWidgetItem* TranslationDialog::rootItem( const QLocale& locale )
{
	QTreeWidgetItem* item = mRootItems.value( locale.name() );
	
	if ( !item ) {
		item = newItem( locale );
		ui->twLocales->addTopLevelItem( item );
		mRootItems[ locale.name() ] = item;
	}
	
	return item;
}

void TranslationDialog::on_tbReload_clicked()
{
	// reload translations if needed
	if ( mTranslationManager->availableLocales().isEmpty() ) {
		mTranslationManager->reloadTranslations();
	}
	
	// keep current locale
	const QString currentLocale = selectedLocale();
	
	// clear items
	ui->twLocales->clear();
	mRootItems.clear();
	
	// create new ones
	foreach ( const QLocale& locale, mTranslationManager->availableQLocales() ) {
		QTreeWidgetItem* rootItem = this->rootItem( QLocale( locale.language() ) );
		
		if ( rootItem->data( 0, Qt::UserRole ).toString() == locale.name() ) {
			continue;
		}
		
		rootItem->addChild( newItem( locale ) );
	}
	
	// sort items
	ui->twLocales->sortByColumn( 0, Qt::AscendingOrder );
	
	// restore locale
	QAbstractItemModel* model = ui->twLocales->model();
	QModelIndex index = model->match( model->index( 0, 0 ), Qt::UserRole, currentLocale, 1, Qt::MatchFixedString | Qt::MatchWrap ).value( 0 );
	
	if ( !index.isValid() ) {
		index = model->match( model->index( 0, 0 ), Qt::UserRole, currentLocale, 1, Qt::MatchStartsWith | Qt::MatchWrap ).value( 0 );
	}
	
	ui->twLocales->setCurrentIndex( index );
}
