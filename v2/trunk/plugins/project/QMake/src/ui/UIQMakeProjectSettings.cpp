#include "UIQMakeProjectSettings.h"
#include "QMakeProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "UIItemSettings.h"
#include "pMonkeyStudio.h"

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
#include <QDebug>
#include <QWhatsThis>

QString mTranslationMask = "$$TRANSLATIONS_PATH/%2_%3.ts";

UIQMakeProjectSettings::UIQMakeProjectSettings( ProjectItem* m, QWidget* p )
	: QDialog( p ), mReady( false ), mModel( m->model() ), mProject( m ), mDirs( new QDirModel( this ) )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// set window title
	setWindowTitle( QString( "Project Settings - %1" ).arg( projectName() ) );
	// load scopes
	cbScopes->addItems( UISettingsQMake::readScopes() );
	// load operators
	cbOperators->addItems( UISettingsQMake::readOperators() );
	// load text codec
	cbEncodings->addItems( pMonkeyStudio::availableTextCodecs() );
	// load modules & config
	loadModules();
	loadConfigs();
	// set button pixmap
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/Icons/Icons/buttonok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/Icons/Icons/buttoncancel.png" ) );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QPixmap( ":/Icons/Icons/helpkeyword.png" ) );
	//
	mDirs->setReadOnly( true );
	mDirs->setFilter( QDir::AllDirs );
	mDirs->setSorting( QDir::Name );
	leDir->setCompleter( new QCompleter( mDirs, leDir ) );
	lvDirs->setModel( mDirs );
#ifndef Q_WS_WIN
	// all os except windows got only one root drive, so don't need to show root
	lvDirs->setRootIndex( mDirs->index( "/" ) );
