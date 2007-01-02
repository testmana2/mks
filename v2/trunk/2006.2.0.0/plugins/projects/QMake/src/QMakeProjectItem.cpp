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
QMakeProjectItem::~QMakeProjectItem()
{
}
//
int QMakeProjectItem::type() const
{
	return mType;
}
//
void QMakeProjectItem::initialize()
{
/*
	QMakeProjectItemModel* mModel = qobject_cast<QMakeProjectItemModel*>( model() );
	if ( !mModel )
		return;
	//
	switch ( mType )
	{
	case Project:
		setText( mModel->projectName() );
		break;
	case Forms:
		setText( QObject::tr( "Forms" ) );
		break;
	case Headers:
		setText( QObject::tr( "Headers" ) );
		break;
	case Sources:
		setText( QObject::tr( "Sources" ) );
		break;
	case Translations:
		setText( QObject::tr( "Translations" ) );
		break;
	case Dir:
		break;
	case File:
		break;
		
	case Scope:
		break;
		
	default:
		break;
	}
*/
}
