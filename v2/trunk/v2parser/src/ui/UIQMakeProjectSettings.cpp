#include "UIQMakeProjectSettings.h"
#include "QMakeProjectScopesProxy.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "UIItemSettings.h"
//
#include <QFileDialog>
//
UIQMakeProjectSettings::UIQMakeProjectSettings( QMakeProjectModel* m, QWidget* p )
	: QDialog( p ), mProxy( 0 ), mProject( m )
{
	setupUi( this );
	//
	mProxy = new QMakeProjectScopesProxy( mProject );
	tvScopes->setModel( mProxy );
	lvContents->setModel( mProject );
	setCurrentIndex( mProject->index( 0, 0 ) );
	//
	loadModules();
	loadConfigs();
	loadSettings();
	// connections
	connect( tbIcon, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbHelpFile, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwCompilerFlags, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
}
//
UIQMakeProjectSettings::~UIQMakeProjectSettings()
{
	delete mProxy;
	qDeleteAll( mModules );
	mModules.clear();
	qDeleteAll( mConfigs );
	mConfigs.clear();
}
//
void UIQMakeProjectSettings::execute( QMakeProjectModel* m, QWidget* p )
{
	if ( !m )
		return;
	UIQMakeProjectSettings d( m, p );
	d.exec();
}
//
QModelIndex UIQMakeProjectSettings::currentIndex()
{
	QModelIndex i;
	if ( lvContents->selectionModel()->selectedIndexes().count() )
		i = lvContents->selectionModel()->selectedIndexes().at( 0 );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
	{
		i = tvScopes->selectionModel()->selectedIndexes().at( 0 );
		if ( i.isValid() )
			i = mProxy->mapToSource( i );
	}
	return i;
}
//
void UIQMakeProjectSettings::setCurrentIndex( const QModelIndex& i )
{
	// clear selection
	tvScopes->clearSelection();
	lvContents->clearSelection();
	// set/select current index
	if ( !i.isValid() )
		return;
	else if ( i.data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::ValueType )
	{
		tvScopes->setCurrentIndex( mProxy->mapFromSource( i.parent() ) );
		lvContents->setRootIndex( i.parent() );
		lvContents->setCurrentIndex( i );
	}
	else
	{
		tvScopes->setCurrentIndex( mProxy->mapFromSource( i ) );
		lvContents->setRootIndex( i );
	}
}
//
void UIQMakeProjectSettings::loadModules()
{
	// load modules informations
	// TODO: need to be add to qmake plugin properties, so user can add new if new qt release go out
	mModules << new QtItem( "QtCore", "core", "QT", "Add support for Qt Core classes" );
	mModules << new QtItem( "QtGui", "gui", "QT", "Add support for Qt Gui classes" );
	mModules << new QtItem( "QtNetwork", "network", "QT", "Add support for Qt Network classes" );
	mModules << new QtItem( "QtOpenGL", "opengl", "QT", "Add support for Qt OpenGL classes" );
	mModules << new QtItem( "QtSql", "sql", "QT", "Add support for Qt Sql classes" );
	mModules << new QtItem( "QtSvg", "svg", "QT", "Add support for Qt Svg classes" );
	mModules << new QtItem( "QtXml", "xml", "QT", "Add support for Qt Xml classes" );
	mModules << new QtItem( "Qt3Support", "qt3support", "QT", "Add support for Qt Qt3Support classes" );
	mModules << new QtItem( "QtDesigner", "designer", "CONFIG", "Add support for Qt Designer classes" );
	mModules << new QtItem( "QtUiTools", "uitools", "CONFIG", "Add support for Qt UiTools classes" );
	mModules << new QtItem( "QtAssistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" );
	mModules << new QtItem( "QtTest", "qtestlib", "CONFIG", "Add support for Qt Test classes" );
	mModules << new QtItem( "QAxContainer", "qaxcontainer", "CONFIG", "Add support for QAxContainer classes" );
	mModules << new QtItem( "QAxServer", "qaxserver", "CONFIG", "Add support for QAxServer classes" );
	mModules << new QtItem( "QtDBus", "qdbus", "CONFIG", "Add support for Qt DBus classes" );
	// fill lwQtModules
	foreach ( QtItem* i, mModules )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwQtModules );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setToolTip( i->Description );
		it->setCheckState( Qt::Unchecked );
	}
}
//
void UIQMakeProjectSettings::loadConfigs()
{
	// load configs informations
	// TODO: need to be add to qmake plugin properties, so user can add new if new qt release go out
	mConfigs << new QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled." );
	mConfigs << new QtItem( "stl", "stl", "CONFIG", "STL support is enabled." );
	mConfigs << new QtItem( "exceptions", "execptions", "CONFIG", "Exception support is enabled." );
	mConfigs << new QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project." );
	mConfigs << new QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used." );
	mConfigs << new QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the \\l{#qt}{QT} variable." );
	mConfigs << new QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined." );
	mConfigs << new QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead." );
	mConfigs << new QtItem( "LIB ONLY", QString::null, QString::null, "Options for LIB template only" );
	mConfigs << new QtItem( "dll", "dll", "CONFIG", "The target is a shared object/DLL.The proper include paths, compiler flags and libraries will automatically be added to the project." );
	mConfigs << new QtItem( "staticlib", "staticlib", "CONFIG", "The target is a static library (lib only). The proper compiler flags will automatically be added to the project." );
	mConfigs << new QtItem( "plugin", "plugin", "CONFIG", "The target is a plugin (lib only). This enables dll as well." );
	mConfigs << new QtItem( "X11 ONLY", QString::null, QString::null, "Options for X11 only" );
	mConfigs << new QtItem( "x11", "", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project." );
	mConfigs << new QtItem( "MAC OS X ONLY", QString::null, QString::null, "Options for Mac OS X only" );
	mConfigs << new QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary." );
	mConfigs << new QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary." );
	mConfigs << new QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)." );
	mConfigs << new QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle." );
	mConfigs << new QtItem( "WINDOWS ONLY", QString::null, QString::null, "Options for Windows only" );
	mConfigs << new QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths,compiler flags and libraries will automatically be added to the project." );
	mConfigs << new QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project." );
	mConfigs << new QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default." );
	mConfigs << new QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an application/library project." );
	mConfigs << new QtItem( "ACTIVEQT ONLY", QString::null, QString::null, "No help available" );
	mConfigs << new QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" );
	// fill lwCompilerFlags
	QFont fo( lwCompilerFlags->font() );
	fo.setPointSize( 8 );
	fo.setBold( true );
	foreach ( QtItem* i, mConfigs )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwCompilerFlags );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setToolTip( i->Description );
		if ( it->text().toLower().contains( "only" ) )
		{
			it->setFlags( 0 );
			it->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
			it->setBackground( Qt::darkBlue );
			it->setForeground( Qt::white );
			it->setFont( fo );
		}
		else
			it->setCheckState( Qt::Unchecked );
	}
}
//
void UIQMakeProjectSettings::loadSettings()
{
	QString c, s;
	QStringList l;
	// Application
	leTitle->setText( mProject->getStringValues( "APP_TITLE" ) );
	leIcon->setText( mProject->getStringValues( "APP_ICON" ) );
	if ( !leIcon->text().isEmpty() )
		lPixmap->setPixmap( QPixmap( leIcon->text() ) );
	leHelpFile->setText( mProject->getStringValues( "APP_HELP_FILE" ) );
	leAuthor->setText( mProject->getStringValues( "APP_AUTHOR" ) );
	s = mProject->getStringValues( "VERSION" );
	if ( !s.isEmpty() )
	{
		gbVersion->setChecked( true );
		l = s.split( "." );
		sbMajor->setValue( l.value( 0 ).toInt() );
		sbMinor->setValue( l.value( 1 ).toInt() );
		sbRelease->setValue( l.value( 2 ).toInt() );
		sbBuild->setValue( l.value( 3 ).toInt() );
		cbBuildAutoIncrement->setChecked( mProject->getStringValues( "APP_AUTO_INCREMENT" ).toInt() );
	}
	s = mProject->getStringValues( "TEMPLATE" );
	if ( cbTemplate->findText( s, Qt::MatchExactly ) == -1 )
		cbTemplate->addItem( s );
	cbTemplate->setCurrentIndex( cbTemplate->findText( s, Qt::MatchExactly ) );
	s = mProject->getStringValues( "LANGUAGE" );
	if ( cbLanguage->findText( s, Qt::MatchExactly ) == -1 )
		cbLanguage->addItem( s );
	cbLanguage->setCurrentIndex( cbLanguage->findText( s, Qt::MatchExactly ) );
	c = mProject->getStringValues( "CONFIG", "+=" );
	if ( c.indexOf( "debug_and_release", 0, Qt::CaseInsensitive ) != -1 )
		rbDebugRelease->setChecked( true );
	else if ( c.indexOf( "debug", 0, Qt::CaseInsensitive ) != -1 )
		rbDebug->setChecked( true );
	else if ( c.indexOf( "release", 0, Qt::CaseInsensitive ) != -1 )
		rbRelease->setChecked( true );
	else
		rbRelease->setChecked( true );
	if ( c.indexOf( "warn_off", 0, Qt::CaseInsensitive ) != -1 )
		rbWarnOff->setChecked( true );
	else if ( s.indexOf( "warn_on", 0, Qt::CaseInsensitive ) != -1 )
		rbWarnOn->setChecked( true );
	else
		rbWarnOff->setChecked( true );
	if ( c.indexOf( "build_all", 0, Qt::CaseInsensitive ) != -1 )
		cbBuildAll->setChecked( true );
	if ( c.indexOf( "ordered", 0, Qt::CaseInsensitive ) != -1 )
		cbOrdered->setChecked( true );
	// remove content from config to allow us later to set leConfig
	c.remove( "debug_and_release", Qt::CaseInsensitive );
	c.remove( "debug", Qt::CaseInsensitive );
	c.remove( "release", Qt::CaseInsensitive );
	c.remove( "warn_off", Qt::CaseInsensitive );
	c.remove( "warn_on", Qt::CaseInsensitive );
	c.remove( "build_all", Qt::CaseInsensitive );
	c.remove( "ordered", Qt::CaseInsensitive );
	// Libraries
	gbQtModules->setChecked( c.contains( "qt", Qt::CaseInsensitive ) );
	s = mProject->getStringValues( "QT" );
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
	{
		if ( it->data( QtItem::VariableRole ).toString().toLower() == "config" )
		{
			if ( c.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) )
				it->setCheckState( Qt::Checked );
		}
		else if ( it->data( QtItem::VariableRole ).toString().toLower() == "qt" )
		{
			if ( s.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) )
				it->setCheckState( Qt::Checked );
		}
		c.remove( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive );
	}
	foreach ( QListWidgetItem* it, lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
	{
		if ( it->data( QtItem::VariableRole ).toString().toLower() == "config" )
			if ( c.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) )
				it->setCheckState( Qt::Checked );
		c.remove( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive );
	}
	leConfig->setText( c.simplified() );
	// settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}
