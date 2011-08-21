#include "pGenericTableModel.h"

#include <QDebug>

pGenericTableModel::pGenericTableModel( QObject* parent )
	: QAbstractTableModel( parent )
{
}

pGenericTableModel::~pGenericTableModel()
{
}

int pGenericTableModel::columnCount( const QModelIndex& parent ) const
{
	return parent == QModelIndex() ? mColumnCount : 0;
}

int pGenericTableModel::rowCount( const QModelIndex& parent ) const
{
	return parent == QModelIndex() ? mRowCount : 0;
}

QVariant pGenericTableModel::data( const QModelIndex& index, int role ) const
{
	switch ( role ) {
		case pGenericTableModel::ItemFlags:
			return QVariant::fromValue( flags( index ) );
		default:
			return indexInternalData( index ).value( role );
	}
}

QVariant pGenericTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	return headerInternalData( section, orientation ).value( role );
}

bool pGenericTableModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	pGenericTableModel::IntVariantMap* map = indexInternalData( index );
	
	if ( !map ) {
		return false;
	}
	
	(*map)[ role ] = value;
	emit dataChanged( index, index );
	return true;
}

bool pGenericTableModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role )
{
	pGenericTableModel::IntVariantMap* map = headerInternalData( section, orientation );
	
	if ( !map ) {
		return false;
	}
	
	(*map)[ role ] = value;
	emit headerDataChanged( orientation, section, section );
	return true;
}

Qt::ItemFlags pGenericTableModel::flags( const QModelIndex& index ) const
{
	const QVariant variant = indexInternalData( index ).value( pGenericTableModel::ItemFlags );
	return variant.isNull() ? QAbstractTableModel::flags( index ) : variant.value<Qt::ItemFlags>();
}

void pGenericTableModel::sort( int column, Qt::SortOrder order )
{
	Q_UNUSED( column );
	Q_UNUSED( order );
	qWarning() << Q_FUNC_INFO << "Not yet implemented";
}

void pGenericTableModel::setColumnCount( int count )
{
	if ( count == mColumnCount ) {
		return;
	}
	
	if ( count > mColumnCount ) {
		const int diff = count -mColumnCount;
		beginInsertColumns( QModelIndex(), mColumnCount, diff -1 );
		mColumnCount = count;
		endInsertColumns();
	}
	else {
		beginRemoveColumns( QModelIndex(), count, mColumnCount -1 );
		mColumnCount = count;
		foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
			if ( point.first >= mColumnCount ) {
				mData.remove( point );
			}
		}
		foreach ( const pGenericTableModel::Point& point, mHeaderData.keys() ) {
			if ( point.first == Qt::Horizontal && point.second >= mColumnCount ) {
				mHeaderData.remove( point );
			}
		}
		endRemoveColumns();
	}
}

void pGenericTableModel::setRowCount( int count )
{
	if ( count == mRowCount ) {
		return;
	}
	
	if ( count > mRowCount ) {
		const int diff = count -mRowCount;
		beginInsertRows( QModelIndex(), mRowCount, diff -1 );
		mRowCount = count;
		endInsertRows();
	}
	else {
		beginRemoveRows( QModelIndex(), count, mRowCount -1 );
		mRowCount = count;
		foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
			if ( point.second >= mRowCount ) {
				mData.remove( point );
			}
		}
		foreach ( const pGenericTableModel::Point& point, mHeaderData.keys() ) {
			if ( point.first == Qt::Vertical && point.second >= mRowCount ) {
				mHeaderData.remove( point );
			}
		}
		endRemoveRows();
	}
}

pGenericTableModel::IntVariantMap* pGenericTableModel::indexInternalData( const QModelIndex& index )
{
	return index.isValid() ? &mData[ qMakePair( index.column(), index.row() ) ] : 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::indexInternalData( const QModelIndex& index ) const
{
	return mData.value( qMakePair( index.column(), index.row() ) );
}

pGenericTableModel::IntVariantMap* pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation )
{
	switch ( orientation ) {
		case Qt::Horizontal:
			return section >= 0 && section < mColumnCount ? &mHeaderData[ qMakePair( (int)orientation, section ) ] : 0;
		case Qt::Vertical:
			return section >= 0 && section < mRowCount ? &mHeaderData[ qMakePair( (int)orientation, section ) ] : 0;
	}
	
	return 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation ) const
{
	return mHeaderData.value( qMakePair( (int)orientation, section ) );
}

void pGenericTableModel::clear( bool onlyData )
{
	if ( rowCount() == 0 && columnCount() == 0 ) {
		return;
	}
	
	if ( onlyData ) {
		beginResetModel();
		mData.clear();
		endResetModel();
	}
	else {
		beginRemoveColumns( QModelIndex(), 0, mColumnCount -1 );
		mColumnCount = 0;
		mHeaderData.clear();
		endRemoveColumns();
		
		beginRemoveRows( QModelIndex(), 0, mRowCount -1 );
		mRowCount = 0;
		mData.clear();
		endRemoveRows();
	}
}

QModelIndexList pGenericTableModel::checkedIndexes() const
{
	QModelIndexList indexes;
	
	foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
		const pGenericTableModel::IntVariantMap& map = mData[ point ];
		
		if ( map.value( Qt::CheckStateRole ).toInt() != Qt::Unchecked ) {
			indexes << index( point.second, point.first );
		}
	}
	
	return indexes;
}

QList<int> pGenericTableModel::checkedRows() const
{
	QList<int> rows;
	
	foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
		const pGenericTableModel::IntVariantMap& map = mData[ point ];
		
		if ( map.value( Qt::CheckStateRole ).toInt() != Qt::Unchecked ) {
			rows << point.second;
		}
	}
	
	return rows;
}
