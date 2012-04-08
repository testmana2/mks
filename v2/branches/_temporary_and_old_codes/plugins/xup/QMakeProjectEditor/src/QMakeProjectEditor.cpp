#include "QMakeProjectEditor.h"
#include "QMakeXUPItem.h"
#include "ui/UISettingsQMakeXUPProjectEditor.h"
#include "QMakeProjectEditorDialog.h"

#include <coremanager.h>
#include <maininterface.h>
#include <xupmanager.h>

QMakeProjectEditor::QMakeProjectEditor()
{
    mItem = 0;
    // set plugin infos
    mPluginInfos.Caption = tr( "QMake Project Editor" );
    mPluginInfos.Description = tr( "QMake Project Settings Editor for XUPManager" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iXUP;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.1.0";
    mPluginInfos.Enabled = false;
}

QMakeProjectEditor::~QMakeProjectEditor()
{
    if ( isEnabled() )
        setEnabled( false );
}

bool QMakeProjectEditor::setEnabled( bool b )
{
    if ( b && !isEnabled() )
    {
        // register qmake item
        mItem = new QMakeXUPItem;
        MonkeyCore::projectsManager()->registerItem( mItem );
        // set plugin enabled
        mPluginInfos.Enabled = true;
    }
    else if ( !b && isEnabled() )
    {
        // unregister qmake item, unregistering auto delete the item
        MonkeyCore::projectsManager()->unRegisterItem( mItem );
        // set plugin disabled
        mPluginInfos.Enabled = false;
    }
    // return default value
    return true;
}

QWidget* QMakeProjectEditor::settingsWidget()
{ return new UISettingsQMakeXUPProjectEditor; }

bool QMakeProjectEditor::editProject( XUPProjectItem* project )
{
    if ( !project )
        return false;
    return QMakeProjectEditorDialog( project, MonkeyCore::mainWindow() ).exec();
}

Q_EXPORT_PLUGIN2( XUPQMakeProjectEditor, QMakeProjectEditor )