//
void UIQMakeProjectSettings::tb_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QString s;
	// Application
	if ( tb == tbIcon )
	{
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application icon" ), leIcon->text(), tr( "Images (*.png *.xpm *.jpg *.ico)" ) );
		if ( !s.isEmpty() )
		{
			leIcon->setText( s );
			lPixmap->setPixmap( QPixmap( s ) );
		}
	}
	else if ( tb == tbHelpFile )
	{
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application help root file" ), leHelpFile->text(), tr( "HTMLs (*.htm *.html)" ) );
		if ( !s.isEmpty() )
			leHelpFile->setText( s );
	}
}
//
void UIQMakeProjectSettings::lw_currentItemChanged( QListWidgetItem* it, QListWidgetItem* )
{
	if ( it )
		tbInformations->setHtml( it->toolTip() );
}
//

void UIQMakeProjectSettings::on_cbScopes_currentIndexChanged( const QString& )
{
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}

void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& s )
{
	QString k = QString( "%1|%2" ).arg( cbScopes->currentText(), s );
	// check if we already take it
	if ( !mSettingsOutput.contains( k ) )
	{
		// load destdir and target
		leOutputPath->setText( mProject->getStringValues( "DESTDIR", s, cbScopes->currentText() ) );
		leOutputName->setText( mProject->getStringValues( "TARGET", s, cbScopes->currentText() ) );
		// bakup data
		mSettingsOutput[k] = QStringList() << leOutputPath->text() << leOutputName->text();
	}
	else
	{
		// restore data
		leOutputPath->setText( mSettingsOutput.value( k ).at( 0 ) );
		leOutputName->setText( mSettingsOutput.value( k ).at( 1 ) );
	}
		
	// load values
	
}

void UIQMakeProjectSettings::on_cbVariables_currentIndexChanged( const QString& )
{
}

//
void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{
	setCurrentIndex( mProxy->mapToSource( i ) );
}
//
void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	UIItemSettings::edit( mProject, 0, this )->exec();
}
//
void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mProject, static_cast<QMakeProjectItem*>( mProject->itemFromIndex( i ) ), this )->exec();
}
//
void UIQMakeProjectSettings::on_tbRemove_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		mProject->QAbstractItemModel::removeRow( i.row(), i.parent() );
}
//
void UIQMakeProjectSettings::on_tbClear_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
	{
		i = i.parent();
		while ( mProject->rowCount( i ) )
			mProject->QAbstractItemModel::removeRow( 0, i );
	}
}
//
void UIQMakeProjectSettings::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->itemFromIndex( i )->moveUp() )
			setCurrentIndex( i.sibling( i.row() -1, i.column() ) );
}
//
void UIQMakeProjectSettings::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->itemFromIndex( i )->moveDown() )
			setCurrentIndex( i.sibling( i.row() +1, i.column() ) );
}
//
void UIQMakeProjectSettings::accept()
{
	QDialog::accept();
}
//
void UIQMakeProjectSettings::reject()
{
	QDialog::reject();
}
