/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : Ctags2Api.cpp
** Date      : 2007-11-04T22:50:34
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "Ctags2Api.h"
#include "pMenuBar.h"
#include "UICtags2Api.h"

Ctags2Api::Ctags2Api()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Api File Generator" );
	mPluginInfos.Description = tr( "This plugin allow to generate api file using ctags." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

Ctags2Api::~Ctags2Api()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool Ctags2Api::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = pMenuBar::instance()->action( "mEdit/aCtags2Api",  tr( "Api File Generator" ), QIcon( ":/icons/Ctags2Api.png" ), tr( "Ctrl+Alt+G" ), mPluginInfos.Description );
		connect( a, SIGNAL( triggered() ), this, SLOT( UICtags2Api_show() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete action
		delete pMenuBar::instance()->action( "mEdit/aCtags2Api" );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void Ctags2Api::UICtags2Api_show()
{
	// create dialog
	UICtags2Api w;
	// restore settings
	w.leCtagsBinary->setText( settingsValue( "CtagsBinary", w.leCtagsBinary->text() ).toString() );
	w.cbRemovePrivate->setChecked( settingsValue( "RemovePrivate", w.cbRemovePrivate->isChecked() ).toBool() );
	w.cbWindowsMode->setChecked( settingsValue( "WindowsMode", w.cbWindowsMode->isChecked() ).toBool() );
	w.cbLetter->setCurrentIndex( w.cbLetter->findText( settingsValue( "Letter", w.cbLetter->currentText() ).toString() ) );
	w.exec();
	// save ctags file and options
	setSettingsValue( "CtagsBinary", w.leCtagsBinary->text() );
	setSettingsValue( "RemovePrivate", w.cbRemovePrivate->isChecked() );
	setSettingsValue( "WindowsMode", w.cbWindowsMode->isChecked() );
	setSettingsValue( "Letter", w.cbLetter->currentText() );
}

Q_EXPORT_PLUGIN2( BaseCtags2Api, Ctags2Api )
