#include "qCtagsSenseBrowser.h"
#include "qCtagsSense.h"
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseMembersModel.h"

#include "FileManager.h"

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
	
	connect( mSense, SIGNAL( indexChanged() ), this, SLOT( mSense_indexChanged() ) );
	connect( mFileManager, SIGNAL( buffersModified( const QMap<QString, QString>& ) ), mSense, SLOT( tagEntries( const QMap<QString, QString>& ) ) );
	connect( this, SIGNAL( memberActivated( const QString&, const QModelIndex& ) ), mFileManager, SLOT( memberActivated( const QString&, const QModelIndex& ) ) );
	connect( mLanguagesModel, SIGNAL( ready() ), this, SLOT( mLanguagesModel_ready() ) );
	connect( mFilesModel, SIGNAL( ready() ), this, SLOT( mFilesModel_ready() ) );
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
	
	// *********************
#if defined Q_OS_MAC
	mSense->tagEntry( "/Developer/SDKs/MacOSX10.4u.sdk/usr/include" );
	mSense->tagEntry( "/Users/filipeazevedo/Documents/Development/Qt4/mks/classexplorer/qCtagsSense/src" );
#elif defined Q_OS_WIN
#else
	//mSense->tagEntry( "/usr/include" );
	//mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/v2/branches/dev" );
	mSense->tagEntry( "/usr/include/qt4/Qt" );
	mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/classexplorer/qCtagsSense/src" );
#endif
}

qCtagsSenseBrowser::~qCtagsSenseBrowser()
{
	qApp->quit();
}

void qCtagsSenseBrowser::on_cbLanguages_currentIndexChanged( int id )
{
	mLanguage = mLanguagesModel->language( id );
	mFileName.clear();
	mFilesModel->refresh( mLanguage );
}

void qCtagsSenseBrowser::on_cbFileNames_currentIndexChanged( int id )
{
	mFileName = mFilesModel->fileName( id );
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::on_cbMembers_currentIndexChanged( int id )
{
	Q_UNUSED( id );
	emit memberActivated( mFileName, cbMembers->view()->currentIndex() );
}

void qCtagsSenseBrowser::on_tvMembers_activated( const QModelIndex& index )
{
	emit memberActivated( mFileName, index );
}

void qCtagsSenseBrowser::mSense_indexChanged()
{
	mLanguage = mLanguagesModel->language( cbLanguages->currentIndex() );
	mFileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	mLanguagesModel->refresh();
}

void qCtagsSenseBrowser::mLanguagesModel_ready()
{
	bool languageLocked = cbLanguages->blockSignals( true );
	int id = mLanguagesModel->indexOf( mLanguage );
	
	if ( id == -1 && !cbLanguages->count() == 0 )
	{
		id = 0;
	}
	
	mLanguage = mLanguagesModel->language( id );
	cbLanguages->setCurrentIndex( id );
	mFilesModel->refresh( mLanguage );
	cbLanguages->blockSignals( languageLocked );
}

void qCtagsSenseBrowser::mFilesModel_ready()
{	
	bool fileLocked = cbFileNames->blockSignals( true );
	int id = mFilesModel->indexOf( mFileName );
	
	if ( id == -1 && !cbFileNames->count() == 0 )
	{
		id = 0;
	}
	
	mFileName = mFilesModel->fileName( id );
	cbFileNames->setCurrentIndex( id );
	mMembersModel->refresh( mFileName );
	cbFileNames->blockSignals( fileLocked );
}

void qCtagsSenseBrowser::mMembersModel_ready()
{
	qobject_cast<QTreeView*>( cbMembers->view() )->expandAll();
	tvMembers->expandAll();
}
