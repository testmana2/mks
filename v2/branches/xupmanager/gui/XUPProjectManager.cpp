#include "XUPProjectManager.h"
#include "UIXUPFindFiles.h"
#include "../iconmanager/pIconManager.h"
#include "../core/XUPProjectModel.h"
#include "../core/XUPFilteredProjectModel.h"
#include "../core/XUPProjectItem.h"

#include <QFileDialog>
#include <QTextCodec>

#include <QDebug>
#include <QMenu>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	tbOpen->setDefaultAction( action( atOpen ) );
	tbClose->setDefaultAction( action( atClose ) );
	
	mFilteredModel = new XUPFilteredProjectModel( tvFiltered );
	tvFiltered->setModel( mFilteredModel );
	
	mDebugMenu = new QMenu( tbDebug );
	tbDebug->setMenu( mDebugMenu );
	tbDebug->setPopupMode( QToolButton::InstantPopup );
	
	mDebugMenu->addAction( "interpretValue" );
	mDebugMenu->addAction( "interpretVariable" );
	mDebugMenu->addAction( "project" );
	mDebugMenu->addAction( "topLevelProject" );
	mDebugMenu->addAction( "rootIncludeProject" );
}

XUPProjectManager::~XUPProjectManager()
{
	delete XUPProjectItem::projectInfos();
}

void XUPProjectManager::fileClosed( QObject* object )
{
	QWidget* widget = qobject_cast<QWidget*>( object ); // fuck bug on casting to QPlainTextEdit that return 0
	if ( widget )
		mOpenedFiles.remove( widget->windowFilePath() );
}

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* project = cbProjects->itemData( id ).value<XUPProjectModel*>();
	setCurrentProject( project );
}

void XUPProjectManager::on_tbDebug_triggered( QAction* action )
{
	const QModelIndex index = tvNative->currentIndex();
	if ( !index.isValid() )
		return;
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
	
	pteLog->appendPlainText( "------------------" );
	
	QString attribute;
	
	switch ( item->type() )
	{
		case XUPItem::Project:
			attribute = "name";
			break;
		case XUPItem::Comment:
			attribute = "value";
			break;
		case XUPItem::EmptyLine:
			attribute = "count";
			break;
		case XUPItem::Variable:
			attribute = "name";
			break;
		case XUPItem::Value:
			attribute = "content";
			break;
		case XUPItem::Function:
			attribute = "parameters";
			break;
		case XUPItem::Scope:
			attribute = "name";
			break;
		case XUPItem::DynamicFolder:
			attribute = "name";
			break;
		case XUPItem::Folder:
			attribute = "name";
			break;
		case XUPItem::File:
			attribute = "content";
			break;
		case XUPItem::Path:
			attribute = "content";
			break;
		case XUPItem::Unknow:
			break;
	}
	
	if ( action->text() == "interpretValue" )
	{
		if ( item->type() == XUPItem::Value )
		{
			pteLog->appendPlainText( item->attribute( attribute ).prepend( "Interpret value '" ).append( "'" ) );
			pteLog->appendPlainText( item->project()->rootIncludeProject()->interpretValue( item, attribute ) );
		}
	}
	else if ( action->text() == "interpretVariable" )
	{
		if ( item->type() == XUPItem::Variable )
		{
			pteLog->appendPlainText( item->attribute( attribute ).prepend( "Interpret variable '" ).append( "'" ) );
			pteLog->appendPlainText( item->project()->rootIncludeProject()->interpretVariable( item->attribute( attribute ), item, "#Null" ) );
		}
	}
	else if ( action->text() == "project" )
	{
		pteLog->appendPlainText( item->project()->displayText().prepend( "Project: " ) );
	}
	else if ( action->text() == "topLevelProject" )
	{
		pteLog->appendPlainText( item->project()->topLevelProject()->displayText().prepend( "Top level project: " ) );
	}
	else if ( action->text() == "rootIncludeProject" )
	{
		pteLog->appendPlainText( item->project()->rootIncludeProject()->displayText().prepend( "Root include project: " ) );
	}
}

void XUPProjectManager::on_tvFiltered_activated( const QModelIndex& index )
{
	XUPItem* item = mFilteredModel->mapToSource( index );
	if ( item )
	{
		if ( item->type() == XUPItem::File )
		{
			XUPProjectItem* pItem = item->project()->rootIncludeProject();
			QString fn = pItem->filePath( pItem->interpretValue( item, "content" ) );
			
			if ( !QFile::exists( fn ) )
			{
				QString findFile = item->attribute( "content" ).remove( '"' );
				QFileInfoList files = pItem->findFile( findFile );
				switch ( files.count() )
				{
					case 0:
						fn.clear();
						break;
					case 1:
						fn = files.at( 0 ).absoluteFilePath();
						break;
					default:
					{
						UIXUPFindFiles dlg( findFile, this );
						dlg.setFiles( files, pItem->path() );
						fn.clear();
						if ( dlg.exec() )
						{
							fn = dlg.selectedFile();
						}
						break;
					}
				}
			}
			
			if ( QFile::exists( fn ) )
			{
				openFile( fn );
			}
		}
	}
}

