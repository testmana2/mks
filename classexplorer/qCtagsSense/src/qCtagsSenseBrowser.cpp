#include "qCtagsSenseBrowser.h"
#include "qCtagsSense.h"
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseMembersModel.h"

#include "FileManager.h"

#include <QDebug>
#include <QTime>

qCtagsSenseBrowser::qCtagsSenseBrowser( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	
	mSense = new qCtagsSense( this );
	mLanguagesModel = new qCtagsSenseLanguagesModel( mSense->sql() );
	mFilesModel = new qCtagsSenseFilesModel( mSense->sql() );
	mMembersModel = new qCtagsSenseMembersModel( mSense->sql() );
	
	cbLanguages->setModel( mLanguagesModel );
	cbFileNames->setModel( mFilesModel );
	
	QTreeView* tv = new QTreeView( cbMembers );
	tv->setHeaderHidden( true );
	cbMembers->setView( tv );
	cbMembers->setModel( mMembersModel );
	
	tvMembers->setHeaderHidden( true );
	tvMembers->setModel( mMembersModel );
	
	mFileManager = new FileManager( this );
	
	connect( mSense, SIGNAL( indexChanged() ), this, SLOT( indexChanged() ) );
	connect( mFileManager, SIGNAL( buffersModified( const QMap<QString, QString>& ) ), mSense, SLOT( tagEntries( const QMap<QString, QString>& ) ) );
	connect( this, SIGNAL( memberActivated( const QString&, const QModelIndex& ) ), mFileManager, SLOT( memberActivated( const QString&, const QModelIndex& ) ) );
	
	// *********************
	
	//mSense->tagEntry( "/usr/include" );
	//mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/v2/branches/dev" );
	mSense->tagEntry( "/usr/include/qt4/Qt" );
	mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/classexplorer/qCtagsSense/src" );
}

qCtagsSenseBrowser::~qCtagsSenseBrowser()
{
	qApp->quit();
}

void qCtagsSenseBrowser::on_cbLanguages_currentIndexChanged( int id )
{
	const QString language = mLanguagesModel->language( id );
	mFilesModel->refresh( language );
	cbFileNames->setCurrentIndex( 0 );
}

void qCtagsSenseBrowser::on_cbFileNames_currentIndexChanged( int id )
{
	const QString fileName = mFilesModel->fileName( id );
	mMembersModel->populateFromFile( fileName );
	qobject_cast<QTreeView*>( cbMembers->view() )->expandAll();
	tvMembers->expandAll();
}

void qCtagsSenseBrowser::on_cbMembers_currentIndexChanged( int id )
{
	Q_UNUSED( id );
	const QString fileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	emit memberActivated( fileName, cbMembers->view()->currentIndex() );
}

void qCtagsSenseBrowser::on_tvMembers_activated( const QModelIndex& index )
{
	const QString fileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	emit memberActivated( fileName, index );
}

void qCtagsSenseBrowser::indexChanged()
{
	const QString language = mLanguagesModel->language( cbLanguages->currentIndex() );
	const QString fileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	
	bool languageLocked = cbLanguages->blockSignals( true );
	bool fileLocked = cbFileNames->blockSignals( true );
	bool memberLocked = cbMembers->blockSignals( true );
	
	mLanguagesModel->refresh();
	
	if ( !language.isEmpty() )
	{
		cbLanguages->setCurrentIndex( mLanguagesModel->indexOf( language ) );
		mFilesModel->refresh( language );
		
		if ( !fileName.isEmpty() )
		{
			cbFileNames->setCurrentIndex( mFilesModel->indexOf( fileName ) );
			mMembersModel->populateFromFile( fileName );
		}
	}
	
	qobject_cast<QTreeView*>( cbMembers->view() )->expandAll();
	tvMembers->expandAll();
	
	cbLanguages->blockSignals( languageLocked );
	cbFileNames->blockSignals( fileLocked );
	cbMembers->blockSignals( memberLocked );
}
