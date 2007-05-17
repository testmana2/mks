#include "UIQMakeProjectSettings.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectProxy.h"
#include "QMakeProjectItem.h"
#include "UIItemSettings.h"
//
#include <QCompleter>
#include <QDirModel>
#include <QHeaderView>
#include <QLocale>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileIconProvider>
#include <QTextCodec>
#include <QMessageBox>
#include <QCloseEvent>
//
QString mTranslationMask = "translations/%1_%2.ts";
//
UIQMakeProjectSettings::UIQMakeProjectSettings( QMakeProjectItem* m, QWidget* p )
	: QDialog( p ), mReady( false ), mProject( m ), mDirs( new QDirModel( this ) )
{
	setupUi( this );
	setWindowTitle( QString( "Project Settings - %1" ).arg( m->data().toString() ) );
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/Icons/Icons/buttonok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/Icons/Icons/buttoncancel.png" ) );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QPixmap( ":/Icons/Icons/helpkeyword.png" ) );
	//
	mDirs->setReadOnly( true );
	mDirs->setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
	mDirs->setSorting( QDir::Name );
	leDir->setCompleter( new QCompleter( mDirs, leDir ) );
	tvDirs->setModel( mDirs );
#ifndef Q_WS_WIN
	// all os except windows got only one root drive, so don't need to show root
	tvDirs->setRootIndex( mDirs->index( "/" ) );
#endif
	for ( int i = 1; i < tvDirs->header()->count(); i++ )
		tvDirs->setColumnHidden( i, true );
	//
	setDir( mDirs->index( projectPath() ) );
	// scopes
	mScopesProxy = new QMakeProjectProxy( mProject->model(), false );
	mScopesProxy->setFilterRoles( QList<int>() << AbstractProjectModel::ValueType );
	mScopesProxy->setFiltering( true );
	tvScopes->setModel( mScopesProxy );
	tvScopes->setRootIndex( mScopesProxy->mapFromSource( mProject->index().parent() ) );
	// scope content
	mContentProxy = new QMakeProjectProxy( mProject->model(), false );
	mContentProxy->setFilterRoles( QList<int>() << AbstractProjectModel::ValueType );
	mContentProxy->setNegateFilter( false );
	mContentProxy->setFiltering( true );
	lvContents->setModel( mContentProxy );
	setCurrentIndex( mProject->index().child( 0, 0 ) );
	// loading...
	loadEncodings();
	loadModules();
	loadConfigs();
	loadSettings();
	loadLanguages();
	// connections
	connect( tbIcon, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbHelpFile, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	foreach ( QSpinBox* sb, gbVersion->findChildren<QSpinBox*>() )
		if ( sb != sbBuild )
			connect( sb, SIGNAL( valueChanged( int ) ), this, SLOT( sb_valueChanged( int ) ) );
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwCompilerFlags, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( cbScopes, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbOperators, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( tbOutputPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( cbVariables, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( leDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( setDir( const QString& ) ) );
	connect( tvDirs, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( setDir( const QModelIndex& ) ) );
	mReady = true;
	// settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}
//
UIQMakeProjectSettings::~UIQMakeProjectSettings()
{
	delete mScopesProxy;
	delete mContentProxy;
}
//
void UIQMakeProjectSettings::closeEvent( QCloseEvent* e )
{
	if ( result() == QDialog::Rejected && QMessageBox::question( this, tr( "Cancel..." ), tr( "Are you sure ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		e->ignore();
}
//
void UIQMakeProjectSettings::execute( QMakeProjectItem* m, QWidget* p )
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
		i = mContentProxy->mapToSource( lvContents->selectionModel()->selectedIndexes().at( 0 ) );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
		i = mScopesProxy->mapToSource( tvScopes->selectionModel()->selectedIndexes().at( 0 ) );
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
	else if ( i.data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ValueType )
	{
		tvScopes->setCurrentIndex( mScopesProxy->mapFromSource( i.parent() ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i.parent() ) );
		lvContents->setCurrentIndex( mContentProxy->mapFromSource( i ) );
	}
	else
	{
		tvScopes->setCurrentIndex( mScopesProxy->mapFromSource( i ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i ) );
	}
}
//
QString UIQMakeProjectSettings::projectName() const
{
	return mProject->model()->name( mProject->index() );
}
//
QString UIQMakeProjectSettings::projectPath() const
{
	return mProject->model()->path( mProject->index() );
}
//
QString UIQMakeProjectSettings::getFilePath( const QString& s )
{
	QString f = mProject->model()->filePath( s, mProject->index() );
	if ( !QFile::exists( f ) )
		f = projectPath();
	return f;
}
//
QString UIQMakeProjectSettings::getRelativeFilePath( const QString& s )
{
	return mProject->model()->relativeFilePath( s, mProject->index() );
}
//
void UIQMakeProjectSettings::loadEncodings()
{
	QStringList l;
	foreach ( QByteArray ba, QTextCodec::availableCodecs() )
		l << ba;
	l.sort();
	cbEncodings->addItems( l );
}
//
void UIQMakeProjectSettings::loadModules()
{
	// load modules informations
	// TODO: need to be add to qmake plugin properties, so user can add new if new qt release go out
	mModules = UISettingsQMake::readQtModules();
	// fill lwQtModules
	foreach ( QtItem* i, mModules )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwQtModules );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
		it->setCheckState( Qt::Unchecked );
	}
	// free memory
	qDeleteAll( mModules );
	mModules.clear();
}
//
void UIQMakeProjectSettings::loadConfigs()
{
	// load configs informations
	// TODO: need to be add to qmake plugin properties, so user can add new if new qt release go out
	mConfigs = UISettingsQMake::readSettings();
	// fill lwCompilerFlags
	QFont fo( lwCompilerFlags->font() );
	fo.setPointSize( 8 );
	fo.setBold( true );
	foreach ( QtItem* i, mConfigs )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwCompilerFlags );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
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
	// free memory
	qDeleteAll( mConfigs );
	mConfigs.clear();
}
//
void UIQMakeProjectSettings::loadSettings()
{
	// load configs informations
	// TODO: need to be add to qmake plugin properties, so user can add new if new qt release go out
	QStringList list;
	list = QStringList() << QString::null << "win32" << "unix" << "linux" << "mac" << "macx" << "aix-g++" << "aix-g++-64"
	<< "aix-xlc" << "aix-xlc-64" << "darwin-g++" << "freebsd-g++" << "freebsd-g++34" << "freebsd-g++40" << "freebsd-icc"
	<< "hpux-acc" << "hpux-acc-64" << "hpux-acc-o64" << "hpux-g++" << "hpux-g++-64" << "hpuxi-acc" << "hpuxi-acc-64"
	<< "hurd-g++" << "irix-cc" << "irix-cc-64" << "irix-g++" << "irix-g++-64" << "linux-cxx" << "linux-ecc-64"
	<< "linux-g++" << "linux-g++-32" << "linux-g++-64" << "linux-icc" << "linux-kcc" << "linux-lsb" << "linux-pgcc"
	<< "lynxos-g++" << "macx-g++" << "macx-icc" << "macx-pbuilder" << "macx-xcode" << "macx-xlc" << "netbsd-g++"
	<< "openbsd-g++" << "sco-cc" << "sco-g++" << "solaris-cc" << "solaris-cc-64" << "solaris-g++" << "solaris-g++-64"
	<< "tru64-cxx" << "tru64-g++" << "unixware-cc" << "unixware-g++" << "win32-g++" << "win32-x-g++" << "default"
	<< "freebsd-generic-g++" << "linux-arm-g++" << "linux-cellon-g++" << "linux-generic-g++" << "linux-generic-g++-32"
	<< "linux-ipaq-g++" << "linux-mips-g++" << "linux-sharp-g++" << "linux-x86_64-g++" << "linux-x86-g++"
	<< "linux-zylonite-g++" << "macx-generic-g++" << "solaris-generic-g++";
	foreach ( QString s, list )
		cbScopes->addItem( s );
	list = QStringList() << "=" << "-=" << "+=" << "*=" << "~=";
	foreach ( QString s, list )
		cbOperators->addItem( s );
	//
	QString c, s;
	QStringList l;
	// Application
	leTitle->setText( mProject->model()->getStringValues( "APP_TITLE" ) );
	leTitle->setModified( false );
	leIcon->setText( mProject->model()->getStringValues( "APP_ICON" ) );
	leIcon->setModified( false );
	if ( !leIcon->text().isEmpty() )
		lPixmap->setPixmap( QPixmap( QFileInfo( leIcon->text() ).isRelative() ? projectPath().append( "/" ).append( leIcon->text() ) : leIcon->text() ) );
	leHelpFile->setText( mProject->model()->getStringValues( "APP_HELP_FILE" ) );
	leHelpFile->setModified( false );
	leAuthor->setText( mProject->model()->getStringValues( "APP_AUTHOR" ) );
	leAuthor->setModified( false );
	s = mProject->model()->getStringValues( "VERSION" );
	if ( !s.isEmpty() )
	{
		gbVersion->setChecked( true );
		l = s.split( "." );
		sbMajor->setValue( l.value( 0 ).toInt() );
		sbMinor->setValue( l.value( 1 ).toInt() );
		sbRelease->setValue( l.value( 2 ).toInt() );
		sbBuild->setValue( l.value( 3 ).toInt() );
		cbBuildAutoIncrement->setChecked( mProject->model()->getStringValues( "APP_AUTO_INCREMENT" ).toInt() );
	}
	s = mProject->model()->getStringValues( "TEMPLATE" );
	if ( cbTemplate->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbTemplate->addItem( s );
	cbTemplate->setCurrentIndex( cbTemplate->findText( s, Qt::MatchExactly ) );
	s = mProject->model()->getStringValues( "LANGUAGE" );
	if ( cbLanguage->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbLanguage->addItem( s );
	cbLanguage->setCurrentIndex( cbLanguage->findText( s, Qt::MatchExactly ) );
	s = mProject->model()->getStringValues( "ENCODING" );
	if ( s.isEmpty() )
		s = "UTF-8";
	if ( cbEncodings->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbEncodings->addItem( s );
	cbEncodings->setCurrentIndex( cbEncodings->findText( s, Qt::MatchExactly ) );
	c = mProject->model()->getStringValues( "CONFIG", mProject->index(), "+=" );
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
	else if ( c.indexOf( "warn_on", 0, Qt::CaseInsensitive ) != -1 )
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
	s = mProject->model()->getStringValues( "QT" );
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
}
//
void UIQMakeProjectSettings::loadLanguages()
{
	QStringList t = mProject->model()->getListValues( "TRANSLATIONS" );
	QListWidgetItem*  it;
	for ( int i = QLocale::C; i < QLocale::Zulu +1; i++ )
	{
		QString s = QLocale::languageToString( ( QLocale::Language )i );
		QString f = mTranslationMask.arg( mProject->data().toString() ).arg( s );
		it = new QListWidgetItem( s, lwTranslations );
		it->setCheckState( t.contains( f, Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
	}
}
//
void UIQMakeProjectSettings::setDir( const QString& s )
{
	if ( QFile::exists( s ) )
		setDir( mDirs->index( s ) );
}
//
void UIQMakeProjectSettings::setDir( const QModelIndex& i )
{
	if ( tvDirs->currentIndex() != i )
		tvDirs->setCurrentIndex( i );
	QString s = leDir->text();
	if ( s.endsWith( "/" ) )
		s.chop( 1 );
	if ( s != mDirs->filePath( i ) )
		leDir->setText( mDirs->filePath( i ) );
	lwFiles->clear();
	QDir d( leDir->text() );
	foreach ( QString s, d.entryList( QDir::Files, QDir::Name ) )
		lwFiles->addItem( new QListWidgetItem( mDirs->iconProvider()->icon( QFileIconProvider::File ), s ) );
}
//
void UIQMakeProjectSettings::addValue( const QString& s )
{
	// check if value already exists
	if ( lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		// add project translation if needed
		// got language
		QString f = QFileInfo( s ).baseName().remove( mProject->data().toString() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( mProject->data().toString() ).arg( f ) )
		{
			// find item to check and check it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Checked );
		}
	}
	// add item
	lwValues->addItem( s );
}
//
void UIQMakeProjectSettings::editValue( const QString& s )
{
	// got item to edit, else return
	QListWidgetItem* it = lwValues->currentItem();
	if ( !it )
		return;
	// check if value already exists
	if ( lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		QString f;
		// remove the edited translation if needed
		// got language
		f = QFileInfo( it->text() ).baseName().remove( mProject->data().toString() +'_' );
		// look if it's a project translation
		if ( it->text() == mTranslationMask.arg( mProject->data().toString() ).arg( f ) )
		{
			// find item and uncheck it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Unchecked );
		}
		// add project translation if needed
		// got language
		f = QFileInfo( s ).baseName().remove( mProject->data().toString() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( mProject->data().toString() ).arg( f ) )
		{
			// find item to check and check it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Checked );
		}
	}
	// edit item
	it->setText( s );
}
//
void UIQMakeProjectSettings::removeValue( const QString& s )
{
	// got item to delete, else return
	QListWidgetItem* cit = s.isEmpty() ? lwValues->currentItem() : lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
	if ( !cit )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		// got language
		QString f = QFileInfo( cit->text() ).baseName().remove( mProject->data().toString() +'_' );
		// look if it's a project translation
		if ( cit->text() == mTranslationMask.arg( mProject->data().toString() ).arg( f ) )
		{
			// find item and uncheck it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Unchecked );
		}
	}
	// delete item
	delete cit;
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
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application icon" ), getFilePath( leIcon->text() ), tr( "Images (*.png *.xpm *.jpg *.ico)" ) );
		if ( !s.isEmpty() )
		{
			leIcon->setText( getRelativeFilePath( s ) );
			leIcon->setModified( true );
			lPixmap->setPixmap( QPixmap( s ) );
		}
	}
	else if ( tb == tbHelpFile )
	{
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application help root file" ), getFilePath( leHelpFile->text() ), tr( "HTMLs (*.htm *.html)" ) );
		if ( !s.isEmpty() )
		{
			leHelpFile->setText( getRelativeFilePath( s ) );
			leHelpFile->setModified( true );
		}
	}
	else if ( tb == tbOutputPath )
	{
		s = QFileDialog::getExistingDirectory( this, tr( "Choose your application output path" ), getFilePath( leOutputPath->text() ) );
		if ( !s.isEmpty() )
		{
			leOutputPath->setText( getRelativeFilePath( s ) );
			leOutputPath->setModified( true );
		}
	}
}
//
void UIQMakeProjectSettings::sb_valueChanged( int )
{
	sbBuild->setValue( 0 );
}
//
void UIQMakeProjectSettings::on_cbTemplate_currentIndexChanged( const QString& s )
{
	if ( mReady )
		cbOrdered->setEnabled( s.toLower() == "subdirs" );
}
//
void UIQMakeProjectSettings::lw_currentItemChanged( QListWidgetItem* it, QListWidgetItem* )
{
	if ( it )
		tbInformations->setHtml( it->data( QtItem::HelpRole ).toString() );
}
//
void UIQMakeProjectSettings::cb_highlighted( int )
{
	QString k = QString( "%1|%2" ).arg( cbScopes->currentText(), cbOperators->currentText() );
	if ( leOutputPath->isModified() )
		mSettings[ QString( "%1|DESTDIR" ).arg( k ) ] = QStringList( leOutputPath->text() );
	if ( leOutputName->isModified() )
		mSettings[ QString( "%1|TARGET" ).arg( k ) ] = QStringList( leOutputName->text() );
	QStringList l;
	foreach ( QListWidgetItem* it, lwValues->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	mSettings[ QString( "%1|%2" ).arg( k, cbVariables->currentText() ) ] = l;
}
//
void UIQMakeProjectSettings::on_cbScopes_currentIndexChanged( const QString& )
{
	if ( mReady )
		on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}
//
void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& s )
{
	if ( !mReady )
		return;
	QString k = QString( "%1|%2" ).arg( cbScopes->currentText(), s );
	// set backup data if available
	if ( mSettings.contains( QString( "%1|DESTDIR" ).arg( k ) ) )
		leOutputPath->setText( mSettings[ QString( "%1|DESTDIR" ).arg( k ) ].join( " " ) );
	else // got data
		leOutputPath->setText( mProject->model()->getStringValues( "DESTDIR", mProject->index(), s, cbScopes->currentText() ) );
	leOutputPath->setModified( false );
	// set backup data if available
	if ( mSettings.contains( QString( "%1|TARGET" ).arg( k ) ) )
		leOutputName->setText( mSettings[ QString( "%1|TARGET" ).arg( k ) ].join( " " ) );
	else // got data
		leOutputName->setText( mProject->model()->getStringValues( "TARGET", mProject->index(), s, cbScopes->currentText() ) );
	leOutputName->setModified( false );
	// load variables values
	on_cbVariables_currentIndexChanged( cbVariables->currentText() );
}
//
void UIQMakeProjectSettings::on_tvDirs_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() && cbVariables->currentText().toLower().contains( "path" ) )
		addValue( getRelativeFilePath( mDirs->filePath( i ) ) );
}
//
void UIQMakeProjectSettings::on_lwFiles_itemDoubleClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	QFileInfo f( leDir->text().append( "/" ).append( i->text() ) );
	QString s = f.completeSuffix().toLower();
	QStringList libraries = QStringList() << "lib" << "dll" << "a" << "la" << "so" << "dylib";
	QString v = cbVariables->currentText().toLower();
	if ( v == "libs" && ( libraries.contains( s ) || s.startsWith( "so." ) ) )
	{
		// lib path
		s = getRelativeFilePath( f.path() );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-L" );
		addValue( s );
		// file
		s = f.baseName();
		if ( s.startsWith( "lib", Qt::CaseInsensitive ) )
			s.remove( 0, 3 );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-l" );
		addValue( s );
	}
	else if ( v == "defines" )
		addValue( f.baseName().toUpper() );
	else if ( v.contains( "path" ) )
		addValue( getRelativeFilePath( f.path() ) );
	else if ( ( v == "translations" && s == "ts" ) || ( v == "resources" && s == "qrc" ) || ( v == "def_file" && s == "def" ) || ( v == "rc_file" && s == "rc" ) || ( v == "res_file" && s == "res" ) )
		addValue( getRelativeFilePath( f.filePath() ) );
}
//
void UIQMakeProjectSettings::on_cbVariables_currentIndexChanged( const QString& s )
{
	if ( !mReady )
		return;
	QString k = QString( "%1|%2|%3" ).arg( cbScopes->currentText(), cbOperators->currentText(), s );
	lwValues->clear();
	if ( mSettings.contains( k ) )
		lwValues->addItems( mSettings[ k ] );
	else
		lwValues->addItems( mProject->model()->getListValues( s, mProject->index(), cbOperators->currentText(), cbScopes->currentText() ) );
}
//
void UIQMakeProjectSettings::on_pbAddValue_clicked()
{
	bool b;
	QString s = QInputDialog::getText( this, tr( "Add value..." ), tr( "Set your value content :" ), QLineEdit::Normal, QString::null, &b );
	if ( b && !s.isEmpty() )
		addValue( s );
}
//
void UIQMakeProjectSettings::on_pbEditValue_clicked()
{
	if ( !lwValues->currentItem() )
		return;
	bool b;
	QString s = QInputDialog::getText( this, tr( "Edit value..." ), tr( "Edit your value content :" ), QLineEdit::Normal, lwValues->currentItem()->text(), &b );
	if ( b && !s.isEmpty() )
		editValue( s );
}
//
void UIQMakeProjectSettings::on_pbRemoveValue_clicked()
{
	removeValue();
}
//
void UIQMakeProjectSettings::on_pbClearValues_clicked()
{
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
		while ( lwValues->count() )
			removeValue( lwValues->item( 0 )->text() );
	else
		lwValues->clear();
}
//
void UIQMakeProjectSettings::on_lwTranslations_itemChanged( QListWidgetItem* it )
{
	if ( !it || !lwTranslations->isVisible() )
		return;
	// get translation file
	QString s = mTranslationMask.arg( mProject->data().toString() ).arg( it->text() );
	QString k = "|=|TRANSLATIONS";
	// check translation
	switch ( it->checkState() )
	{
		case Qt::Checked:
			if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
				lwValues->addItem( s );
			else
			{
				if ( !mSettings.contains( k ) )
					mSettings[ k ] = mProject->model()->getListValues( "TRANSLATIONS" );
				if ( !mSettings.value( k ).contains( s, Qt::CaseInsensitive ) )
					mSettings[ k ] << s;
			}
			break;
		case Qt::Unchecked:
			if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
				delete lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			else
			{
				if ( !mSettings.contains( k ) )
					mSettings[ k ] = mProject->model()->getListValues( "TRANSLATIONS" );
				if ( mSettings.value( k ).contains( s, Qt::CaseInsensitive ) )
					mSettings[ k ].removeAll( s );
			}
			break;
		default:
			break;
	}
}
//
void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{
	setCurrentIndex( mScopesProxy->mapToSource( i ) );
}
//
void UIQMakeProjectSettings::on_tvScopes_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mProject->model(), mProject->model()->itemFromIndex( mScopesProxy->mapToSource( i ) ), this )->exec();
}
//
void UIQMakeProjectSettings::on_lvContents_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mProject->model(), mProject->model()->itemFromIndex( mContentProxy->mapToSource( i ) ), this )->exec();
}
//
void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	UIItemSettings::edit( mProject->model(), 0, this )->exec();
}
//
void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mProject->model(), mProject->model()->itemFromIndex( i ), this )->exec();
}
//
void UIQMakeProjectSettings::on_tbRemove_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		mProject->model()->QAbstractItemModel::removeRow( i.row(), i.parent() );
}
//
void UIQMakeProjectSettings::on_tbClear_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
	{
		i = i.parent();
		while ( mProject->model()->rowCount( i ) )
			mProject->model()->QAbstractItemModel::removeRow( 0, i );
	}
}
//
void UIQMakeProjectSettings::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->model()->itemFromIndex( i )->moveUp() )
			setCurrentIndex( i.sibling( i.row() -1, i.column() ) );
}
//
void UIQMakeProjectSettings::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->model()->itemFromIndex( i )->moveDown() )
			setCurrentIndex( i.sibling( i.row() +1, i.column() ) );
}
//
void UIQMakeProjectSettings::accept()
{
	// backup current settings state
	cb_highlighted( 0 );
	// get model
	QMakeProjectModel* m = mProject->model();
	// applications
	QStringList s, c;
	if ( leTitle->isModified() )
		m->setStringValues( leTitle->text(), "APP_TITLE" );
	if ( leIcon->isModified() )
		m->setStringValues( leIcon->text(), "APP_ICON" );
	if ( leHelpFile->isModified() )
		m->setStringValues( leHelpFile->text(), "APP_HELP_FILE" );
	if ( leAuthor->isModified() )
		m->setStringValues( leAuthor->text(), "APP_AUTHOR" );
	if ( gbVersion->isChecked() )
	{
		m->setStringValues( QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() ), "VERSION" );
		m->setStringValues( QString::number( cbBuildAutoIncrement->isChecked() ), "APP_AUTO_INCREMENT" );
	}
	else
	{
		m->setStringValues( QString::null, "VERSION" );
		m->setStringValues( QString::null, "APP_AUTO_INCREMENT" );
	}
	if ( !cbTemplate->currentText().isEmpty() )
		m->setStringValues( cbTemplate->currentText(), "TEMPLATE" );
	if ( !cbLanguage->currentText().isEmpty() )
		m->setStringValues( cbLanguage->currentText(), "LANGUAGE" );
	if ( !cbEncodings->currentText().isEmpty() )
		m->setStringValues( cbEncodings->currentText(), "ENCODING" );
	// reading config variable
	if ( rbDebug->isChecked() )
		s << "debug";
	else if ( rbRelease->isChecked() )
		s << "release";
	else if ( rbDebugRelease->isChecked() )
	{
		s << "debug_and_release";
		if ( cbBuildAll->isChecked() )
			s << "build_all";
	}
	if ( rbWarnOff->isChecked() )
		s << "warn_off";
	else if ( rbWarnOn->isChecked() )
		s << "warn_on";
	if ( cbTemplate->currentText().toLower() == "subdirs" && cbOrdered->isChecked() )
		s << "ordered";
	// read qt modules / lwCompilerFlags
	QList<QListWidgetItem*> l = lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	l << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	foreach ( QListWidgetItem* it, l )
	{
		QString v = it->data( QtItem::VariableRole ).toString().toLower();
		if ( it->checkState() == Qt::Checked )
		{
			if ( v == "config" )
				s << it->data( QtItem::ValueRole ).toString();
			else if ( v == "qt" )
				c << it->data( QtItem::ValueRole ).toString();
		}		
	}
	// add other config
	if ( !leConfig->text().isEmpty() )
		s << leConfig->text().simplified().split( " " );
	m->setListValues( s, "CONFIG", mProject->index(), "+=" );
	m->setListValues( c, "QT" );
	s.clear();
	c.clear();
	// settings
	foreach ( QString v, mSettings.keys() )
	{
		QStringList l = v.split( "|" );
		m->setListValues( mSettings.value( v ), l.at( 2 ), mProject->index(), l.at( 1 ), l.at( 0 ) );
	}
	// close dialog
	QDialog::accept();
}
//
void UIQMakeProjectSettings::reject()
{
	setResult( QDialog::Rejected );
	close();
}