#endif
	//
	setDir( mDirs->index( projectPath() ) );
	// scopes
	mScopesProxy = new QMakeProxy( mModel, false, mProject );
	mScopesProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mScopesProxy->setFiltering( true );
	tvScopes->setModel( mScopesProxy );
	tvScopes->header()->hide();
	tvScopes->setRootIndex( mScopesProxy->mapFromSource( mProject->index() ) );
	// scope content
	mContentProxy = new QMakeProxy( mModel, false, mProject );
	mContentProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mContentProxy->setNegateFilter( false );
	mContentProxy->setFiltering( true );
	lvContents->setModel( mContentProxy );
	// set currentindex
	setCurrentIndex( mProject->child( 0, 0 )->index() );
	// loading...
	querySettings();
	//loadLanguages();
	// connections
	connect( cbScopes, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbOperators, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	/*
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
	connect( lvDirs, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( setDir( const QModelIndex& ) ) );
	connect( tbTranslationsPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	mReady = true;
	*/
	// settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}

void UIQMakeProjectSettings::closeEvent( QCloseEvent* e )
{
	if ( result() == QDialog::Rejected && QMessageBox::question( this, tr( "Cancel..." ), tr( "Are you sure ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		e->ignore();
}

QStringList UIQMakeProjectSettings::currentCONFIG() const
{
	// vars
	QStringList l;
	// get values
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->checkState() == Qt::Checked )
			if ( it->data( QtItem::VariableRole ).toString().toLower() == "config" )
				l << it->data( QtItem::ValueRole ).toString();
	// return values
	return l;
}

QStringList UIQMakeProjectSettings::currentQT() const
{
	// vars
	QStringList l;
	// get values
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->checkState() == Qt::Checked )
			if ( it->data( QtItem::VariableRole ).toString().toLower() == "qt" )
				l << it->data( QtItem::ValueRole ).toString();
	// return values
	return l;
}

QModelIndex UIQMakeProjectSettings::currentIndex()
{
	QModelIndex i;
	if ( lvContents->selectionModel()->selectedIndexes().count() )
		i = mContentProxy->mapToSource( lvContents->selectionModel()->selectedIndexes().at( 0 ) );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
		i = mScopesProxy->mapToSource( tvScopes->selectionModel()->selectedIndexes().at( 0 ) );
	return i;
	return QModelIndex();
}

void UIQMakeProjectSettings::setCurrentIndex( const QModelIndex& i )
{
	// TODO : need fix ( Qt Bug ?! ) calling setRootIndex on a index that have no child show all model
	// clear selection
	tvScopes->clearSelection();
	lvContents->clearSelection();
	// set/select current index
	if ( !i.isValid() )
		return;
	else if ( i.data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
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

QString UIQMakeProjectSettings::projectName() const
{ return mProject->name(); }

QString UIQMakeProjectSettings::projectPath() const
{ return mProject->canonicalPath(); }

QString UIQMakeProjectSettings::getFilePath( const QString& s )
{ return QFile::exists( mProject->canonicalFilePath( s ) ) ? mProject->canonicalFilePath( s ) : mProject->canonicalPath(); }

QString UIQMakeProjectSettings::getRelativeFilePath( const QString& s )
{ return mProject->relativeFilePath( s ); }

QString UIQMakeProjectSettings::getStringValues( const QString& v, const QString& o, const QString& s ) const
{ return mProject->getStringValues( v, o, s ); }

QStringList UIQMakeProjectSettings::getListValues( const QString& v, const QString& o, const QString& s ) const
{ return mProject->getListValues( v, o, s ); }

void UIQMakeProjectSettings::setStringValues( const QString& sv, const QString& v, const QString& o, const QString& s )
{ mProject->setStringValues( sv, v, o, s ); }

void UIQMakeProjectSettings::setListValues( const QStringList& lv, const QString& v, const QString& o, const QString& s )
{ mProject->setListValues( lv, v, o, s ); }

void UIQMakeProjectSettings::querySettings()
{
	// read all possible combinaison settings
	for ( int sc = 0; sc < cbScopes->count(); sc++ )
	{
		const QString Scope = cbScopes->itemText( sc );
		for ( int oc = 0; oc < cbOperators->count(); oc++ )
		{
			const QString Operator = cbOperators->itemText( oc );
			// set key
			const QString key = QString( "%1|%2|%3" ).arg( Scope ).arg( Operator );
			// check application page
			mOriginalSettings[ key.arg( "APP_TITLE" ) ] = getListValues( "APP_TITLE", Operator, Scope );
			mOriginalSettings[ key.arg( "APP_ICON" ) ] = getListValues( "APP_ICON", Operator, Scope );
			mOriginalSettings[ key.arg( "APP_HELP_FILE" ) ] = getListValues( "APP_HELP_FILE", Operator, Scope );
			mOriginalSettings[ key.arg( "APP_AUTHOR" ) ] = getListValues( "APP_AUTHOR", Operator, Scope );
			mOriginalSettings[ key.arg( "TEMPLATE" ) ] = getListValues( "TEMPLATE", Operator, Scope );
			mOriginalSettings[ key.arg( "LANGUAGE" ) ] = getListValues( "LANGUAGE", Operator, Scope );
			mOriginalSettings[ key.arg( "CONFIG" ) ] = getListValues( "CONFIG", Operator, Scope );
			mOriginalSettings[ key.arg( "QT" ) ] = getListValues( "QT", Operator, Scope );
			mOriginalSettings[ key.arg( "VERSION" ) ] = getListValues( "VERSION", Operator, Scope );
			mOriginalSettings[ key.arg( "APP_AUTO_INCREMENT" ) ] = getListValues( "APP_AUTO_INCREMENT", Operator, Scope );
			mOriginalSettings[ key.arg( "ENCODING" ) ] = getListValues( "ENCODING", Operator, Scope );
			mOriginalSettings[ key.arg( "DESTDIR" ) ] = getListValues( "DESTDIR", Operator, Scope );
			mOriginalSettings[ key.arg( "TARGET" ) ] = getListValues( "TARGET", Operator, Scope );
			mOriginalSettings[ key.arg( "LIBS" ) ] = getListValues( "LIBS", Operator, Scope );
			mOriginalSettings[ key.arg( "DEFINES" ) ] = getListValues( "DEFINES", Operator, Scope );
			mOriginalSettings[ key.arg( "INCLUDEPATH" ) ] = getListValues( "INCLUDEPATH", Operator, Scope );
			mOriginalSettings[ key.arg( "DEPENDPATH" ) ] = getListValues( "DEPENDPATH", Operator, Scope );
			mOriginalSettings[ key.arg( "VPATH" ) ] = getListValues( "VPATH", Operator, Scope );
			mOriginalSettings[ key.arg( "RESOURCES" ) ] = getListValues( "RESOURCES", Operator, Scope );
			mOriginalSettings[ key.arg( "DEF_FILE" ) ] = getListValues( "DEF_FILE", Operator, Scope );
			mOriginalSettings[ key.arg( "RC_FILE" ) ] = getListValues( "RC_FILE", Operator, Scope );
			mOriginalSettings[ key.arg( "RES_FILE" ) ] = getListValues( "RES_FILE", Operator, Scope );
			mOriginalSettings[ key.arg( "TRANSLATIONS_PATH" ) ] = getListValues( "", Operator, Scope );
			mOriginalSettings[ key.arg( "TRANSLATIONS" ) ] = getListValues( "TRANSLATIONS", Operator, Scope );
		}
	}
	// set settings original one
	mSettings = mOriginalSettings;
}

void UIQMakeProjectSettings::loadModules()
{
	// load modules informations
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

void UIQMakeProjectSettings::loadConfigs()
{
	// load configs informations
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

void UIQMakeProjectSettings::loadSettings()
{
	// load configs informations
	cbScopes->addItems( UISettingsQMake::readScopes() );
	cbOperators->addItems( UISettingsQMake::readOperators() );
	//
	QString c, s;
	QStringList l;
	// Application
	leTitle->setText( getStringValues( "APP_TITLE" ) );
	leTitle->setModified( false );
	leIcon->setText( getStringValues( "APP_ICON" ) );
	leIcon->setModified( false );
	if ( !leIcon->text().isEmpty() )
		lPixmap->setPixmap( QPixmap( QFileInfo( leIcon->text() ).isRelative() ? projectPath().append( "/" ).append( leIcon->text() ) : leIcon->text() ) );
	leHelpFile->setText( getStringValues( "APP_HELP_FILE" ) );
	leHelpFile->setModified( false );
	leAuthor->setText( getStringValues( "APP_AUTHOR" ) );
	leAuthor->setModified( false );
	s = getStringValues( "VERSION" );
	if ( !s.isEmpty() )
	{
		gbVersion->setChecked( true );
		l = s.split( "." );
		sbMajor->setValue( l.value( 0 ).toInt() );
		sbMinor->setValue( l.value( 1 ).toInt() );
		sbRelease->setValue( l.value( 2 ).toInt() );
		sbBuild->setValue( l.value( 3 ).toInt() );
		cbBuildAutoIncrement->setChecked( getStringValues( "APP_AUTO_INCREMENT" ).toInt() );
	}
	s = getStringValues( "TEMPLATE" );
	if ( cbTemplate->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbTemplate->addItem( s );
	cbTemplate->setCurrentIndex( cbTemplate->findText( s, Qt::MatchExactly ) );
	s = getStringValues( "LANGUAGE" );
	if ( cbLanguage->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbLanguage->addItem( s );
	cbLanguage->setCurrentIndex( cbLanguage->findText( s, Qt::MatchExactly ) );
	s = getStringValues( "ENCODING" );
	if ( s.isEmpty() )
		s = "UTF-8";
	if ( cbEncodings->findText( s, Qt::MatchExactly ) == -1 && !s.isEmpty() )
		cbEncodings->addItem( s );
	cbEncodings->setCurrentIndex( cbEncodings->findText( s, Qt::MatchExactly ) );
	c = getStringValues( "CONFIG", "+=" );
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
	// TODO
	//gbQtModules->setChecked( c.contains( "qt", Qt::CaseInsensitive ) );
	s = getStringValues( "QT" );
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

void UIQMakeProjectSettings::loadLanguages()
{
	leTranslationsPath->setText( getStringValues( "TRANSLATIONS_PATH" ) );
	leTranslationsPath->setModified( false );
	QStringList t = getListValues( "TRANSLATIONS" );
	QListWidgetItem*  it;
	bool b = false;
	for ( int i = QLocale::C +1; i < QLocale::Zulu +1; i++ )
	{
		QString s = QLocale::languageToString( ( QLocale::Language )i );
		QString f = mTranslationMask.arg( projectName() ).arg( s );
		it = new QListWidgetItem( s, lwTranslations );
		it->setCheckState( t.contains( f, Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
		if ( !b )
			b = it->checkState() == Qt::Checked;
	}
}

void UIQMakeProjectSettings::setDir( const QString& s )
{
	if ( QFile::exists( s ) )
		setDir( mDirs->index( s ) );
}

void UIQMakeProjectSettings::setDir( const QModelIndex& i )
{
	QString p = QDir( mDirs->filePath( i ) ).canonicalPath();
	lvDirs->setRootIndex( mDirs->index( p ) );
	QString s = leDir->text();
	if ( s.endsWith( "/" ) )
		s.chop( 1 );
	if ( s != p )
	{
		leDir->setText( p );
		lwFiles->clear();
		QDir d( p );
		foreach ( s, d.entryList( QDir::Files, QDir::Name ) )
			lwFiles->addItem( new QListWidgetItem( mDirs->iconProvider()->icon( QFileIconProvider::File ), s ) );
	}
}

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
		QString f = QFileInfo( s ).baseName().remove( projectName() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( projectName() ).arg( f ) )
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
		f = QFileInfo( it->text() ).baseName().remove( projectName() +'_' );
		// look if it's a project translation
		if ( it->text() == mTranslationMask.arg( projectName() ).arg( f ) )
		{
			// find item and uncheck it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Unchecked );
		}
		// add project translation if needed
		// got language
		f = QFileInfo( s ).baseName().remove( projectName() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( projectName() ).arg( f ) )
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
		QString f = QFileInfo( cit->text() ).baseName().remove( projectName() +'_' );
		// look if it's a project translation
		if ( cit->text() == mTranslationMask.arg( projectName() ).arg( f ) )
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
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application help root file" ), getFilePath( leHelpFile->text() ), tr( "HTMLs (*.htm *.html);;PDFs (*.pdf);;Windows Help Files (*.chm *.hlp)" ) );
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
	else if ( tb == tbTranslationsPath )
	{
		s = QFileDialog::getExistingDirectory( this, tr( "Choose your translations path" ), getFilePath( tbTranslationsPath->text() ) );
		if ( !s.isEmpty() )
		{
			leTranslationsPath->setText( getRelativeFilePath( s ) );
			leTranslationsPath->setModified( true );
		}
	}
}

void UIQMakeProjectSettings::sb_valueChanged( int )
{
	sbBuild->setValue( 0 );
}

void UIQMakeProjectSettings::on_cbTemplate_currentIndexChanged( const QString& s )
{
	// enable / disable widget according to template == subdirs
	bool b = s.toLower() == "subdirs";
	cbOrdered->setEnabled( b );
	leTitle->setDisabled( b );
	leIcon->setDisabled( b );
	tbIcon->setDisabled( b );
	leHelpFile->setDisabled( b );
	tbHelpFile->setDisabled( b );
	gbVersion->setDisabled( b );
	fBuildType->setDisabled( b );
	fWarn->setDisabled( b );
	cbBuildAll->setDisabled( b ? b : !rbDebugRelease->isChecked() );
	wLibraries->setDisabled( b );
	wSettings->setDisabled( b );
	wTranslations->setDisabled( b );
}

void UIQMakeProjectSettings::lw_currentItemChanged( QListWidgetItem* it, QListWidgetItem* )
{
	tbInformations->setHtml( it ? it->data( QtItem::HelpRole ).toString() : "" );
}

#include <QDebug>
void UIQMakeProjectSettings::cb_highlighted( int )
{
	// backup all datas for current scope , operator
	QStringList l;
	QString s;
	// set key
	const QString key = QString( "%1|%2|%3" ).arg( cbScopes->currentText() ).arg( cbOperators->currentText() );
	// check application page
	if ( leTitle->text() != mSettings[ key.arg( "APP_TITLE" ) ].join( " " ) )
		mSettings[ key.arg( "APP_TITLE" ) ] = QStringList( leTitle->text() );
	if ( leIcon->text() != mSettings[ key.arg( "APP_ICON" ) ].join( " " ) )
		mSettings[ key.arg( "APP_ICON" ) ] = QStringList( leIcon->text() );
	if ( leHelpFile->text() != mSettings[ key.arg( "APP_HELP_FILE" ) ].join( " " ) )
		mSettings[ key.arg( "APP_HELP_FILE" ) ] = QStringList( leHelpFile->text() );
	if ( leAuthor->text() != mSettings[ key.arg( "APP_AUTHOR" ) ].join( " " ) )
		mSettings[ key.arg( "APP_AUTHOR" ) ] = QStringList( leAuthor->text() );
	if ( cbTemplate->currentText() != mSettings[ key.arg( "TEMPLATE" ) ].join( " " ) )
		mSettings[ key.arg( "TEMPLATE" ) ] = QStringList( cbTemplate->currentText() );
	if ( cbLanguage->currentText() != mSettings[ key.arg( "LANGUAGE" ) ].join( " " ) )
		mSettings[ key.arg( "LANGUAGE" ) ] = QStringList( cbLanguage->currentText() );
	l = currentCONFIG();
	qDebug() << l;
	if ( l != mSettings[ key.arg( "CONFIG" ) ] )
		mSettings[ key.arg( "CONFIG" ) ] = l;
	l = currentQT();
	qDebug() << l;
	if ( l != mSettings[ key.arg( "QT" ) ] )
		mSettings[ key.arg( "QT" ) ] = l;
	s = QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() );
	if ( s == "0.0.0.0" )
		s.clear();
	if ( s != mSettings[ key.arg( "VERSION" ) ].join( " " ) )
		mSettings[ key.arg( "VERSION" ) ] = QStringList( QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() ) );
	if ( !gbVersion->isChecked() )
		mSettings[ key.arg( "VERSION" ) ].clear();
	if ( cbBuildAutoIncrement->isChecked() != mSettings[ key.arg( "APP_AUTO_INCREMENT" ) ].join( " " ).toInt() )
		mSettings[ key.arg( "APP_AUTO_INCREMENT" ) ] = QStringList( QString::number( cbBuildAutoIncrement->isChecked() ) );
	if ( !gbVersion->isChecked() )
		mSettings[ key.arg( "APP_AUTO_INCREMENT" ) ].clear();
	if ( cbEncodings->currentText() != mSettings[ key.arg( "ENCODING" ) ].join( " " ) )
		mSettings[ key.arg( "ENCODING" ) ] = QStringList( cbEncodings->currentText() );
	if ( leOutputPath->text() != mSettings[ key.arg( "DESTDIR" ) ].join( " " ) )
		mSettings[ key.arg( "DESTDIR" ) ] = QStringList( leOutputPath->text() );
	if ( leOutputName->text() != mSettings[ key.arg( "TARGET" ) ].join( " " ) )
		mSettings[ key.arg( "TARGET" ) ] = QStringList( leOutputName->text() );
	// translation page
	if ( leTranslationsPath->text() != mSettings[ key.arg( "TRANSLATIONS_PATH" ) ].join( " " ) )
		mSettings[ key.arg( "TRANSLATIONS_PATH" ) ] = QStringList( leTranslationsPath->text() );
	
	/*
	mSettings[ key.arg( "LIBS" ) ] = QStringList() << ;
	mSettings[ key.arg( "DEFINES" ) ] = QStringList() << ;
	mSettings[ key.arg( "INCLUDEPATH" ) ] = QStringList() << ;
	mSettings[ key.arg( "DEPENDPATH" ) ] = QStringList() << ;
	mSettings[ key.arg( "VPATH" ) ] = QStringList() << ;
	mSettings[ key.arg( "RESOURCES" ) ] = QStringList() << ;
	mSettings[ key.arg( "DEF_FILE" ) ] = QStringList() << ;
	mSettings[ key.arg( "RC_FILE" ) ] = QStringList() << ;
	mSettings[ key.arg( "RES_FILE" ) ] = QStringList() << ;
	mSettings[ key.arg( "TRANSLATIONS" ) ] = QStringList() << ;
	*/
}

void UIQMakeProjectSettings::on_cbScopes_currentIndexChanged( const QString& )
{ on_cbOperators_currentIndexChanged( cbOperators->currentText() ); }

void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& )
{
	// backup all datas for current scope , operator
	QStringList l;
	// set key
	const QString key = QString( "%1|%2|%3" ).arg( cbScopes->currentText() ).arg( cbOperators->currentText() );
	// check application page
	leTitle->setText( mSettings[ key.arg( "APP_TITLE" ) ].join( " " ) );
	leIcon->setText( mSettings[ key.arg( "APP_ICON" ) ].join( " " ) );
	leHelpFile->setText( mSettings[ key.arg( "APP_HELP_FILE" ) ].join( " " ) );
	leAuthor->setText( mSettings[ key.arg( "APP_AUTHOR" ) ].join( " " ) );
	cbTemplate->setCurrentIndex( cbTemplate->findText( mSettings[ key.arg( "TEMPLATE" ) ].join( " " ) ) );
	cbLanguage->setCurrentIndex( cbLanguage->findText( mSettings[ key.arg( "LANGUAGE" ) ].join( " " ) ) );
	/*
	l = currentCONFIG();
	if ( l != mSettings[ key.arg( "CONFIG" ) ] )
		mSettings[ key.arg( "CONFIG" ) ] = l;
	l = currentQT();
	if ( l != mSettings[ key.arg( "QT" ) ] )
		mSettings[ key.arg( "QT" ) ] = l;
	*/
	l = mSettings[ key.arg( "VERSION" ) ];
	sbMajor->setValue( 0 );
	sbMinor->setValue( 0 );
	sbRelease->setValue( 0 );
	sbBuild->setValue( 0 );
	gbVersion->setChecked( !l.isEmpty() );
	if ( !l.isEmpty() )
	{
		l = l.value( 0 ).split( "." );
		sbMajor->setValue( l.value( 0 ).toInt() );
		sbMinor->setValue( l.value( 1 ).toInt() );
		sbRelease->setValue( l.value( 2 ).toInt() );
		sbBuild->setValue( l.value( 3 ).toInt() );
	}
	cbBuildAutoIncrement->setChecked( mSettings[ key.arg( "APP_AUTO_INCREMENT" ) ].join( " " ).toInt() );
	cbEncodings->setCurrentIndex( cbEncodings->findText( mSettings[ key.arg( "ENCODING" ) ].join( " " ) ) );
	leOutputPath->setText( mSettings[ key.arg( "DESTDIR" ) ].join( " " ) );
	leOutputName->setText( mSettings[ key.arg( "TARGET" ) ].join( " " ) );
	// translation page
	leTranslationsPath->setText( mSettings[ key.arg( "TRANSLATIONS_PATH" ) ].join( " " ) );
	// load variables values
	on_cbVariables_currentIndexChanged( cbVariables->currentText() );
}

void UIQMakeProjectSettings::on_lwFiles_itemDoubleClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	QFileInfo f( leDir->text().append( "/" ).append( i->text() ) );
	QString s = f.completeSuffix().toLower();
	// TODO : add entry to qmake plugin so user can configure list himself
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

void UIQMakeProjectSettings::on_cbVariables_currentIndexChanged( const QString& s )
{
	/*
	if ( !mReady )
		return;
	QString k = QString( "%1|%2|%3" ).arg( cbScopes->currentText(), cbOperators->currentText(), s );
	lwValues->clear();
	if ( mSettings.contains( k ) )
		lwValues->addItems( mSettings[ k ] );
	else
		lwValues->addItems( getListValues( s, cbOperators->currentText(), cbScopes->currentText() ) );
	*/
}

void UIQMakeProjectSettings::on_pbAddValue_clicked()
{
	bool b;
	QString s = QInputDialog::getText( this, tr( "Add value..." ), tr( "Set your value content :" ), QLineEdit::Normal, QString::null, &b );
	if ( b && !s.isEmpty() )
		addValue( s );
}

void UIQMakeProjectSettings::on_pbEditValue_clicked()
{
	QListWidgetItem* it = lwValues->currentItem();
	if ( !it )
		return;
	bool b;
	QString s = QInputDialog::getText( this, tr( "Edit value..." ), tr( "Edit your value content :" ), QLineEdit::Normal, it->text(), &b );
	if ( b && !s.isEmpty() )
		editValue( s );
}

void UIQMakeProjectSettings::on_pbRemoveValue_clicked()
{
	removeValue();
}

void UIQMakeProjectSettings::on_pbClearValues_clicked()
{
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
		while ( lwValues->count() )
			removeValue( lwValues->item( 0 )->text() );
	else
		lwValues->clear();
}

void UIQMakeProjectSettings::on_lwTranslations_itemChanged( QListWidgetItem* it )
{
	if ( !it )
		return;
	// if translations checked and no translations path, set one
	if ( leTranslationsPath->text().isEmpty() )
	{
		leTranslationsPath->setText( "translations" );
		leTranslationsPath->setModified( true );
	}
	if ( !lwTranslations->isVisible() )
		return;
	// get translation file
	QString s = mTranslationMask.arg( projectName() ).arg( it->text() );
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
					mSettings[ k ] = getListValues( "TRANSLATIONS" );
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
					mSettings[ k ] = getListValues( "TRANSLATIONS" );
				if ( mSettings.value( k ).contains( s, Qt::CaseInsensitive ) )
					mSettings[ k ].removeAll( s );
			}
			break;
		default:
			break;
	}
}

void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{
	// TODO : Fix me
	setCurrentIndex( mScopesProxy->mapToSource( i ) );
}

void UIQMakeProjectSettings::on_tvScopes_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( mScopesProxy->mapToSource( i ) ), this )->exec();
}

void UIQMakeProjectSettings::on_lvContents_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( mContentProxy->mapToSource( i ) ), this )->exec();
}

void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	UIItemSettings::edit( mModel, 0, this )->exec();
}

