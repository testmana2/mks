#include "QMakeProjectItemDelegate.h"
//
#include <QMetaEnum>
#include <QSpinBox>
//
QMakeProjectItemDelegate::QMakeProjectItemDelegate( QWidget* p )
	: QItemDelegate( p )
{
}
//
QWidget* QMakeProjectItemDelegate::createEditor( QWidget* w, const QStyleOptionViewItem&, const QModelIndex& i ) const
{
	if ( !i.isValid() || i.column() == 0 )
		return 0;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case QMakeProjectItem::TypeRole:
		{
			QComboBox* cb = new QComboBox( w );
			const QMetaObject mo = QMakeProjectItem::staticMetaObject;
			QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeType" ) );
			for ( int j = QMakeProjectItem::FirstType +1; j < QMakeProjectItem::LastType; j++ )
				cb->addItem( me.key( j ), me.value( j ) );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
			break;
		}
		case QMakeProjectItem::ValueRole:
		case QMakeProjectItem::CommentRole:
		case QMakeProjectItem::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = new QValueFileFolderEdit( w, i );
			vffe->setText( i.data().toString() );
			return vffe;
		}
		/*
		case QMakeProjectItem::IndentRole:
		case QMakeProjectItem::SortRole:
		case QMakeProjectItem::SortRole2:
		{
			QSpinBox* sb = new QSpinBox( w );
			sb->setValue( i.data().toInt() );
			return sb;
		}
		*/
		case QMakeProjectItem::OperatorRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItems( QStringList() << "" << "=" << "-=" << "+=" << "*=" << "~=" );
			cb->setCurrentIndex( cb->findText( i.data().toString() ) );
			return cb;
		}
		/*
		case QMakeProjectItem::FilterRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItem( tr( "true" ), "true" );
			cb->addItem( tr( "false" ), "false" );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		*/
		case QMakeProjectItem::ProjectRole:
		{
			break;
		}
	}
	return 0;
}
//
void QMakeProjectItemDelegate::setModelData( QWidget* w, QAbstractItemModel* m, const QModelIndex& i ) const
{
	//if ( !w || !m || !i.isValid() || i.column() == 0 )
		//return;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case QMakeProjectItem::TypeRole:
		//case QMakeProjectItem::FilterRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->itemData( cb->currentIndex() ), Qt::DisplayRole );
			return;
			break;
		}
		case QMakeProjectItem::ValueRole:
		case QMakeProjectItem::CommentRole:
		case QMakeProjectItem::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = qobject_cast<QValueFileFolderEdit*>( w );
			m->setData( i, vffe->text(), Qt::DisplayRole );
			return;
		}
		/*
		case QMakeProjectItem::IndentRole:
		case QMakeProjectItem::SortRole:
		case QMakeProjectItem::SortRole2:
		{
			QSpinBox* sb = qobject_cast<QSpinBox*>( w );
			m->setData( i, sb->value(), Qt::DisplayRole );
			return;
		}
		*/
		case QMakeProjectItem::OperatorRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->currentText(), Qt::DisplayRole );
			return;
		}
		case QMakeProjectItem::ProjectRole:
		{
			return;
		}
	}
}
