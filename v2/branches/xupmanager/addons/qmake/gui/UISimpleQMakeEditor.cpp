#include "UISimpleQMakeEditor.h"
#include "../../../core/XUPProjectItem.h"

#include <QFileDialog>
#include <QDebug>

UISimpleQMakeEditor::UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
	
	foreach ( QRadioButton* rb, gbProjectType->findChildren<QRadioButton*>() )
	{
		connect( rb, SIGNAL( toggled( bool ) ), this, SLOT( projectTypeChanged() ) );
	}
	
#warning should use settings from qmake plugin setttings
	QListWidgetItem* coreItem = new QListWidgetItem( lwQtModules );
	coreItem->setText( "QtCore" );
	coreItem->setData( Qt::UserRole, "core" );
	
	QListWidgetItem* guiItem = new QListWidgetItem( lwQtModules );
	guiItem->setText( "QtGui" );
	guiItem->setData( Qt::UserRole, "gui" );
	
	QListWidgetItem* networkItem = new QListWidgetItem( lwQtModules );
	networkItem->setText( "QtNetwork" );
	networkItem->setData( Qt::UserRole, "network" );
	
	QListWidgetItem* svgItem = new QListWidgetItem( lwQtModules );
	svgItem->setText( "QtSvg" );
	svgItem->setData( Qt::UserRole, "svg" );
	
	QListWidgetItem* designerItem = new QListWidgetItem( lwModules );
	designerItem->setText( "Designer" );
	designerItem->setData( Qt::UserRole, "designer" );
	
	QListWidgetItem* phononItem = new QListWidgetItem( lwModules );
	phononItem->setText( "Phonon" );
	phononItem->setData( Qt::UserRole, "phonon" );
	
	QListWidgetItem* webkitkItem = new QListWidgetItem( lwModules );
	webkitkItem->setText( "WebKit" );
	webkitkItem->setData( Qt::UserRole, "webkit" );
	
	init( project );
}

UISimpleQMakeEditor::~UISimpleQMakeEditor()
{
}

void UISimpleQMakeEditor::updateProjectFiles()
{
	int pType = mProject->projectType();
	foreach ( const QString& variable, mFileVariables )
	{
		QTreeWidgetItem* topItem = mProjectFilesItems.value( variable );
		QStringList files = mProject->splitMultiLineValue( mValues[ variable ] );
		if ( topItem && files.isEmpty() )
		{
			delete mProjectFilesItems.take( variable );
		}
		else if ( !files.isEmpty() )
		{
			if ( !topItem )
			{
				topItem = new QTreeWidgetItem( twFiles, QTreeWidgetItem::UserType +1 );
				topItem->setText( 0, mProject->projectInfos()->displayText( pType, variable ) );
				topItem->setIcon( 0, mProject->projectInfos()->displayIcon( pType, variable ) );
				mProjectFilesItems[ variable ] = topItem;
			}
			
			for ( int i = 0; i < topItem->childCount(); i++ )
			{
				QTreeWidgetItem* item = topItem->child( i );
				QString fn = item->data( 0, Qt::UserRole ).toString();
				if ( files.contains( fn ) )
				{
					files.removeAll( fn );
				}
			}
			
			foreach ( const QString& fn, files )
			{
				QTreeWidgetItem* item = new QTreeWidgetItem( topItem, QTreeWidgetItem::UserType );
				item->setText( 0, fn );
				item->setData( 0, Qt::UserRole, fn );
				item->setIcon( 0, mProject->projectInfos()->displayIcon( XUPProjectItem::XUPProject, "FILES" ) );
			}
		}
	}
}