void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( i ), this )->exec();
}

void UIQMakeProjectSettings::on_tbRemove_clicked()
{
	ProjectItem* it = mModel->itemFromIndex( currentIndex() );
	if ( it )
		it->remove();
}

void UIQMakeProjectSettings::on_tbClear_clicked()
{ mProject->removeRows( 0, mProject->rowCount() ); }

void UIQMakeProjectSettings::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mModel->itemFromIndex( i )->moveUp() )
			setCurrentIndex( i.sibling( i.row() -1, i.column() ) );
}

void UIQMakeProjectSettings::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mModel->itemFromIndex( i )->moveDown() )
			setCurrentIndex( i.sibling( i.row() +1, i.column() ) );
}

void UIQMakeProjectSettings::on_dbbButtons_helpRequested()
{
	QString s;
	switch ( lwMenu->currentRow() )
	{
		case 0:
			s = tr( "<b>Application</b>: Here you can configure global project settings." );
			break;
		case 1:
			s = tr( "<b>Libraries</b>: Here you can easily check the Qt module you want, and add differents configuration flag." );
			break;
		case 2:
			s = tr( "<b>Settings</b>: Here you can configure differents settings for many scopes and operators." );
			break;
		case 3:
			s = tr( "<b>Translations</b>: Here you can easily check the languages you want to provide for your project." );
			break;
		case 4:
			s = tr( "<b>Advanced</b>: This is a direct access to the project model, be carrefull while editing items as you can break your project." );
			break;
	}
	if ( !s.isEmpty() )
		QWhatsThis::showText( dbbButtons->button( QDialogButtonBox::Help )->mapToGlobal( QPoint( 0, 0 ) ), s );
}

