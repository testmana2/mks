#include "UIProjectSettingsQMake.h"
#include "QMakeProjectProxy.h"
//
UIProjectSettingsQMake::UIProjectSettingsQMake( QMakeProjectProxy* m, QWidget* p )
	: QDialog( p ), mProxy( m )
{
	setupUi( this );
	tvScopes->setModel( m->project() );
	lvContents->setModel( m->project() );
}
//
void UIProjectSettingsQMake::execute( QMakeProjectProxy* m, QWidget* p )
{
	if ( !m )
		return;	
	UIProjectSettingsQMake d( m, p );
	d.exec();
}
//
void UIProjectSettingsQMake::on_tvScopes_clicked( const QModelIndex& i )
{
	//lvContents->setRootIndex( mProxy->mapToSource( i ) );
	lvContents->setRootIndex( i );
}
//
void UIProjectSettingsQMake::accept()
{
}
//
void UIProjectSettingsQMake::reject()
{
}