void UISimpleQMakeEditor::init( XUPProjectItem* project )
{
	mFileVariables = project->projectInfos()->fileVariables( project->projectType() );
	QString value;
	QStringList values;
	mProject = project;
	mValues.clear();
	
	mValues[ "TEMPLATE" ] = project->interpretVariable( "TEMPLATE" );
	mValues[ "CONFIG" ] = project->interpretVariable( "CONFIG" );
	mValues[ "QT" ] = project->interpretVariable( "QT" );
	mValues[ "TARGET" ] = project->interpretVariable( "TARGET" );
	mValues[ "DESTDIR" ] = project->interpretVariable( "DESTDIR" );
	mValues[ "DLLDESTDIR" ] = project->interpretVariable( "DLLDESTDIR" );
	
	foreach ( const QString& variable, mFileVariables )
	{
		mValues[ variable ] = project->interpretVariable( variable );
	}
	
	// update gui
	
	// project
	value = mValues[ "TEMPLATE" ];
	if ( value == "app" )
	{
		rbApplication->setChecked( true );
	}
	else if ( value == "subdirs" )
	{
		rbSubdirs->setChecked( true );
	}
	else if ( value == "lib" )
	{
		values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
		if ( values.contains( "designer" ) )
		{
			rbQtDesignerPlugin->setChecked( true );
		}
		else if ( values.contains( "plugin" ) )
		{
			rbQtPlugin->setChecked( true );
		}
		else if ( values.contains( "shared" ) || values.contains( "dll" ) )
		{
			rbSharedLib->setChecked( true );
		}
		else if ( values.contains( "static" ) || values.contains( "staticlib" ) )
		{
			rbStaticLib->setChecked( true );
		}
	}
	
	if ( !rbSubdirs->isChecked() )
	{
		leProjectName->setText( mValues[ "TARGET" ] );
		if ( rbApplication->isChecked() || rbStaticLib->isChecked() )
		{
			leProjectTarget->setText( mValues[ "DESTDIR" ] );
		}
		else
		{
			leProjectTarget->setText( mValues[ "DLLDESTDIR" ] );
		}
	}
	
	// modules
	values = project->splitMultiLineValue( mValues[ "QT" ] );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		if ( values.contains( item->data( Qt::UserRole ).toString() ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		if ( values.contains( item->data( Qt::UserRole ).toString() ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	// compiler settings
	values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
	if ( values.contains( "debug_and_release" ) )
	{
		rbDebugRelease->setChecked( true );
		if ( values.contains( "build_all" ) )
		{
			cbBuildAll->setChecked( true );
		}
	}
	else if ( values.contains( "debug" ) )
	{
		rbDebug->setChecked( true );
	}
	else if ( values.contains( "release" ) )
	{
		rbRelease->setChecked( true );
	}
	
	if ( values.contains( "exceptions" ) )
	{
		cbExceptions->setChecked( true );
	}
	
	if ( values.contains( "rtti" ) )
	{
		cbRtti->setChecked( true );
	}
	
	if ( values.contains( "stl" ) )
	{
		cbStl->setChecked( true );
	}
	
	if ( values.contains( "thread" ) )
	{
		cbThread->setChecked( true );
	}
	
	if ( values.contains( "app_bundle" ) || values.contains( "lib_bundle" ) )
	{
		cbBundle->setChecked( true );
	}
	
	if ( values.contains( "ppc" ) )
	{
		cbPpc->setChecked( true );
	}
	
	if ( values.contains( "x86" ) )
	{
		cbX86->setChecked( true );
	}
	
	if ( values.contains( "x11" ) )
	{
		cbX11->setChecked( true );
	}
	
	if ( values.contains( "warn_on" ) )
	{
		rbWarningOn->setChecked( true );
	}
	
	if ( values.contains( "warn_off" ) )
	{
		rbWarningOff->setChecked( true );
	}
	
	if ( values.contains( "console" ) )
	{
		cbConsole->setChecked( true );
	}
	
	if ( values.contains( "embed_manifest_dll" ) || values.contains( "embed_manifest_exe" ) )
	{
		cbManifest->setChecked( true );
	}
	
	if ( values.contains( "windows" ) )
	{
		cbWindows->setChecked( true );
	}
	
	updateProjectFiles();
}

void UISimpleQMakeEditor::projectTypeChanged()
{
	bool isSubdirs = rbSubdirs->isChecked();
	leProjectName->setDisabled( isSubdirs );
	leProjectTarget->setDisabled( isSubdirs );
	tbProjectTarget->setDisabled( isSubdirs );
}

void UISimpleQMakeEditor::on_tbProjectTarget_clicked()
{
	QString path = QFileDialog::getExistingDirectory( this, tr( "Choose a target path for your project" ), mProject->filePath( leProjectTarget->text() ) );
	if ( !path.isEmpty() )
	{
		leProjectTarget->setText( mProject->relativeFilePath( path ) );
	}
}

void UISimpleQMakeEditor::on_tbAddFile_clicked()
{
	const QString filters = mProject->projectInfos()->variableSuffixesFilter( mProject->projectType() );
	QStringList files = QFileDialog::getOpenFileNames( this, tr( "Choose the file(s) to add to your project" ), mProject->path(), filters );
	
	if ( !files.isEmpty() )
	{
		foreach ( QString fn, files )
		{
			fn = mProject->relativeFilePath( fn );
			
			if ( fn.contains( " " ) )
			{
				fn.prepend( '"' ).append( '"' );
			}
			
			QString variable = mProject->projectInfos()->variableNameForFileName( mProject->projectType(), fn );
			
			if ( !mValues[ variable ].contains( fn ) )
			{
				mValues[ variable ] += " " +fn;
			}
		}
		
		updateProjectFiles();
	}
}

void UISimpleQMakeEditor::on_tbRemoveFile_clicked()
{
	QList<QTreeWidgetItem*> selectedItems = twFiles->selectedItems();
	
	foreach ( QTreeWidgetItem* item, selectedItems )
	{
		if ( item->type() == QTreeWidgetItem::UserType +1 )
		{
			continue;
		}
		
		const QString variable = mProjectFilesItems.key( item->parent() );
		const QString fn = item->data( 0, Qt::UserRole ).toString();
		
		mValues[ variable ].remove( fn );
		delete item;
	}
	
	if ( !selectedItems.isEmpty() )
	{
		updateProjectFiles();
	}
}

void UISimpleQMakeEditor::accept()
{
	QString tmplate;
	QStringList config;
	QStringList qt;
	QStringList target;
	QString destdir;
	QString dlldestdir;
	
	mValues.clear();
	
	// project
	if ( rbApplication->isChecked() )
	{
		tmplate = "app";
	}
	else if ( rbSubdirs->isChecked() )
	{
		tmplate = "subdirs";
	}
	else
	{
		tmplate = "lib";
		if ( rbQtDesignerPlugin->isChecked() )
		{
			config << "designer";
		}
		else if ( rbQtPlugin->isChecked() )
		{
			config << "plugin";
		}
		else if ( rbSharedLib->isChecked() )
		{
			config << "dll";
		}
		else if ( rbStaticLib->isChecked() )
		{
			config << "staticlib";
		}
	}
	
	if ( !rbSubdirs->isChecked() )
	{
		target << leProjectName->text();
		if ( rbApplication->isChecked() || rbStaticLib->isChecked() )
		{
			destdir = leProjectTarget->text();
		}
		else
		{
			dlldestdir = leProjectTarget->text();
		}
	}
	
	// modules
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			qt << item->data( Qt::UserRole ).toString();
		}
	}
	
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			config << item->data( Qt::UserRole ).toString();
		}
	}
	
	// compiler settings
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->isChecked() || !ab->isEnabled() )
		{
			continue;
		}
		
		if ( !ab->statusTip().isEmpty() )
		{
			config << ab->statusTip();
		}
		else
		{
			if ( ab == cbManifest )
			{
				if ( tmplate == "app" )
				{
					config << "embed_manifest_exe";
				}
				else if ( tmplate == "lib" )
				{
					config << "embed_manifest_dll";
				}
			}
			else if ( ab == cbBundle )
			{
				if ( tmplate == "app" )
				{
					config << "app_bundle";
				}
				else if ( tmplate == "lib" )
				{
					config << "lib_bundle";
				}
			}
		}
	}
	
	mValues[ "TEMPLATE" ] = tmplate;
	mValues[ "CONFIG" ] = config.join( " " );
	mValues[ "QT" ] = qt.join( " " );
	mValues[ "TARGET" ] = target.join( " " );
	mValues[ "DESTDIR" ] = destdir;
	mValues[ "DLLDESTDIR" ] = dlldestdir;
	
	qWarning() << mValues;
}
