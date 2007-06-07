#include "UIPluginsSettings.h"
#include "PluginsManager.h"
#include "Settings.h"
//
#include <QMetaEnum>
//
QPointer<UIPluginsSettings> UIPluginsSettings::mSelf = 0L;
//
UIPluginsSettings* UIPluginsSettings::self( PluginsManager* m, QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIPluginsSettings( m, p );
	return mSelf;
}
//
UIPluginsSettings::UIPluginsSettings( PluginsManager* m, QWidget* p )
	: QDialog( p ), mPluginsManager( m )
{
	Q_ASSERT( mPluginsManager != 0 );
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// fill list with plugins type
	const QMetaObject mo = BasePlugin::staticMetaObject;
	QMetaEnum e = mo.enumerator( mo.indexOfEnumerator( "Type" ) );
	for ( int i = 0; i < e.keyCount() -1; i++ )
		cbType->addItem( e.key( i ), e.value( i ) );
	// update plugins list
	updateList();
	// connections
	connect( cbType, SIGNAL( currentIndexChanged( int ) ), this, SLOT( currentIndexChanged( int ) ) );
	connect( lwNames, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( itemClicked( QListWidgetItem* ) ) );
	// activate first plugin
	if ( lwNames->count() )
	{
		lwNames->setCurrentRow( 0 );
		itemClicked( lwNames->item( 0 ) );
	}
}
//
void UIPluginsSettings::clearInfos()
{
	leCaption->clear();
	leName->clear();
	leVersion->clear();
	leType->clear();
	teDescription->clear();
	cbAutoInstall->setChecked( false );
	cbInstallUninstall->setChecked( false );
	swWidgets->setCurrentIndex( -1 );
}
//
void UIPluginsSettings::updateList()
{
	// clear
	while ( swWidgets->count() )
		delete swWidgets->widget( 0 );
	lwNames->clear();
	// create items
	for ( int i = 0; i < mPluginsManager->plugins().count(); i++ )
	{
		BasePlugin* bp = mPluginsManager->plugins().at( i ); // got plugin
		QListWidgetItem* it = new QListWidgetItem( bp->infos().Name, lwNames ); // create item
		it->setData( Qt::UserRole, i ); // plugin index
		it->setData( Qt::UserRole +1, swWidgets->addWidget( bp->settingsWidget() ) ); // settings widget index
		it->setData( Qt::UserRole +2, bp->infos().Type ); // plugin type
		it->setData( Qt::UserRole +3, Settings::current()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() ); // plugin auto install
	}
}
//
void UIPluginsSettings::currentIndexChanged( int i )
{
	for ( int j = 0; j < lwNames->count(); j++ )
	{
		QListWidgetItem* it = lwNames->item( j ); // get item
		int pt = it->data( Qt::UserRole +2 ).toInt(); // get item plugin type
		int ct = cbType->itemData( i ).toInt(); // get current visible type
		it->setHidden( ( ct != BasePlugin::iAll && ct != pt ) ? true : false ); // show or hide the plugin
	}
	// select the first visible item if current is hidden
	if ( lwNames->count() && lwNames->currentItem()->isHidden() )
	{
		for ( int j = 0; j < lwNames->count(); j++ )
		{
			QListWidgetItem* it = lwNames->item( j ); // get item
			if ( !it->isHidden() )
			{
				lwNames->setCurrentItem( it );
				itemClicked( it );
				return;
			}
		}
	}
	// in case of no entry/plugins
	clearInfos();
}
//
void UIPluginsSettings::itemClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	BasePlugin::PluginInfos pi = mPluginsManager->plugins().at( i->data( Qt::UserRole ).toInt() )->infos();
	leCaption->setText( pi.Caption );
	leName->setText( pi.Name );
	leVersion->setText( pi.Version );
	leType->setText( QString::number( pi.Type ) );
	teDescription->setPlainText( pi.Description );
	cbAutoInstall->setChecked( i->data( Qt::UserRole +3 ).toBool() );
	cbInstallUninstall->setChecked( pi.Installed );
	swWidgets->setCurrentIndex( i->data( Qt::UserRole +1 ).toInt() );
}
//
void UIPluginsSettings::on_cbAutoInstall_clicked( bool b )
{
	if ( lwNames->currentItem() && !lwNames->currentItem()->isHidden() )
		lwNames->currentItem()->setData( Qt::UserRole +3, b );
}
//
void UIPluginsSettings::on_cbInstallUninstall_clicked( bool b )
{
	if ( lwNames->currentItem() && !lwNames->currentItem()->isHidden() )
	{
		BasePlugin* bp = mPluginsManager->plugins().at( lwNames->currentRow() );
		if ( b )
			bp->install();
		else
			bp->uninstall();
		cbInstallUninstall->setChecked( bp->infos().Installed );
	}
}
//
void UIPluginsSettings::accept()
{
	// save auto install states
	for ( int i = 0; i < lwNames->count(); i++ )
	{
		QListWidgetItem* it = lwNames->item( i );
		Settings::current()->setValue( QString( "Plugins/%1" ).arg( it->text() ), it->data( Qt::UserRole +3 ).toBool() );
	}
	QDialog::accept();
}
