#include "UIQMakeProjectSettings.h"
#include "QMakeProjectScopesProxy.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "UIItemSettings.h"
//
UIQMakeProjectSettings::UIQMakeProjectSettings( QMakeProjectModel* m, QWidget* p )
	: QDialog( p ), mProxy( 0 ), mProject( m )
{
	setupUi( this );
	mProxy = new QMakeProjectScopesProxy( mProject );
	tvScopes->setModel( mProxy );
	lvContents->setModel( mProject );
	setCurrentIndex( mProject->index( 0, 0 ) );
}
//
UIQMakeProjectSettings::~UIQMakeProjectSettings()
{
	delete mProxy;
}
//
void UIQMakeProjectSettings::execute( QMakeProjectModel* m, QWidget* p )
{
	if ( !m )
		return;
	UIQMakeProjectSettings d( m, p );
	d.exec();
}
//
QModelIndex UIQMakeProjectSettings::currentIndex()
{
	QModelIndex i;
	if ( lvContents->selectionModel()->selectedIndexes().count() )
		i = lvContents->selectionModel()->selectedIndexes().at( 0 );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
	{
		i = tvScopes->selectionModel()->selectedIndexes().at( 0 );
		if ( i.isValid() )
			i = mProxy->mapToSource( i );
	}
	return i;
}
//
void UIQMakeProjectSettings::setCurrentIndex( const QModelIndex& i )
{
	// clear selection
	tvScopes->clearSelection();
	lvContents->clearSelection();
	// set/select current index
	if ( i.data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::ValueType )
	{
		tvScopes->setCurrentIndex( mProxy->mapFromSource( i.parent() ) );
		lvContents->setRootIndex( i.parent() );
		lvContents->setCurrentIndex( i );
	}
	else
	{
		tvScopes->setCurrentIndex( mProxy->mapFromSource( i ) );
		lvContents->setRootIndex( i );
	}
}
//
void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{
	// clear selection
	lvContents->clearSelection();
	// set root index to show
	lvContents->setRootIndex( mProxy->mapToSource( i ) );
}
//
void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	UIItemSettings::edit( mProject, 0, this )->exec();
}
//
void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mProject, static_cast<QMakeProjectItem*>( mProject->itemFromIndex( i ) ), this )->exec();
}
//
void UIQMakeProjectSettings::on_tbRemove_clicked()
{
	QModelIndex i = currentIndex();
	//if ( i.isValid() )
		//mProject->removeRow( i.row(), i.parent() );
}
//
void UIQMakeProjectSettings::on_tbClear_clicked()
{
	QModelIndex i = currentIndex();
	//if ( i.isValid() )
		//while ( mProject->rowCount( i.parent() ) )
			//mProject->removeRow( 0, i.parent() );
}
//
void UIQMakeProjectSettings::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->itemFromIndex( i )->moveUp() )
			setCurrentIndex( i.sibling( i.row() -1, i.column() ) );
}
//
void UIQMakeProjectSettings::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mProject->itemFromIndex( i )->moveDown() )
			setCurrentIndex( i.sibling( i.row() +1, i.column() ) );
}
//
void UIQMakeProjectSettings::accept()
{
	QDialog::accept();
}
//
void UIQMakeProjectSettings::reject()
{
	QDialog::reject();
}
