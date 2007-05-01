#include "UIProjectsViewer.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectProxy.h"
#include "UIQMakeProjectSettings.h"
//
QPointer<UIProjectsViewer> UIProjectsViewer::mSelf = 0L;
//
UIProjectsViewer* UIProjectsViewer::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIProjectsViewer( p );
	return mSelf;
}
//
UIProjectsViewer::UIProjectsViewer( QWidget* p )
	: QWidget( p ), mModel( 0 ), mProxy( 0 )
{
	setupUi( this );
}
//
bool UIProjectsViewer::openProject( const QString& s )
{
	delete mProxy;
	delete mModel;
	//
	mModel = new QMakeProjectModel( s, this );
	if ( mModel->isOpen() )
	{
		mProxy = new QMakeProjectProxy( mModel, this );
		tvProject->setModel( mProxy );
		//
		/*
		connect( cbFilter, SIGNAL( stateChanged( int ) ), mProxy, SLOT( setFilter( int ) ) );
		connect( tvProject, SIGNAL( clicked( const QModelIndex& ) ), mProxy, SLOT( currentProjectName( const QModelIndex& ) ) );
		connect( mProxy, SIGNAL( sig_curPro( const QString& ) ), lProject, SLOT( setText( const QString& ) ) );
		connect( pbWrite, SIGNAL( clicked() ), mModel, SLOT( saveProject() ) );
		connect( mModel, SIGNAL( sig_writeText( const QString& ) ), teOutput, SLOT( setPlainText( const QString& ) ) );
		*/
		//
	}
	return mModel->isOpen();
}
//
void UIProjectsViewer::on_pbSettings_clicked()
{
	if ( mModel->isOpen() )
	{
		//UIQMakeProjectSettings::execute( mModel->project( tvProject->currentIndex() ), this );
		UIQMakeProjectSettings::execute( mModel->project( mProxy->mapToSource( tvProject->currentIndex() ) ), this );
	}
}
