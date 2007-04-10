#include "UIProjectSettingsProxyModel.h"
#include "ProjectSettingsProxyModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"
#include "UIItemSettings.h"
//
UIProjectSettingsProxyModel::UIProjectSettingsProxyModel( ProjectItemModel* m, QWidget* p )
	: QDialog( p ), mProxy( 0 ), mProject( m )
{
	setupUi( this );
	mProxy = new ProjectSettingsProxyModel( mProject );
	tvScopes->setModel( mProxy );
	lvContents->setModel( mProject );
}
//
UIProjectSettingsProxyModel::~UIProjectSettingsProxyModel()
{
	delete mProxy;
}
//
void UIProjectSettingsProxyModel::execute( ProjectItemModel* m, QWidget* p )
{
	if ( !m )
		return;
	UIProjectSettingsProxyModel d( m, p );
	d.exec();
}
//
QModelIndex UIProjectSettingsProxyModel::currentIndex()
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
void UIProjectSettingsProxyModel::on_tvScopes_clicked( const QModelIndex& i )
{
	lvContents->setRootIndex( mProxy->mapToSource( i ) );
	lvContents->clearSelection();
}
//
void UIProjectSettingsProxyModel::on_tbAdd_clicked()
{
	UIItemSettings::edit( mProject, 0, this )->exec();
}
//
void UIProjectSettingsProxyModel::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mProject, dynamic_cast<ProjectItem*>( mProject->itemFromIndex( i ) ), this )->exec();
}
//
void UIProjectSettingsProxyModel::on_tbRemove_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		mProject->removeRow( i.row(), i.parent() );
}
//
void UIProjectSettingsProxyModel::on_tbClear_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		while ( mProject->rowCount( i.parent() ) )
			mProject->removeRow( 0, i.parent() );
}
//
void UIProjectSettingsProxyModel::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( !i.isValid() || i.row() == 0 )
		return;
	// got index row
	int id = i.row();
	QModelIndex iParent = i.parent();
	// move index
	if ( iParent.isValid() )
	{
		QStandardItem* mItem = mProject->itemFromIndex( i );
		QStandardItem* mParentItem = mProject->itemFromIndex( iParent );
		mParentItem->insertRow( mItem->row() -1, mParentItem->takeRow( mItem->row() ) );
		lvContents->setCurrentIndex( i.parent().child( id -1, 0 ) );
	}
	else
	{
		mProject->insertRow( id -1, mProject->takeRow( id ) );
		tvScopes->setCurrentIndex( mProxy->mapFromSource( mProject->index( id -1, 0 ) ) );
	}
}
//
void UIProjectSettingsProxyModel::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( !i.isValid() || i.row() == mProject->rowCount( i.parent() ) -1 )
		return;
	// got index row
	int id = i.row();
	// index child
	QList<QStandardItem*> l;
	// got index parent item
	QStandardItem* pItem = mProject->itemFromIndex( i.parent() );
	// move index
	if ( pItem )
	{
		l = pItem->takeRow( id );
		pItem->insertRow( id +1, l );
		lvContents->setCurrentIndex( i.parent().child( id +1, 0 ) );
	}
	else
	{
		l = mProject->takeRow( id );
		mProject->insertRow( id +1, l );
		tvScopes->setCurrentIndex( mProxy->mapFromSource( mProject->item( id +1 )->index() ) );
	}
}
//
void UIProjectSettingsProxyModel::accept()
{
	QDialog::accept();
}
//
void UIProjectSettingsProxyModel::reject()
{
	QDialog::reject();
}
