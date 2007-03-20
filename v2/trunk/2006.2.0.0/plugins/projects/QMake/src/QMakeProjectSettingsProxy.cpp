#include "QMakeProjectSettingsProxy.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
#include "MenuBar.h"
#include "UIProjectSettingsQMake.h"
//
#include <QMenu>
#include <QAbstractItemView>

QMakeProjectSettingsProxy::QMakeProjectSettingsProxy( QMakeProjectItemModel* m )
	: AbstractProjectProxy( m ),mSource(m)
{
	setSourceModel( m );
}
//
bool QMakeProjectSettingsProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !i.isValid() )
		return true;
	// got the needed index
	QModelIndex index = i;
	if ( mSource->hasChildren( i ) )
		index = i.child( r, i.column() );	
	// checking is in settingsView
	if ( isSettingsView() )
	{
		if ( index.data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::Value )
			return false;
		return true;
	}
	// checking is complex model
	else if ( isComplexModel() )
		return true;
	// apply filtering
	switch ( index.data( QMakeProjectItem::TypeRole ).toInt() )
	{
	case QMakeProjectItem::Project:
	case QMakeProjectItem::Folder:
	case QMakeProjectItem::Value:
	case QMakeProjectItem::File:
		return true;
		break;
	case QMakeProjectItem::Scope:
	{
		for ( int j = 0; j < mSource->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
		return false;
		break;
	}
	case QMakeProjectItem::Variable:
		return QMakeProjectItemModel::simpleModelVariables().contains( index.data().toString(), Qt::CaseInsensitive );
		break;
	}
	return false;
}
