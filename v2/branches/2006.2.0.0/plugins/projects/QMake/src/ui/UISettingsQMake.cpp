#include "UISettingsQMake.h"
#include "Settings.h"
// qscintilla include
#include <qscilexercpp.h>
#include <qsciapis.h>
//
#include <QFileDialog>
#include <QFileInfo>
//
UISettingsQMake::UISettingsQMake( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	pbProgress->hide();
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
void UISettingsQMake::recursiveFiles( QDir d )
{
	// looking fodlers to load
	foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
	{
		d.cd( s );
		recursiveFiles( d );
		d.cdUp();
	}
	// looking files to load
	foreach ( QString s, d.entryList( QDir::Files ) )
	{
		lInformations->setText( tr( "Loading file to prepare: %1" ).arg( s ) );
		qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
		mAPI->load( d.absoluteFilePath( s ) );
	}
}
//
void UISettingsQMake::generateApi( const QString& s1, const QString& s2 )
{
	// disable window
	window()->setEnabled( false );
	// show progress bar
	pbProgress->show();
	pbProgress->setValue( 0 );
	// create api objects
	mLexer = new QsciLexerCPP( this );
	mAPI = new QsciAPIs( mLexer );
	// load prepared files
	mFile = s1 +".api";
	if ( mAPI->isPrepared( mFile ) )
		QFile::remove( mFile );
	// add header files to apis
	recursiveFiles( QDir( s2 ) );
	// connect
	connect( mAPI, SIGNAL( apiPreparationFinished() ), this, SLOT( generationFinished() ) );
	// prepare apis files
	lInformations->setText( tr( "Preparing files, please wait..." ) );
	mAPI->prepare();
}
//
void UISettingsQMake::generationFinished()
{
	// save prepared files
	mAPI->savePrepared( mFile );
	// delete api objects
	delete mLexer;
	// hide progress bar
	pbProgress->setValue( -1 );
	pbProgress->hide();
	// finish message
	lInformations->setText( tr( "Prepared files finished." ) );
	// enable window
	window()->setEnabled( true );
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
void UISettingsQMake::on_pbGenerate_clicked()
{
	// apply settings in case of user set path but not yet click on apply
	on_bbDialog_clicked( 0 );
	// clear label
	lInformations->clear();
	// if no checked box cancel
	if ( !cbQt4->isChecked() && !cbQt3->isChecked() )
	{
		lInformations->setText( tr( "You need to check the boxes for the Qt version you want to generate api." ) );
		return;
	}
	//
	QString s;
	// if qt4 and no qt4 path configured
	if ( cbQt4->isChecked() )
	{
		s = Settings::current()->value( "Plugins/QMake/Qt4 Path" ).toString().append( "/include" );
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your Qt4 include path" ), s );
		if ( !s.isNull() )
			generateApi( "Qt4", s );
	}
	// if qt3 and no qt3 path configured
	if ( cbQt3->isChecked() )
	{
		s = Settings::current()->value( "Plugins/QMake/Qt3 Path" ).toString().append( "/include" );
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your Qt3 include path" ), s );
		if ( !s.isNull() )
			generateApi( "Qt3", s );
	}
}
//
void UISettingsQMake::on_bbDialog_clicked( QAbstractButton* )
{
	for ( int i = 0; i < cbKeywords->count(); i++ )
		Settings::current()->setValue( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ), cbKeywords->itemData( i, DataRole ).toString() );
}
