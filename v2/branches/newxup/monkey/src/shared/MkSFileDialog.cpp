#include "MkSFileDialog.h"
#include "XUPProjectItem.h"
#include "ProjectTypesIndex.h"
#include "pMonkeyStudio.h"
#include "pTreeComboBox.h"
#include "XUPAddFiles.h"
#include "MonkeyCore.h"
#include "XUPProjectManager.h"
#include "Settings.h"

#include <QComboBox>
#include <QDebug>

MkSFileDialog::MkSFileDialog( QWidget* parent, const QString& caption, const QString& directory, const QString& filter, bool textCodecEnabled )
	: pFileDialog( parent, caption, directory, filter, textCodecEnabled, false )
{
	mAddFiles = new XUPAddFiles( this );
	glDialog->addWidget( mAddFiles, 6, 0, 1, -1 );
	
	connect( mAddFiles, SIGNAL( currentScopeChanged( XUPItem* ) ), this, SLOT( currentScopeChanged( XUPItem* ) ) );
}

void MkSFileDialog::setRecentNameFilter( const QString& name )
{
	if ( !name.isEmpty() ) {
		selectNameFilter( name );
	}
}

void MkSFileDialog::currentScopeChanged( XUPItem* scope )
{
	if ( scope ) {
		QString projectPath = QDir( scope->project()->path() ).canonicalPath();
		
		if ( !directory().canonicalPath().startsWith( projectPath ) ) {
			setDirectory( projectPath );
		}
	}
}

pFileDialogResult MkSFileDialog::getOpenFileName( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
	pFileDialogResult result;
	MkSFileDialog fd( parent );
	setOpenFileNameDialog( &fd, caption, dir, filter, enabledTextCodec, false, selectedFilter, options );
	fd.setTextCodec( pMonkeyStudio::defaultCodec() );
	fd.mAddFiles->setVisible( false );
	
	if ( useRecents ) {
		if ( !filter.isEmpty() ) {
			fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
		}
	}
	
	if ( fd.exec() == QDialog::Accepted ) {
		if ( useRecents ) {
			if ( !filter.isEmpty() ) {
				MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
			}
		}
		
		if ( selectedFilter ) {
			*selectedFilter = fd.selectedNameFilter();
		}
		
		result[ "filename" ] = fd.selectedFiles().value( 0 );
		result[ "codec" ] = fd.textCodec();
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getOpenFileNames( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
	pFileDialogResult result;
	MkSFileDialog fd( parent );
	setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, false, selectedFilter, options );
	fd.setTextCodec( pMonkeyStudio::defaultCodec() );
	fd.mAddFiles->setVisible( false );
	
	if ( useRecents ) {
		if ( !filter.isEmpty() ) {
			fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
		}
	}
	
	if ( fd.exec() == QDialog::Accepted ) {
		if ( useRecents ) {
			if ( !filter.isEmpty() ) {
				MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
			}
		}
		
		if ( selectedFilter ) {
			*selectedFilter = fd.selectedNameFilter();
		}
		
		result[ "filenames" ] = fd.selectedFiles();
		result[ "codec" ] = fd.textCodec();
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getSaveFileName( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
	pFileDialogResult result;
	MkSFileDialog fd( parent );
	setSaveFileNameDialog( &fd, caption, dir, filter, enabledTextCodec, selectedFilter, options );
	fd.setTextCodec( pMonkeyStudio::defaultCodec() );
	fd.mAddFiles->setVisible( false );
	
	if ( useRecents ) {
		if ( !filter.isEmpty() ) {
			fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
		}
	}
	
	if ( fd.exec() == QDialog::Accepted ) {
		if ( useRecents ) {
			if ( !filter.isEmpty() ) {
				MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
			}
		}
		
		if ( selectedFilter ) {
			*selectedFilter = fd.selectedNameFilter();
		}
		
		result[ "filename" ] = fd.selectedFiles().value( 0 );
		result[ "codec" ] = fd.textCodec();
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getExistingDirectory( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, bool enabledTextCodec, Options options )
{
	pFileDialogResult result;
	MkSFileDialog fd( parent );
	setOpenFileNameDialog( &fd, caption, dir, QString::null, enabledTextCodec, false, 0, options );
	fd.setFileMode( QFileDialog::Directory );
	fd.setTextCodec( pMonkeyStudio::defaultCodec() );
	fd.mAddFiles->setVisible( false );
	
	#warning Does we need recents handling here ?
	Q_UNUSED( useRecents );
	
	/*if ( useRecents ) {
		if ( !filter.isEmpty() ) {
			fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
		}
	}*/
	
	if ( fd.exec() == QDialog::Accepted ) {
		/*if ( useRecents ) {
			if ( !filter.isEmpty() ) {
				MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
			}
		}*/
		
		result[ "filename" ] = fd.selectedFiles().value( 0 );
		result[ "codec" ] = fd.textCodec();
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getProjectAddFiles( QWidget* parent, bool allowChooseScope )
{
	pFileDialogResult result;
	XUPProjectModel* model = MonkeyCore::projectsManager()->currentProjectModel();
	
	if ( model ) {
		XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
		QString caption = tr( "Choose file(s) to add to your project" );
		QString dir = pMonkeyStudio::defaultProjectsDirectory();
		QString filter = curProject->documentFilters().sourceFileNameFilter();
		bool enabledTextCodec = false;
		
		MkSFileDialog fd( parent );
		setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, false, 0, 0 );
		fd.setTextCodec( pMonkeyStudio::defaultCodec() );
		fd.mAddFiles->setModel( model );
		fd.mAddFiles->setCurrentScope( curProject );
		fd.mAddFiles->setScopeChoiceEnabled( allowChooseScope );
		
		if ( fd.exec() == QDialog::Accepted ) {
			result[ "filenames" ] = fd.selectedFiles();
			result[ "scope" ] = QVariant::fromValue<XUPItem*>( fd.mAddFiles->currentScope() );
			result[ "import" ] = fd.mAddFiles->importExternalFiles();
			result[ "importpath" ] = fd.mAddFiles->importExternalFilesPath();
			result[ "directory" ] = fd.directory().absolutePath();
		}
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getNewEditorFile( QWidget* parent )
{
	pFileDialogResult result;
	XUPProjectModel* model = MonkeyCore::projectsManager()->currentProjectModel();
	XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
	QString caption = tr( "New File Name..." );
	QString dir = pMonkeyStudio::defaultProjectsDirectory();
	QString filter = curProject ? curProject->documentFilters().sourceFileNameFilter() : pMonkeyStudio::availableFilesFilter();
	bool enabledTextCodec = true;
	
	MkSFileDialog fd( parent );
	setSaveFileNameDialog( &fd, caption, dir, filter, enabledTextCodec, 0, 0 );
	fd.setTextCodec( pMonkeyStudio::defaultCodec() );
	
	if ( curProject ) {
		fd.mAddFiles->setModel( model );
		fd.mAddFiles->setAddToProjectChoice( true );
		fd.mAddFiles->setAddToProject( false );
		fd.mAddFiles->setCurrentScope( curProject );
	}
	else {
		fd.mAddFiles->setVisible( false );
	}
	
	if ( fd.exec() == QDialog::Accepted ) {
		result[ "filename" ] = fd.selectedFiles().value( 0 );
		result[ "codec" ] = fd.textCodec();
		
		if ( model ) {
			result[ "addtoproject" ] = fd.mAddFiles->addToProject();
			result[ "scope" ] = QVariant::fromValue<XUPItem*>( fd.mAddFiles->currentScope() );
		}
	}
	
	return result;
}
