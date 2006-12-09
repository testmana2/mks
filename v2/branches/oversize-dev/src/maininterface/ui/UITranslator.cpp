#include "UITranslator.h"
#include "Settings.h"
//
#include <QLibraryInfo>
#include <QFileDialog>
#include <QLocale>
//
QPointer<UITranslator> UITranslator::mSelf = 0L;
//
UITranslator* UITranslator::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UITranslator( p );
	return mSelf;
}
//
UITranslator::UITranslator( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	mTranslationsPath = Settings::current()->value( "Paths/Translations", QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ).toString();
	on_tbReload_clicked();
}
//
void UITranslator::accept()
{
	if ( lwTranslations->currentItem() )
	{
		Settings::current()->setValue( "Language", lwTranslations->currentItem()->text() );
		Settings::current()->setValue( "Paths/Translations", mTranslationsPath );
		QDialog::accept();
	}
}
//
void UITranslator::on_tbLocate_clicked()
{
	QString s = QFileDialog::getExistingDirectory( this, tr( "Choose the location of your translations" ), mTranslationsPath );
	if ( !s.isEmpty() )
	{
		mTranslationsPath = s;
		on_tbReload_clicked();
	}
}
//
void UITranslator::on_tbReload_clicked()
{
	lwTranslations->clear();
	QDir d( mTranslationsPath );
	if ( !d.exists() )
		return;
	QStringList l = d.entryList( QStringList() << "monkey2_*.qm", QDir::Files | QDir::Readable );
	lwTranslations->addItem( "english" );
	foreach ( QString s, l )
		lwTranslations->addItem( s.remove( "monkey2_" ).remove( ".qm" ) );
	QList<QListWidgetItem*> li = lwTranslations->findItems( QLocale::languageToString( QLocale::system().language() ), Qt::MatchFixedString );
	if ( li.count() )
		lwTranslations->setCurrentItem( li.at( 0 ) );
}
