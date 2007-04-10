#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "ProjectItemModel.h"
#include "UIProjectSettingsProxyModel.h"
#include "ProjectItemDelegate.h"
//
#include <QMetaEnum>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFontMetrics>
//
QPointer<UIItemSettings> UIItemSettings::mSelf = 0L;
//
UIItemSettings* UIItemSettings::edit( ProjectItemModel* p, ProjectItem* i, QWidget* w )
{
	if ( !mSelf )
		mSelf = new UIItemSettings( p, i, w );
	return mSelf;
}
//
UIItemSettings::UIItemSettings( ProjectItemModel* p, ProjectItem* pi, QWidget* w )
	: QDialog( w ), mItem( pi ), mProject( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twValueRoles->setItemDelegate( new ProjectItemDelegate( twValueRoles ) );
	// fill list with node type
	const QMetaObject mo = ProjectItem::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twValueRoles );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		it->setText( 0, me.key( i ) );
		it->setData( 0, Qt::UserRole +1, me.value( i ) );
		if ( pi )
			it->setText( 1, mItem->data( me.value( i ) ).toString() );
		// longer string in pixel for resizing the first column
		if ( fm.width( it->text( 0 ) ) > pMax )
			pMax = fm.width( it->text( 0 ) );
	}
	twValueRoles->setColumnWidth( 0, pMax +10 );
}
//
void UIItemSettings::accept()
{
	if ( !mItem )
	// if no valid index, create one
	{
		UIProjectSettingsProxyModel* d = qobject_cast<UIProjectSettingsProxyModel*>( parentWidget() );
		if ( d )
		{
			int r = 0;
			int rc = 0;
			mItem = dynamic_cast<ProjectItem*>( mProject->itemFromIndex( d->currentIndex() ) );
			if ( mItem )
			{
				r = mItem->row();
				rc = mItem->rowCount();
			}
			else
			{
				r = 0;
				rc = mProject->rowCount();
			}
			// get the node type
			ProjectItem::NodeType pNodeType = ( ProjectItem::NodeType )twValueRoles->topLevelItem( 0 )->data( 1, Qt::DisplayRole ).toInt();
			// ask which kind of item we must create
			if ( QMessageBox::question( this, tr( "New Item.." ), tr( "Create item as a child of the selected item ?" ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
			{
				if ( mItem )
					mItem->insertRow( rc, new ProjectItem( pNodeType ) );
				else
					mProject->insertRow( rc, new ProjectItem( pNodeType ) );					
				mItem = dynamic_cast<ProjectItem*>( mItem ? mItem->child( rc ) : mProject->item( rc ) );
			}
			else
			{
				if ( mItem && mItem->QStandardItem::parent() )
					mItem->QStandardItem::parent()->insertRow( r, new ProjectItem( pNodeType ) );
				else
					mProject->insertRow( r, new ProjectItem( pNodeType ) );
				mItem = dynamic_cast<ProjectItem*>( mItem && mItem->QStandardItem::parent() ? mItem->QStandardItem::parent()->child( r ) : mProject->item( r ) );
			}
		}
	}
	//
	if ( mItem )
	// update index data
	{
		for ( int i = 0; i < twValueRoles->topLevelItemCount(); i++ )
			mItem->setData( twValueRoles->topLevelItem( i )->text( 1 ), twValueRoles->topLevelItem( i )->data( 0, Qt::UserRole +1 ).toInt() );
		mProject->updateItem( mItem );
	}
	//
	QDialog::accept();
}
