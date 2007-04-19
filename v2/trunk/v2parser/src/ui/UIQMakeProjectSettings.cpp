#include "UIQMakeProjectSettings.h"
#include "QMakeProjectScopesProxy.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "UIItemSettings.h"
//
UIQMakeProjectSettings::UIQMakeProjectSettings( QMakeProjectModel* m, QWidget* p )
	: QDialog( p ), mProxy( 0 ), mProject( m )
{
	setupUi( this );
	//
	for ( int i = 0; i < lwQtModules->count(); i++ )
		lwQtModules->item( i )->setCheckState( Qt::Unchecked );
	QFont fo( lwCompilerFlags->font() );
	fo.setPointSize( 8 );
	fo.setBold( true );
	for ( int i = 0; i < lwCompilerFlags->count(); i++ )
	{
		QListWidgetItem* it = lwCompilerFlags->item( i );
		Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
		if ( !it->text().toLower().contains( "only" ) )
			f |= Qt::ItemIsEnabled;
		it->setFlags( f );
		if ( f & Qt::ItemIsEnabled )
			it->setCheckState( Qt::Unchecked );
		else
		{
			it->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
			it->setBackground( Qt::darkBlue );
			it->setForeground( Qt::white );
			it->setFont( fo );
		}
	}
	//
	mProxy = new QMakeProjectScopesProxy( mProject );
	tvScopes->setModel( mProxy );
	lvContents->setModel( mProject );
	setCurrentIndex( mProject->index( 0, 0 ) );
	loadSettings();
}
//
UIQMakeProjectSettings::~UIQMakeProjectSettings()
{
	delete mProxy;
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
void UIQMakeProjectSettings::loadSettings()
{
	QString s;
	QStringList l;
	// Application
	leTitle->setText( mProject->getStringValues( "APP_TITLE" ) );
	leIcon->setText( mProject->getStringValues( "APP_ICON" ) );
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
	s = mProject->getStringValues( "CONFIG", "+=" );
	if ( s.indexOf( "debug_and_release" ) != -1 )
		rbDebugRelease->setChecked( true );
	else if ( s.indexOf( "debug" ) != -1 )
		rbDebug->setChecked( true );
	else if ( s.indexOf( "release" ) != -1 )
		rbRelease->setChecked( true );
	else
		rbRelease->setChecked( true );
	if ( s.indexOf( "warn_off" ) != -1 )
		rbWarnOff->setChecked( true );
	else if ( s.indexOf( "warn_onn" ) != -1 )
		rbWarnOn->setChecked( true );
	else
		rbWarnOff->setChecked( true );
	if ( s.indexOf( "build_all" ) != -1 )
		cbBuildAll->setChecked( true );
	if ( s.indexOf( "ordered" ) != -1 )
		cbOrdered->setChecked( true );
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