void UIQMakeProjectSettings::accept()
{
	// backup current settings state
	cb_highlighted( 0 );
	// applications
	QStringList s, c;
	// if subdirs only fill these and return
	if ( !cbEncodings->currentText().isEmpty() )
		setStringValues( cbEncodings->currentText(), "ENCODING" );
	if ( !cbTemplate->currentText().isEmpty() )
		setStringValues( cbTemplate->currentText(), "TEMPLATE" );
	if ( !cbLanguage->currentText().isEmpty() )
		setStringValues( cbLanguage->currentText(), "LANGUAGE" );
	if ( leAuthor->isModified() )
		setStringValues( leAuthor->text(), "APP_AUTHOR" );
	if ( cbTemplate->currentText().toLower() == "subdirs" )
	{
		if ( cbOrdered->isChecked() )
			s << "ordered";
		setListValues( s, "CONFIG", "+=" );
		// close dialog
		QDialog::accept();
		return;
	}
	//
	if ( leTitle->isModified() )
		setStringValues( leTitle->text(), "APP_TITLE" );
	if ( leIcon->isModified() )
		setStringValues( leIcon->text(), "APP_ICON" );
	if ( leHelpFile->isModified() )
		setStringValues( leHelpFile->text(), "APP_HELP_FILE" );
	if ( gbVersion->isChecked() )
	{
		setStringValues( QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() ), "VERSION" );
		setStringValues( QString::number( cbBuildAutoIncrement->isChecked() ), "APP_AUTO_INCREMENT" );
	}
	else
	{
		setStringValues( QString::null, "VERSION" );
		setStringValues( QString::null, "APP_AUTO_INCREMENT" );
	}
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
	setListValues( s, "CONFIG", "+=" );
	setListValues( c, "QT" );
	s.clear();
	c.clear();
	// translations
	if ( leTranslationsPath->isModified() )
		setStringValues( leTranslationsPath->text(), "TRANSLATIONS_PATH" );
	// settings
	foreach ( QString v, mSettings.keys() )
	{
		QStringList l = v.split( "|" );
		setListValues( mSettings.value( v ), l.at( 2 ), l.at( 1 ), l.at( 0 ) );
	}
	// close dialog
	QDialog::accept();
}

void UIQMakeProjectSettings::reject()
{
	setResult( QDialog::Rejected );
	close();
}
