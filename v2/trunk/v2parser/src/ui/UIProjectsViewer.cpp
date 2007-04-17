#include "UIProjectsViewer.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectProxyModel.h"
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
#ifndef QT_NO_DEBUG
#include "modeltest.h"
#endif
bool UIProjectsViewer::openProject( const QString& s )
{
	delete mProxy;
	delete mModel;
	//
	mModel = new QMakeProjectModel( s, this );
	if ( mModel->isOpen() )
	{
#ifndef QT_NO_DEBUG
		new ModelTest( mModel, this );
#endif
		mProxy = new QMakeProjectProxyModel( mModel );
		tvProject->setModel( mProxy );
		tvProject->setRootIndex( mProxy->index( 1, 0 ) );
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
	UIQMakeProjectSettings::execute( mModel, this );
}
