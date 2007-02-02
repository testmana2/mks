#include "UISettingsQMake.h"
#include "Settings.h"
//
#include <QFileDialog>
#include <QFileInfo>
//
UISettingsQMake::UISettingsQMake( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// create items
	// qt4
	cbKeywords->addItem( QIcon( ":/Icons/Icons/folder_blue.png" ), "Qt4 Path", PathType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "QMake4", BinaryType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "lupdate4", BinaryType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "lrelease4", BinaryType );
	// qt3
	cbKeywords->addItem( QIcon( ":/Icons/Icons/folder_blue.png" ), "Qt3 Path", PathType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "QMake3", BinaryType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "lupdate3", BinaryType );
	cbKeywords->addItem( QIcon( ":/Icons/Icons/misc.png" ), "lrelease3", BinaryType );
	//
	loadSettings();
}
//
void UISettingsQMake::loadSettings()
{
	for ( int i = 0; i < cbKeywords->count(); i++ )
		cbKeywords->setItemData( i, Settings::current()->value( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ) ).toString(), DataRole );
	//
	if ( cbKeywords->count() )
		on_cbKeywords_currentIndexChanged( 0 );
}
//
void UISettingsQMake::on_cbKeywords_currentIndexChanged( int i )
{
	lePath->setText( cbKeywords->itemData( i, DataRole ).toString() );
}
//
void UISettingsQMake::on_lePath_editingFinished()
{
	cbKeywords->setItemData( cbKeywords->currentIndex(), lePath->text(), DataRole );
}
//
void UISettingsQMake::on_tbBrowse_clicked()
{
	QString s = cbKeywords->currentText();
	switch ( cbKeywords->itemData( cbKeywords->currentIndex(), TypeRole ).toInt() )
	{
	case BinaryType:
		s = QFileDialog::getOpenFileName( this, tr( "Choose your tool binary" ), s );
		if ( !s.isNull() )
			lePath->setText( s );
		break;
	case PathType:
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your tool path" ), s );
		if ( !s.isNull() )
			lePath->setText( s );
		break;
	}
}
//
void UISettingsQMake::on_bbDialog_clicked( QAbstractButton* )
{
	for ( int i = 0; i < cbKeywords->count(); i++ )
		Settings::current()->setValue( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ), cbKeywords->itemData( i, DataRole ).toString() );
}
