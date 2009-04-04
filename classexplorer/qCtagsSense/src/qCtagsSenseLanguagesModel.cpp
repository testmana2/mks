#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseSQL.h"

#include <QSqlRecord>
#include <QDebug>

qCtagsSenseLanguagesModel::qCtagsSenseLanguagesModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
}

qCtagsSenseLanguagesModel::~qCtagsSenseLanguagesModel()
{
}

int qCtagsSenseLanguagesModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseLanguagesModel::data( const QModelIndex& index, int role ) const
{
	if ( index.isValid() )
	{
		if ( role == Qt::DisplayRole )
		{
			return mLanguages.at( index.internalId() );
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mLanguages.at( index.internalId() );
		}
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseLanguagesModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		if ( row >= 0 && row < rowCount( parent ) && column >= 0 && column < columnCount( parent ) )
		{
			return createIndex( row, column, row );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseLanguagesModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int qCtagsSenseLanguagesModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : mLanguages.count();
}

bool qCtagsSenseLanguagesModel::hasChildren( const QModelIndex& parent ) const
{
	return parent.isValid() ? false : !mLanguages.isEmpty();
}

QString qCtagsSenseLanguagesModel::language( int id ) const
{
	return mLanguages.value( id );
}

int qCtagsSenseLanguagesModel::indexOf( const QString& language ) const
{
	return mLanguages.indexOf( language );
}

void qCtagsSenseLanguagesModel::refresh()
{
	mLanguages.clear();
	
	const QString sql = QString(
		"SELECT DISTINCT( language ) FROM files"
	);
	
	QSqlQuery q = mSQL->query();
	
	if ( q.exec( sql ) )
	{
		while ( q.next() )
		{
			const QString language = q.record().value( 0 ).toString();
			mLanguages << language;
		}
		
		mLanguages.sort();
	}
	else
	{
		qWarning() << "Can't get languages";
	}
	
	reset();
}
