/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
/*!
	\file ClassBrowserSettings.cpp
	\date 2009-05-01
	\author Filipe AZEVEDO
	\brief Settings widget of ClassBrowser plugin
*/
#include "ClassBrowserSettings.h"
#include "ClassBrowser.h"

#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>

/*!
	Creates settings widget
	\param plugin Pointer to ClassBrowser plugin
	\param parent Parent widget of settings widget
*/
ClassBrowserSettings::ClassBrowserSettings( ClassBrowser* plugin, QWidget* parent )
	: QWidget( parent )
{
	// retain plugin
	mPlugin = plugin;
	
	// integration
	QLabel* label = new QLabel( tr( "Integration Mode:" ) );
	cbIntegrationMode = new QComboBox;
	QHBoxLayout* hbox = new QHBoxLayout;
	cbIntegrationMode->addItem( tr( "Dock" ), ClassBrowser::imDock );
	cbIntegrationMode->addItem( tr( "Combo" ), ClassBrowser::imCombo );
	cbIntegrationMode->addItem( tr( "Both" ), ClassBrowser::imBoth );
	cbIntegrationMode->setCurrentIndex( cbIntegrationMode->findData( plugin->integrationMode() ) );
	hbox->addWidget( label );
	hbox->addWidget( cbIntegrationMode );
	
	// list editor
	mPathEditor = new pPathListEditor( this, tr( "System include paths" ) );
	mPathEditor->setValues( plugin->systemPaths() );
	
	mStringEditor = new pStringListEditor( this, tr( "Filtered file suffixes" ) );
	mStringEditor->setValues( plugin->filteredSuffixes() );
	
	// apply button
	QDialogButtonBox* dbbApply = new QDialogButtonBox( this );
	dbbApply->addButton( QDialogButtonBox::Apply );
	
	// global layout
	QVBoxLayout* vbox = new QVBoxLayout( this );
	vbox->addLayout( hbox );
	vbox->addWidget( mPathEditor );
	vbox->addWidget( mStringEditor );
	vbox->addWidget( dbbApply );
	
	// connections
	connect( dbbApply->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), this, SLOT( applySettings() ) );
}

/*!
	Handler of clicking Apply button. Applying settings
*/
void ClassBrowserSettings::applySettings()
{
	mPlugin->setIntegrationMode( (ClassBrowser::IntegrationMode)cbIntegrationMode->itemData( cbIntegrationMode->currentIndex() ).toInt() );
	mPlugin->setSystemPaths( mPathEditor->values() );
	mPlugin->setFilteredSuffixes( mStringEditor->values() );
}
