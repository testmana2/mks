#include "GeneralProperties.h"
#include "main.h"
#include "pWorkspace.h"
#include "pOpenedFileModel.h"

#include <XmlObjectProcessor.h>

#include <QDesktopServices>

GeneralProperties::GeneralProperties()
{
    mSaveFilesOnCustomAction = true;
    mDefaultProjectDirectory = QString( "%1/Projects/%2" ).arg( QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation ) ).arg( PACKAGE_NAME );
    mShowQuickFileAccess = false;
    mSortingMode = pOpenedFileModel::OpeningOrder;
    mTabMode = pWorkspace::NoTabs;
    mSaveSessionOnClose = true;
    mRestoreSessionOnOpen = true;
}

void GeneralProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "SaveFilesOnCustomAction", mSaveFilesOnCustomAction );
    container->processData( "DefaultProjectDirectory", mDefaultProjectDirectory );
    container->processData( "ShowQuickFileAccess", mShowQuickFileAccess );
    container->processData( "SortingMode", mSortingMode );
    container->processData( "TabMode", mTabMode );
    container->processData( "SaveSessionOnClose", mSaveSessionOnClose );
    container->processData( "RestoreSessionOnOpen", mRestoreSessionOnOpen );
}