void XUPProjectManager::on_tvNative_activated( const QModelIndex& index )
{
	QModelIndex proxyIndex = mFilteredModel->mapFromSource( static_cast<XUPItem*>( index.internalPointer() ) );
	on_tvFiltered_activated( proxyIndex );
}

QAction* XUPProjectManager::action( XUPProjectManager::ActionType type )
{
	if ( mActions.contains( type ) )
	{
		return mActions[ type ];
	}
	
	QAction* action = 0;
	switch ( type )
	{
		case XUPProjectManager::atOpen:
			action = new QAction( pIconManager::icon( "open.png" ), tr( "Open" ), this );
			action->setToolTip( tr( "Open a project" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( openProject() ) );
			break;
		case XUPProjectManager::atClose:
			action = new QAction( pIconManager::icon( "close.png" ), tr( "Close" ), this );
			action->setToolTip( tr( "Close the current project" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeProject() ) );
			break;
	}
	
	if ( action )
	{
		mActions[ type ] = action;
	}
	
	return action;
}

void XUPProjectManager::addError( const QString& error )
{
	pteLog->appendPlainText( error );
}

bool XUPProjectManager::openFile( const QString& fileName, const QString& encoding )
{
	// check already open file
	QPlainTextEdit* pte = mOpenedFiles.value( fileName );
	if ( pte )
	{
		if ( pte->isMinimized() )
			pte->showNormal();
		pte->activateWindow();
		return true;
	}
	
	// get QFile
	QFile file( fileName );
	
	// check existence
	if ( !file.exists() )
	{
		addError( tr( "file not exists: %1" ).arg( fileName ) );
		return false;
	}
	
	// check is file
	if ( !QFileInfo( fileName ).isFile() )
	{
		addError( tr( "file is not a file: %1" ).arg( fileName ) );
		return false;
	}
	
	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		addError( tr( "Can't open file for reading: %1" ).arg( fileName ) );
		return false;
	}
	
	// decode content
	QTextCodec* codec = QTextCodec::codecForName( encoding.toUtf8() );
	QString buffer = codec->toUnicode( file.readAll() );
	
	pte = new QPlainTextEdit( this );
	pte->setWindowFlags( Qt::Window );
	pte->setAttribute( Qt::WA_DeleteOnClose );
	pte->setWindowFilePath( fileName );
	pte->setPlainText( buffer );
	connect( pte, SIGNAL( modificationChanged( bool )), pte, SLOT( setWindowModified( bool ) ) );
	connect( pte, SIGNAL( destroyed( QObject* )), this, SLOT( fileClosed( QObject* ) ) );
	
	mOpenedFiles[ fileName ] = pte;
	
	pte->show();
	
	return true;
}

void XUPProjectManager::openProject()
{
	const QString fn = QFileDialog::getOpenFileName( this, tr( "Choose a project to open" ), QLatin1String( "." ), XUPProjectItem::projectInfos()->projectsFilter() );
	if ( !fn.isNull() )
	{
		XUPProjectModel* project = new XUPProjectModel( this );
		if ( project->open( fn ) )
		{
			int id = cbProjects->count();
			cbProjects->addItem( project->headerData( 0, Qt::Horizontal, Qt::DisplayRole ).toString(), QVariant::fromValue<XUPProjectModel*>( project ) );
			cbProjects->setItemIcon( id, project->headerData( 0, Qt::Horizontal, Qt::DecorationRole ).value<QIcon>() );
			cbProjects->setCurrentIndex( id );
		}
		else
		{
			pteLog->appendPlainText( project->lastError() );
			delete project;
		}
	}
}

void XUPProjectManager::XUPProjectManager::closeProject()
{
	XUPProjectModel* project = currentProject();
	if ( project )
	{
		if ( !project->save() )
		{
			pteLog->appendPlainText( project->lastError() );
		}
		project->close();
		int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
		cbProjects->removeItem( id );
		delete project;
	}
}

XUPProjectModel* XUPProjectManager::currentProject() const
{
	return qobject_cast<XUPProjectModel*>( tvNative->model() );
}

void XUPProjectManager::setCurrentProject( XUPProjectModel* project )
{
	mFilteredModel->setSourceModel( project );
	tvNative->setModel( project );
	
	int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
	if ( cbProjects->currentIndex() != id )
	{
		cbProjects->setCurrentIndex( id );
	}
}
