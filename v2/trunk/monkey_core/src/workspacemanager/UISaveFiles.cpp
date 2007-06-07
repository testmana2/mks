#include "UISaveFiles.h"
#include "AbstractChild.h"
//
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QFileInfo>
//
UISaveFiles::UISaveFiles( AbstractChild* p, QCloseEvent* e )
	: QDialog( p ), mChild( p ), mEvent( e )
{
	setWindowTitle( tr( "Save File(s) ?[*]" ) );
	resize( 400, 300 );
	//
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 5 );
	vl->setSpacing( 3 );
	//
	QLabel* l = new QLabel;
	l->setText( tr( "Check the files you want to save :" ) );
	vl->addWidget( l );
	//
	lwFiles = new QListWidget;
	vl->addWidget( lwFiles );
	//
	dbbButtons = new QDialogButtonBox;
	dbbButtons->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Close | QDialogButtonBox::Cancel );
	vl->addWidget( dbbButtons );
	//
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( clicked( QAbstractButton* ) ) );
	//
	foreach ( QString s, mChild->files() )
	{
		addFile( s, mChild->isModified( s ) );
		if ( !isWindowModified() && mChild->isModified( s ) )
			setWindowModified( true );
	}
}
//
void UISaveFiles::addFile( const QString& s, bool b )
{
	QListWidgetItem* i = new QListWidgetItem( QFileInfo( s ).fileName(), lwFiles );
	i->setData( Qt::UserRole, s );
	i->setCheckState( Qt::Unchecked );
	if ( b )
		i->setCheckState( Qt::Checked );
}
//
void UISaveFiles::clicked( QAbstractButton* ab )
{
	QPushButton* pb = qobject_cast<QPushButton*>( ab );
	if ( pb  == dbbButtons->button( QDialogButtonBox::Save ) )
	{
		for ( int i = 0; i < lwFiles->count(); i++ )
			if ( lwFiles->item( i )->checkState() != Qt::Unchecked )
				mChild->saveFile( lwFiles->item( i )->data( Qt::UserRole ).toString() );
	}
	else if ( pb == dbbButtons->button( QDialogButtonBox::Cancel ) )
		mEvent->ignore();
	close();
}
//
void UISaveFiles::execute( AbstractChild* c, QCloseEvent* e )
{
	if ( !c || !e || !c->files().count() )
		return;
	UISaveFiles d( c, e );
	if ( d.isWindowModified() )
		d.exec();
}
