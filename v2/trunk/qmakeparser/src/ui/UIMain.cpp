#include "UIMain.h"
#include "AbstractProjectProxy.h"
#include "QMakeProjectItemModel.h"
//
#include <QFileDialog>
//
QPointer<UIMain> UIMain::_self = 0L;
//
UIMain* UIMain::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIMain( parent );
	return _self;
}
//
UIMain::UIMain( QWidget* parent )
	: QWidget( parent ), mProxy( new AbstractProjectProxy( new QMakeProjectItemModel( QString(), this ) ) )
{
	setupUi( this );
	cbComplexModel->setChecked( mProxy->isComplexModel() );
	connect( cbComplexModel, SIGNAL( toggled( bool ) ), mProxy, SLOT( setComplexModel( bool ) ) );
	listView->setModel( mProxy );
	treeView->setModel( mProxy );
	tableView->setModel( mProxy );
}
//
void UIMain::on_pbOpen_clicked()
{
	QString mFilePath = QFileDialog::getOpenFileName( this, tr( "Open Qt Project" ), QDir::homePath().append( "/Desktop" ), tr( "Qt Projects (*.pro)" ) );
	if ( mFilePath.isNull() )
		return;
	on_pbClose_clicked();
	mProxy->project()->setFilePath( mFilePath );
	mProxy->project()->openProject();
}
//
void UIMain::on_pbClose_clicked()
{
	if ( mProxy->project()->isOpen() )
		mProxy->project()->closeProject();
}
