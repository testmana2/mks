#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseSQL.h"

#include <QSqlRecord>
#include <QFileInfo>
#include <QDebug>

qCtagsSenseFilesModel::qCtagsSenseFilesModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
}

qCtagsSenseFilesModel::~qCtagsSenseFilesModel()
{
}

int qCtagsSenseFilesModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseFilesModel::data( const QModelIndex& index, int role ) const
{
	if ( index.isValid() )
	{
		if ( role == Qt::DisplayRole )
		{
			return QFileInfo( mFiles.at( index.internalId() ) ).fileName();
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mFiles.at( index.internalId() );
		}
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseFilesModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		if ( row >= 0 && row < rowCount( parent ) && column >= 0 &&  column < columnCount( parent ) )
		{
			return createIndex( row, column, row );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseFilesModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int qCtagsSenseFilesModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : mFiles.count();
}

bool qCtagsSenseFilesModel::hasChildren( const QModelIndex& parent ) const
{
	return parent.isValid() ? false : !mFiles.isEmpty();
}

QString qCtagsSenseFilesModel::language() const
{
	return mLanguage;
}
	
QString qCtagsSenseFilesModel::fileName( int id ) const
{
	return mFiles.value( id );
}

int qCtagsSenseFilesModel::indexOf( const QString& fileName ) const
{
	return mFiles.indexOf( fileName );
}

void qCtagsSenseFilesModel::refresh( const QString& language  )
{
	mFiles.clear();
	
	const QString sql = QString(
		"SELECT filename FROM files WHERE LOWER( language ) = LOWER( '%1' )"
	).arg( language );
	
	QSqlQuery q = mSQL->query();
	
	if ( q.exec( sql ) )
	{
		mLanguage = language;
		
		while ( q.next() )
		{
			const QString fileName = q.record().value( 0 ).toString();
			mFiles << fileName;
		}
		
		qSort( mFiles.begin(), mFiles.end(), caseInsensitiveFilePathLessThan );
	}
	else
	{
		qWarning() << "Can't get files";
	}
	
	reset();
}

bool qCtagsSenseFilesModel::caseInsensitiveFilePathLessThan( const QString& s1, const QString& s2 )
{
	return QFileInfo( s1 ).fileName().toLower() < QFileInfo( s2 ).fileName().toLower();
}
