#include "UIProjectsViewer.h"
#include "ProjectItemModel.h"
#include "ProjectProxyModel.h"
#include "UIProjectSettingsProxyModel.h"
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
	//
	//openProject( QApplication::applicationDirPath().append( "/../example/monkey.pro" ) );
	openProject( "/home/pasnox/Desktop/pparser/parser.pro" );
}
//
bool UIProjectsViewer::openProject( const QString& s )
{
	delete mProxy;
	delete mModel;
	//
	mModel = new ProjectItemModel();
	mProxy = new ProjectProxyModel( mModel );
	//
	QString f( s );
	mModel->openFile( f );
	//
	connect( cbFilter, SIGNAL( stateChanged( int ) ), mProxy, SLOT( setFilter( int ) ) );
	connect( tvProject, SIGNAL( clicked( const QModelIndex& ) ), mProxy, SLOT( currentProjectName( const QModelIndex& ) ) );
	connect( mProxy, SIGNAL( sig_curPro( const QString& ) ), lProject, SLOT( setText( const QString& ) ) );
	connect( pbWrite, SIGNAL( clicked() ), mModel, SLOT( saveProject() ) );
	connect( mModel, SIGNAL( sig_writeText( const QString& ) ), teOutput, SLOT( setPlainText( const QString& ) ) );
	//
	tvProject->setModel( mProxy );
}
//
void UIProjectsViewer::on_pbSettings_clicked()
{
	UIProjectSettingsProxyModel::execute( mModel, this );
}
