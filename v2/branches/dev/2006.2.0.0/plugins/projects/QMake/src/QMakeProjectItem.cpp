#include "QMakeProjectItem.h"
#include "QMakeProjectItemModel.h"
//
#include <QObject>
//
QMakeProjectItem::QMakeProjectItem( QMakeProjectItem::NodeType t )
	: mType( t )
{
	setData( t, QMakeProjectItem::TypeRole );
	setData( false, QMakeProjectItem::NegateRole );
}
//
int QMakeProjectItem::type() const
{
	return mType;
}
