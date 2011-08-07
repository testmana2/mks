#include "UIXUPEditor.h"

#include <xupmanager/core/XUPProjectItem.h>
#include "XUPDynamicFolderItem.h"
#include <shared/MkSFileDialog.h>
#include <pMonkeyStudio.h>
#include <coremanager/MonkeyCore.h>
#include <pluginsmanager/PluginsManager.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

UIXUPEditor::UIXUPEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	// init dialog
	setupUi( this );
	
	// set dialog infos
	setWindowIcon( project->displayIcon() );
	setWindowTitle( tr( "Project Editor - %2" ).arg( project->displayText() ) );
	
	// set size hint for page item ( left panel )
	for ( int i = 0; i < lwPages->count(); i++ ) {
		QListWidgetItem* item = lwPages->item( i );
		item->setSizeHint( QSize( 154, 40 ) );
	}
	
	// does not show variable editor by default
	//setVariableEditorVisible( false );
	
	/*veEditor->setQuoteSpacedValuesVisible( false );
	veEditor->setQuoteSpacedValuesEnabled( false );*/
	
	// commands
	const BasePluginTypeList types = BasePluginTypeList() << BasePlugin::iCLITool << BasePlugin::iDebugger;
	const QStringList parsers = MonkeyCore::consoleManager()->parsersName();
	
	// commands editor
	ceEditor->setCommandTypes( types );
	ceEditor->setParsers( parsers );
	
	// init project settings dialog
	init( project );
	
	// set correct page
	lwPages->setCurrentRow( 0 );
}

UIXUPEditor::~UIXUPEditor()
{
}

void UIXUPEditor::setVariableEditorVisible( bool visible )
{
	lwPages->item( 2 )->setHidden( !visible );
}

bool UIXUPEditor::isVariableEditorVisible() const
{
	return !lwPages->item( 2 )->isHidden();
}

void UIXUPEditor::updateMainFileComboBox( const QString& selectFile )
{
	cbMainFile->clear();
	
	const QStringList sources = mProject->sourceFiles();
	QMap<QString, QString> files;
	
	foreach ( const QString& file, sources )
	{
		const QString fileName = mProject->relativeFilePath( file );
		
		files[ fileName.toLower() ] = fileName;
	}
	
	cbMainFile->addItems( files.values() );
	const int index = cbMainFile->findText( mProject->relativeFilePath( selectFile ) );
	cbMainFile->setCurrentIndex( index );
}

void UIXUPEditor::init( XUPProjectItem* project )
{
	mProject = project;
	const XUPDynamicFolderSettings folder = XUPProjectItemHelper::projectDynamicFolderSettings( mProject );

	leProjectName->setText( mProject->attribute( "name" ) );
	gbDynamicFolder->setChecked( folder.Active );
	leDynamicFolder->setText( folder.AbsolutePath );
	gbDynamicFilesPatterns->setValues( folder.FilesPatterns );
	updateMainFileComboBox( XUPProjectItemHelper::projectSettingsValue( mProject, "MAIN_FILE" ) );
	feEditor->setup( mProject );
	veEditor->setup( mProject );
	ceEditor->setCommands( XUPProjectItemHelper::projectCommands( mProject ) );
	ceEditor->setCurrentType( ceEditor->commandTypes().first() );
}

void UIXUPEditor::on_tbDynamicFolder_clicked()
{
	QString path = leDynamicFolder->text();
	path = QFileDialog::getExistingDirectory( this, tr( "Select the folder to monitor" ), path );
	
	if ( path.isEmpty() )
	{
		return;
	}
	
	leDynamicFolder->setText( path );
}

void UIXUPEditor::accept()
{
	const XUPDynamicFolderSettings oldSettings = XUPProjectItemHelper::projectDynamicFolderSettings( mProject );
	XUPDynamicFolderSettings settings;
	settings.Active = gbDynamicFolder->isChecked();
	settings.AbsolutePath = leDynamicFolder->text();
	settings.FilesPatterns = gbDynamicFilesPatterns->values();
	
	if ( oldSettings.AbsolutePath != settings.AbsolutePath || oldSettings.FilesPatterns != settings.FilesPatterns ) {
		XUPItem* itemFolder = XUPProjectItemHelper::projectDynamicFolderItem( mProject, false );
		
		if ( itemFolder ) {
			itemFolder->parent()->removeChild( itemFolder );
		}
	}
	
	feEditor->finalize();
	ceEditor->finalize();
	veEditor->finalize();
	mProject->setAttribute( "name", leProjectName->text() );
	XUPProjectItemHelper::setProjectSettingsValue( mProject, "MAIN_FILE", cbMainFile->currentText() );
	XUPProjectItemHelper::setProjectDynamicFolderSettings( mProject, settings );
	XUPProjectItemHelper::setProjectCommands( mProject, ceEditor->commands() );
	
	if ( settings.Active && !settings.AbsolutePath.isEmpty() && QFile::exists( settings.AbsolutePath ) ) {
		XUPDynamicFolderItem* dynamicFolderItem = XUPProjectItemHelper::projectDynamicFolderItem( mProject, true );
		dynamicFolderItem->setRootPath( settings.AbsolutePath, settings.FilesPatterns );
	}
	
	// close dialog
	QDialog::accept();
}
