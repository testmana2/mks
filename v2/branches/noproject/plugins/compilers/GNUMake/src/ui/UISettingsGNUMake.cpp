#include "UISettingsGNUMake.h"
#include "pSettings.h"
//
#include <QFileDialog>
#include <QFileInfo>
//
UISettingsGNUMake::UISettingsGNUMake( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	QString mMake = "make";
#if defined ( Q_WS_WIN )
	mMake = "mingw32-make";
#endif
	//
	leBinary->setText( pSettings::instance()->value( "Plugins/GNU Make/Binary", mMake ).toString() );
	lePath->setText( pSettings::instance()->value( "Plugins/GNU Make/Path" ).toString() );
}
//
void UISettingsGNUMake::on_tbBinary_clicked()
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
void UISettingsGNUMake::on_tbPath_clicked()
{
	QString mPath = QFileDialog::getExistingDirectory( this, tr( "Locate your make tool path" ) );
	if ( !mPath.isNull() )
		lePath->setText( mPath );
}
//
void UISettingsGNUMake::on_bbDialog_clicked( QAbstractButton* )
{
	pSettings::instance()->setValue( "Plugins/GNU Make/Binary", leBinary->text() );
	pSettings::instance()->setValue( "Plugins/GNU Make/Path", lePath->text() );
}
