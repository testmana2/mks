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
	// connect
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwSettings, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( tbAddFilter, SIGNAL( clicked() ), this, SLOT( tbAdd_clicked() ) );
	connect( tbAddQtModule, SIGNAL( clicked() ), this, SLOT( tbAdd_clicked() ) );
	connect( tbAddSetting, SIGNAL( clicked() ), this, SLOT( tbAdd_clicked() ) );
	connect( tbRemoveFilter, SIGNAL( clicked() ), this, SLOT( tbRemove_clicked() ) );
	connect( tbRemoveQtModule, SIGNAL( clicked() ), this, SLOT( tbRemove_clicked() ) );
	connect( tbRemoveSetting, SIGNAL( clicked() ), this, SLOT( tbRemove_clicked() ) );
	connect( tbClearFilters, SIGNAL( clicked() ), this, SLOT( tbClear_clicked() ) );
	connect( tbClearQtModules, SIGNAL( clicked() ), this, SLOT( tbClear_clicked() ) );
	connect( tbClearSettings, SIGNAL( clicked() ), this, SLOT( tbClear_clicked() ) );
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
QStringList UISettingsQMake::defaultFilters()
{
	return QStringList()
	<< "FORMS"
	<< "FORMS3"
	<< "HEADERS"
	<< "SOURCES"
	<< "RESOURCES"
	<< "IMAGES"
	<< "TRANSLATIONS";
}
//
QStringList UISettingsQMake::readFilters()
{
	QStringList l = Settings::current()->value( "Plugins/QMake/Filters" ).toStringList();
	return l.isEmpty() ? defaultFilters() : l;
}
//
QtItemList UISettingsQMake::defaultQtModules()
{
	return QtItemList()
	<< new QtItem( "QtCore", "core", "QT", "Add support for Qt Core classes" )
	<< new QtItem( "QtGui", "gui", "QT", "Add support for Qt Gui classes" )
	<< new QtItem( "QtNetwork", "network", "QT", "Add support for Qt Network classes" )
	<< new QtItem( "QtOpenGL", "opengl", "QT", "Add support for Qt OpenGL classes" )
	<< new QtItem( "QtSql", "sql", "QT", "Add support for Qt Sql classes" )
	<< new QtItem( "QtSvg", "svg", "QT", "Add support for Qt Svg classes" )
	<< new QtItem( "QtXml", "xml", "QT", "Add support for Qt Xml classes" )
	<< new QtItem( "Qt3Support", "qt3support", "QT", "Add support for Qt Qt3Support classes" )
	<< new QtItem( "QtDesigner", "designer", "CONFIG", "Add support for Qt Designer classes" )
	<< new QtItem( "QtUiTools", "uitools", "CONFIG", "Add support for Qt UiTools classes" )
	<< new QtItem( "QtAssistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" )
	<< new QtItem( "QtTest", "qtestlib", "CONFIG", "Add support for Qt Test classes" )
	<< new QtItem( "QAxContainer", "qaxcontainer", "CONFIG", "Add support for QAxContainer classes" )
	<< new QtItem( "QAxServer", "qaxserver", "CONFIG", "Add support for QAxServer classes" )
	<< new QtItem( "QtDBus", "qdbus", "CONFIG", "Add support for Qt DBus classes" );
}
//
QtItemList UISettingsQMake::readQtModules()
{
	QtItemList l;
	int s = Settings::current()->beginReadArray( "Plugins/QMake/QtModules" );
	for ( int i = 0; i < s; i++ )
	{
		Settings::current()->setArrayIndex( i );
		l << new QtItem( Settings::current()->value( "Text" ).toString(), Settings::current()->value( "Value" ).toString(), Settings::current()->value( "Variable" ).toString(), Settings::current()->value( "Help" ).toString() );
	}
	Settings::current()->endArray();
	return l.isEmpty() ? defaultQtModules() : l;
}
//
QtItemList UISettingsQMake::defaultSettings()
{
	return QtItemList()
	<< new QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled." )
	<< new QtItem( "stl", "stl", "CONFIG", "STL support is enabled." )
	<< new QtItem( "exceptions", "execptions", "CONFIG", "Exception support is enabled." )
	<< new QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project." )
	<< new QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used." )
	<< new QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the \\l{#qt}{QT} variable." )
	<< new QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined." )
	<< new QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead." )
	<< new QtItem( "LIB ONLY", QString::null, QString::null, "Options for LIB template only" )
	<< new QtItem( "dll", "dll", "CONFIG", "The target is a shared object/DLL.The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "staticlib", "staticlib", "CONFIG", "The target is a static library (lib only). The proper compiler flags will automatically be added to the project." )
	<< new QtItem( "plugin", "plugin", "CONFIG", "The target is a plugin (lib only). This enables dll as well." )
	<< new QtItem( "X11 ONLY", QString::null, QString::null, "Options for X11 only" )
	<< new QtItem( "x11", "x11", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project." )
	<< new QtItem( "MAC OS X ONLY", QString::null, QString::null, "Options for Mac OS X only" )
	<< new QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary." )
	<< new QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary." )
	<< new QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)." )
	<< new QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle." )
	<< new QtItem( "WINDOWS ONLY", QString::null, QString::null, "Options for Windows only" )
	<< new QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths,compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default." )
	<< new QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an application/library project." )
	<< new QtItem( "ACTIVEQT ONLY", QString::null, QString::null, "Option for Windows/Active Qt only" )
	<< new QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" );
}
//
QtItemList UISettingsQMake::readSettings()
{
	QtItemList l;
	int s = Settings::current()->beginReadArray( "Plugins/QMake/Settings" );
	for ( int i = 0; i < s; i++ )
	{
		Settings::current()->setArrayIndex( i );
		l << new QtItem( Settings::current()->value( "Text" ).toString(), Settings::current()->value( "Value" ).toString(), Settings::current()->value( "Variable" ).toString(), Settings::current()->value( "Help" ).toString() );
	}
	Settings::current()->endArray();
	return l.isEmpty() ? defaultSettings() : l;
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
	// general
	for ( int i = 0; i < cbKeywords->count(); i++ )
		cbKeywords->setItemData( i, Settings::current()->value( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ) ).toString(), DataRole );
	if ( cbKeywords->count() )
		on_cbKeywords_currentIndexChanged( 0 );
	lwFilters->addItems( readFilters() );
	//
	QtItemList l;
	// qt modules
	l = readQtModules();
	foreach ( QtItem* i, l )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwQtModules );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
	}
	qDeleteAll( l );
	l.clear();
	// settings
	l = readSettings();
	foreach ( QtItem* i, l )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwSettings );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
	}
	qDeleteAll( l );
	l.clear();
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
void UISettingsQMake::lw_currentItemChanged( QListWidgetItem* c, QListWidgetItem* p )
{
	if ( p )
	{
		if ( p->listWidget() == lwQtModules )
		{
			p->setText( leCaptionQtModule->text() );
			p->setData( QtItem::ValueRole, leValueQtModule->text() );
			p->setData( QtItem::VariableRole, leVariableQtModule->text().toUpper() );
			p->setData( QtItem::HelpRole, teHelpQtModule->toPlainText() );
		}
		else
		{
			p->setText( leCaptionSetting->text() );
			p->setData( QtItem::ValueRole, leValueSetting->text() );
			p->setData( QtItem::VariableRole, leVariableSetting->text().toUpper() );
			p->setData( QtItem::HelpRole, teHelpSetting->toPlainText() );
		}
	}
	if ( c )
	{
		if ( c->listWidget() == lwQtModules )
		{
			leCaptionQtModule->setText( c->text() );
			leValueQtModule->setText( c->data( QtItem::ValueRole ).toString() );
			leVariableQtModule->setText( c->data( QtItem::VariableRole ).toString() );
			teHelpQtModule->setPlainText( c->data( QtItem::HelpRole ).toString() );
		}
		else
		{
			leCaptionSetting->setText( c->text() );
			leValueSetting->setText( c->data( QtItem::ValueRole ).toString() );
			leVariableSetting->setText( c->data( QtItem::VariableRole ).toString() );
			teHelpSetting->setPlainText( c->data( QtItem::HelpRole ).toString() );
		}
	}
	else if ( sender() == lwQtModules )
	{
		leCaptionQtModule->clear();
		leValueQtModule->clear();
		leVariableQtModule->clear();
		teHelpQtModule->clear();
	}
	else if ( sender() == lwSettings )
	{
		leCaptionSetting->clear();
		leValueSetting->clear();
		leVariableSetting->clear();
		teHelpSetting->clear();
	}
}
//
void UISettingsQMake::tbAdd_clicked()
{
	if ( sender() == tbAddFilter )
	{
		lwFilters->addItem( tr( "New filter" ) );
		lwFilters->setCurrentItem( lwFilters->item( lwFilters->count() -1 ) );
		lwFilters->scrollToItem( lwFilters->item( lwFilters->count() -1 ) );
	}
	else if ( sender() == tbAddQtModule )
	{
		lwQtModules->addItem( tr( "New QtModule" ) );
		lwQtModules->setCurrentItem( lwQtModules->item( lwQtModules->count() -1 ) );
		lwQtModules->scrollToItem( lwQtModules->item( lwQtModules->count() -1 ) );
	}
	else if ( sender() == tbAddSetting )
	{
		lwSettings->addItem( tr( "New Setting" ) );
		lwSettings->setCurrentItem( lwSettings->item( lwSettings->count() -1 ) );
		lwSettings->scrollToItem( lwSettings->item( lwSettings->count() -1 ) );
	}
}
//
void UISettingsQMake::tbRemove_clicked()
{
	if ( sender() == tbRemoveFilter )
		delete lwFilters->currentItem();
	else if ( sender() == tbRemoveQtModule )
		delete lwQtModules->currentItem();
	else if ( sender() == tbRemoveSetting )
		delete lwSettings->currentItem();
}
//
void UISettingsQMake::tbClear_clicked()
{
	if ( sender() == tbClearFilters )
		lwFilters->clear();
	else if ( sender() == tbClearQtModules )
		lwQtModules->clear();
	else if ( sender() == tbClearSettings )
		lwSettings->clear();
}
//
void UISettingsQMake::on_bbDialog_clicked( QAbstractButton* )
{
	// general
	for ( int i = 0; i < cbKeywords->count(); i++ )
		Settings::current()->setValue( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ), cbKeywords->itemData( i, DataRole ).toString() );
	QStringList l;
	foreach ( QListWidgetItem* it, lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	Settings::current()->setValue( "Plugins/QMake/Filters", l );
	// qt modules
	lw_currentItemChanged( 0, lwQtModules->currentItem() );
	Settings::current()->beginWriteArray( "Plugins/QMake/QtModules" );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* it = lwQtModules->item( i );
		Settings::current()->setArrayIndex( i );
		Settings::current()->setValue( "Text", it->text() );
		Settings::current()->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		Settings::current()->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		Settings::current()->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	Settings::current()->endArray();
	// qt modules
	lw_currentItemChanged( 0, lwSettings->currentItem() );
	Settings::current()->beginWriteArray( "Plugins/QMake/Settings" );
	for ( int i = 0; i < lwSettings->count(); i++ )
	{
		QListWidgetItem* it = lwSettings->item( i );
		Settings::current()->setArrayIndex( i );
		Settings::current()->setValue( "Text", it->text() );
		Settings::current()->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		Settings::current()->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		Settings::current()->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	Settings::current()->endArray();
}
