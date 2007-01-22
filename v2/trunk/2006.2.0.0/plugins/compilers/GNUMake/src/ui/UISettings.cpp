#include "UISettings.h"
#include "Settings.h"
//
#include <QFileDialog>
#include <QFileInfo>
//
UISettings::UISettings( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	leBinary->setText( Settings::current()->value( "Plugins/GNU Make/Binary" ).toString() );
	lePath->setText( Settings::current()->value( "Plugins/GNU Make/Path" ).toString() );
}
//
void UISettings::on_tbBinary_clicked()
{
	QString mFilePath = QFileDialog::getOpenFileName( this, tr( "Choose your make tool binary" ) );
	if ( !mFilePath.isNull() )
	{
		QFileInfo f( mFilePath );
		leBinary->setText( f.fileName() );
		lePath->setText( f.path() );
	}
}
//
void UISettings::on_tbPath_clicked()
{
	QString mPath = QFileDialog::getExistingDirectory( this, tr( "Locate your make tool path" ) );
	if ( !mPath.isNull() )
		lePath->setText( mPath );
}
//
void UISettings::on_bbDialog_clicked( QAbstractButton* )
{
	Settings::current()->setValue( "Plugins/GNU Make/Binary", leBinary->text() );
	Settings::current()->setValue( "Plugins/GNU Make/Path", lePath->text() );
}
