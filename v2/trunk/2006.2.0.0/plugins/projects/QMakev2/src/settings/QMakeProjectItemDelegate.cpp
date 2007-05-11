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
		case AbstractProjectModel::TypeRole:
		{
			QComboBox* cb = new QComboBox( w );
			const QMetaObject mo = AbstractProjectModel::staticMetaObject;
			QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeType" ) );
			for ( int j = 0; j < me.keyCount(); j++ )
			{
				if ( me.value( j ) == AbstractProjectModel::FirstType || me.value( j ) == AbstractProjectModel::LastType )
					continue;
				cb->addItem( me.key( j ), me.value( j ) );
			}
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
			break;
		}
		case AbstractProjectModel::ValueRole:
		case AbstractProjectModel::CommentRole:
		case AbstractProjectModel::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = new QValueFileFolderEdit( w, i );
			vffe->setText( i.data().toString() );
			return vffe;
		}
		/*
		case AbstractProjectModel::IndentRole:
		case AbstractProjectModel::SortRole:
		case AbstractProjectModel::SortRole2:
		{
			QSpinBox* sb = new QSpinBox( w );
			sb->setValue( i.data().toInt() );
			return sb;
		}
		*/
		case AbstractProjectModel::OperatorRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItems( QStringList() << "" << "=" << "-=" << "+=" << "*=" << "~=" );
			cb->setCurrentIndex( cb->findText( i.data().toString() ) );
			return cb;
		}
		/*
		case AbstractProjectModel::FilterRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItem( tr( "true" ), "true" );
			cb->addItem( tr( "false" ), "false" );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		*/
		case AbstractProjectModel::ProjectRole:
		{
			break;
		}
	}
	return 0;
}
//
void QMakeProjectItemDelegate::setModelData( QWidget* w, QAbstractItemModel* m, const QModelIndex& i ) const
{
	if ( !w || !m || !i.isValid() || i.column() == 0 )
		return;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case AbstractProjectModel::TypeRole:
		//case AbstractProjectModel::FilterRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->itemData( cb->currentIndex() ), Qt::DisplayRole );
			return;
			break;
		}
		case AbstractProjectModel::ValueRole:
		case AbstractProjectModel::CommentRole:
		case AbstractProjectModel::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = qobject_cast<QValueFileFolderEdit*>( w );
			m->setData( i, vffe->text(), Qt::DisplayRole );
			return;
		}
		/*
		case AbstractProjectModel::IndentRole:
		case AbstractProjectModel::SortRole:
		case AbstractProjectModel::SortRole2:
		{
			QSpinBox* sb = qobject_cast<QSpinBox*>( w );
			m->setData( i, sb->value(), Qt::DisplayRole );
			return;
		}
		*/
		case AbstractProjectModel::OperatorRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->currentText(), Qt::DisplayRole );
			return;
		}
		case AbstractProjectModel::ProjectRole:
		{
			return;
		}
	}
}
