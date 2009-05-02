#include "UIPluginsSettingsElement.h"
#include "UIPluginsSettingsAbout.h"
#include "../BasePlugin.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../settingsmanager/Settings.h"

#include <QDesktopWidget>

UIPluginsSettingsElement::UIPluginsSettingsElement( BasePlugin* plugin, QWidget* p )
	: QWidget( p )
{
	Q_ASSERT( plugin );
	mPlugin = plugin;
	BasePlugin::PluginInfos infos = mPlugin->infos();
	
	// setup dialog
	setupUi( this );
	cbEnabled->setChecked( mPlugin->isEnabled() );
	
	if ( !mPlugin->pixmap().isNull() )
	{
		lIcon->setPixmap( mPlugin->pixmap().scaledToWidth( lIcon->maximumWidth(), Qt::SmoothTransformation ) );
	}
	
	lIcon->setEnabled( cbEnabled->isChecked() );
	lTitle->setText( infos.Caption );
	lDescription->setText( infos.Description );
	pbSettings->setVisible( mPlugin->haveSettingsWidget() );
	cbNeverEnable->setChecked( mPlugin->neverEnable() );
}

BasePlugin* UIPluginsSettingsElement::plugin() const
{ return mPlugin; }

void UIPluginsSettingsElement::on_cbEnabled_toggled( bool checked )
{
	if ( checked )
		cbNeverEnable->setChecked( false );
	lIcon->setEnabled( checked );
	mPlugin->setEnabled( checked );
	MonkeyCore::settings()->setValue( QString( "Plugins/%1" ).arg( mPlugin->infos().Name ), checked );
}

void UIPluginsSettingsElement::on_pbSettings_clicked()
{
	QWidget* widget = mPlugin->settingsWidget();
	
#ifdef Q_OS_MAC
	widget->setParent( qApp->activeWindow(), Qt::Sheet );
#else
	widget->setParent( qApp->activeWindow(), Qt::Dialog );
#endif
	widget->setWindowModality( Qt::ApplicationModal );
	widget->setAttribute( Qt::WA_DeleteOnClose );
	widget->setWindowIcon( mPlugin->pixmap() );
	widget->setWindowTitle( tr( "Settings - %1" ).arg( mPlugin->infos().Caption ) );
	widget->adjustSize();
	
	QRect rect = widget->frameGeometry();
	QRect drect = qApp->desktop()->availableGeometry( qApp->activeWindow() );
	rect.moveCenter( drect.center() );
	
	widget->move( rect.topLeft() );
	widget->show();
}

void UIPluginsSettingsElement::on_pbAbout_clicked()
{
	UIPluginsSettingsAbout psa( mPlugin, window() );
	psa.resize( psa.sizeHint() );
	psa.exec();
}

void UIPluginsSettingsElement::on_cbNeverEnable_toggled( bool checked )
{
	if( checked )
		cbEnabled->setChecked( false );
	MonkeyCore::settings()->setValue( QString( "Plugins/%1/NeverEnable" ).arg( mPlugin->infos().Name ), checked );
}
