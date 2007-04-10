#include "ProjectItemDelegate.h"
//
#include <QMetaEnum>
#include <QSpinBox>
//
ProjectItemDelegate::ProjectItemDelegate( QObject* p )
	: QItemDelegate( p )
{
}
//
QWidget* ProjectItemDelegate::createEditor( QWidget* w, const QStyleOptionViewItem&, const QModelIndex& i ) const
{
	if ( !i.isValid() || i.column() == 0 )
		return 0;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case ProjectItem::TypeRole:
		{
			QComboBox* cb = new QComboBox( w );
			const QMetaObject mo = ProjectItem::staticMetaObject;
			QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeType" ) );
			for ( int j = 0; j < me.keyCount() -1; j++ )
				cb->addItem( me.key( j ), me.value( j ) );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		case ProjectItem::ValueRole:
		case ProjectItem::CommentRole:
		case ProjectItem::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = new QValueFileFolderEdit( w, i );
			vffe->setText( i.data().toString() );
			return vffe;
		}
		case ProjectItem::IndentRole:
		case ProjectItem::SortRole:
		case ProjectItem::SortRole2:
		{
			QSpinBox* sb = new QSpinBox( w );
			sb->setValue( i.data().toInt() );
			return sb;
		}
		case ProjectItem::OperatorRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItems( QStringList() << "" << "=" << "-=" << "+=" << "*=" << "~=" );
			cb->setCurrentIndex( cb->findText( i.data().toString() ) );
			return cb;
		}
		case ProjectItem::FilterRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItem( tr( "true" ), "true" );
			cb->addItem( tr( "false" ), "false" );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		case ProjectItem::ProjectRole:
		{
			break;
		}
	}
	return 0;
}
//
void ProjectItemDelegate::setModelData( QWidget* w, QAbstractItemModel* m, const QModelIndex& i ) const
{
	if ( !w || !m || !i.isValid() || i.column() == 0 )
		return;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case ProjectItem::TypeRole:
		case ProjectItem::FilterRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->itemData( cb->currentIndex() ), Qt::DisplayRole );
			return;
		}
		case ProjectItem::ValueRole:
		case ProjectItem::CommentRole:
		case ProjectItem::AbsoluteFilePathRole:
		{
			QValueFileFolderEdit* vffe = qobject_cast<QValueFileFolderEdit*>( w );
			m->setData( i, vffe->text(), Qt::DisplayRole );
			return;
		}
		case ProjectItem::IndentRole:
		case ProjectItem::SortRole:
		case ProjectItem::SortRole2:
		{
			QSpinBox* sb = qobject_cast<QSpinBox*>( w );
			m->setData( i, sb->value(), Qt::DisplayRole );
			return;
		}
		case ProjectItem::OperatorRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->currentText(), Qt::DisplayRole );
			return;
		}
		case ProjectItem::ProjectRole:
		{
			return;
		}
	}
}
