#include "UIItemSettings.h"
#include "QMakeProjectItem.h"
#include "QMakeProjectModel.h"
#include "UIQMakeProjectSettings.h"
#include "QMakeProjectItemDelegate.h"
//
#include <QMetaEnum>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFontMetrics>
//
QPointer<UIItemSettings> UIItemSettings::mSelf = 0L;
//
UIItemSettings* UIItemSettings::edit( QMakeProjectModel* p, QMakeProjectItem* i, QWidget* w )
{
	if ( !mSelf )
		mSelf = new UIItemSettings( p, i, w );
	return mSelf;
}
//
UIItemSettings::UIItemSettings( QMakeProjectModel* p, QMakeProjectItem* pi, QWidget* w )
	: QDialog( w ), mItem( pi ), mProject( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twValueRoles->setItemDelegate( new QMakeProjectItemDelegate( twValueRoles ) );
	// fill list with node type
	const QMetaObject mo = QMakeProjectItem::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		if ( me.value( i ) == QMakeProjectItem::FirstRole || me.value( i ) == QMakeProjectItem::LastRole )
			continue;
		QTreeWidgetItem* it = new QTreeWidgetItem( twValueRoles );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		it->setText( 0, me.key( i ) );
		it->setData( 0, Qt::UserRole +1, me.value( i ) );
		if ( pi )
			it->setText( 1, mItem->data( me.value( i ) ).toString() );
		// longest string in pixel for resizing the first column
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
		UIQMakeProjectSettings* d = qobject_cast<UIQMakeProjectSettings*>( parentWidget() );
		if ( d )
		{
			int r = 0;
			int rc = 0;
			// get the current index in the project settings
			mItem = static_cast<QMakeProjectItem*>( mProject->itemFromIndex( d->currentIndex() ) );
			// get it s row and rowcount
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
			QMakeProjectItem::NodeType pNodeType = ( QMakeProjectItem::NodeType )twValueRoles->topLevelItem( 0 )->data( 1, Qt::DisplayRole ).toInt();
			// ask which kind of item we must create
			if ( QMessageBox::question( this, tr( "New Item.." ), tr( "Create item as a child of the selected item ?" ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
			// as child
			{
				mProject->insertRow( rc, new QMakeProjectItem( pNodeType ), mItem );
				mItem = static_cast<QMakeProjectItem*>( mItem ? mItem->row( rc ) : mProject->row( rc ) );
			}
			else
			{
				mProject->insertRow( r, new QMakeProjectItem( pNodeType ), mItem->parent() );
				mItem = static_cast<QMakeProjectItem*>( mItem && mItem->parent() ? mItem->parent()->row( r ) : mProject->row( r ) );
			}
			// set the new item the current one
			d->setCurrentIndex( mProject->indexFromItem( mItem ) );
		}
	}
	// update index data
	if ( mItem )
		for ( int i = 0; i < twValueRoles->topLevelItemCount(); i++ )
			mItem->setData( twValueRoles->topLevelItem( i )->text( 1 ), twValueRoles->topLevelItem( i )->data( 0, Qt::UserRole +1 ).toInt() );
	//
	QDialog::accept();
}
