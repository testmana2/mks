#include "pConsoleManagerStepModel.h"

#include <QDebug>

#define pConsoleManagerStepModelColumnCount 1

pConsoleManagerStepModel::pConsoleManagerStepModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mWarnings = 0;
	mErrors = 0;
}

pConsoleManagerStepModel::~pConsoleManagerStepModel()
{
}

int pConsoleManagerStepModel::columnCount( const QModelIndex& parent ) const
{
	return parent != QModelIndex() ? 0 : pConsoleManagerStepModelColumnCount;
}

QVariant pConsoleManagerStepModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() ) {
		return QVariant();
	}
	
	const pConsoleManagerStep* step = static_cast<pConsoleManagerStep*>( index.internalPointer() );
	return step->roleValue( role );
}

QModelIndex pConsoleManagerStepModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( parent != QModelIndex() || row < 0 || row >= mSteps.count() || column < 0 || column >= pConsoleManagerStepModelColumnCount ) {
		return QModelIndex();
	}
	
	return createIndex( row, column, &mSteps[ row ] );
}

QModelIndex pConsoleManagerStepModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int pConsoleManagerStepModel::rowCount( const QModelIndex& parent ) const
{
	return parent != QModelIndex() ? 0 : mSteps.count();
}

bool pConsoleManagerStepModel::hasChildren( const QModelIndex& parent ) const
{
	return parent != QModelIndex() ? false : !mSteps.isEmpty();
}

QModelIndex pConsoleManagerStepModel::index( const pConsoleManagerStep& step ) const
{
	const int row = mSteps.indexOf( step );
	return row == -1 ? QModelIndex() : createIndex( row, 0, &mSteps[ row ] );
}

pConsoleManagerStep pConsoleManagerStepModel::step( const QModelIndex& index ) const
{
	return mSteps.value( index.row() );
}

QModelIndex pConsoleManagerStepModel::nextErrorOrWarning( const QModelIndex& fromIndex ) const
{
	const int row = fromIndex.isValid() ? fromIndex.row() +1 : 0;
	
	if ( row >= rowCount() ) {
		return QModelIndex();
	}
	
	for ( int i = row; i < rowCount(); i++ ) {
		pConsoleManagerStep& step = mSteps[ i ];
		
		if ( step.type() == pConsoleManagerStep::Warning || step.type() == pConsoleManagerStep::Error ) {
			return createIndex( i, 0, &step );
		}
	}
	
	return QModelIndex();
}

QModelIndex pConsoleManagerStepModel::nextWarning( const QModelIndex& fromIndex ) const
{
	const int row = fromIndex.isValid() ? fromIndex.row() +1 : 0;
	
	if ( row >= rowCount() ) {
		return QModelIndex();
	}
	
	for ( int i = row; i < rowCount(); i++ ) {
		pConsoleManagerStep& step = mSteps[ i ];
		
		if ( step.type() == pConsoleManagerStep::Warning ) {
			return createIndex( i, 0, &step );
		}
	}
	
	return QModelIndex();
}

QModelIndex pConsoleManagerStepModel::nextError( const QModelIndex& fromIndex ) const
{
	const int row = fromIndex.isValid() ? fromIndex.row() +1 : 0;
	
	if ( row >= rowCount() ) {
		return QModelIndex();
	}
	
	for ( int i = row; i < rowCount(); i++ ) {
		pConsoleManagerStep& step = mSteps[ i ];
		
		if ( step.type() == pConsoleManagerStep::Error ) {
			return createIndex( i, 0, &step );
		}
	}
	
	return QModelIndex();
}

void pConsoleManagerStepModel::clear()
{
	const int count = rowCount();
	
	if ( count == 0 ) {
		return;
	}
	
	beginRemoveRows( QModelIndex(), 0, count -1 );
	mSteps.clear();
	mWarnings = 0;
	mErrors = 0;
	endRemoveRows();
}

void pConsoleManagerStepModel::appendStep( const pConsoleManagerStep& step )
{
	// get last type
	const pConsoleManagerStep::Type type = mSteps.isEmpty() ? pConsoleManagerStep::Unknown : mSteps.last().type();
	const int count = mSteps.count();
	
	// update warnings/errors
	switch ( step.type() ) {
		case pConsoleManagerStep::Warning:
			mWarnings++;
			break;
		case pConsoleManagerStep::Error:
			mErrors++;
			break;
		default:
			break;
	}

	// add step
	switch ( type ) {
		case pConsoleManagerStep::Compiling: {
			switch ( step.type() ) {
				// add before last item ( count -1 -1 )
				case pConsoleManagerStep::Warning:
				case pConsoleManagerStep::Error: {
					beginInsertRows( QModelIndex(), count -2, count -2 );
					mSteps.insert( count -2, step );
					endInsertRows();
					break;
				}
				// replace last (count -1 )
				default: {
					mSteps[ count -1 ] = step;
					const QModelIndex index = this->index( step );
					emit dataChanged( index, index );
					break;
				}
			}
			
			break;
		}
		default: {
			beginInsertRows( QModelIndex(), count, count );
			mSteps << step;
			endInsertRows();
			break;
		}
	}
	
	// if step is finish, need set error, warning text if needed
	if ( step.type() == pConsoleManagerStep::Finish ) {
		pConsoleManagerStep* _step = &mSteps.last();
		
		if ( step.roleValue( Qt::DisplayRole ).toString().isEmpty() ) {
			_step->setRoleValue( pConsoleManagerStep::TypeRole, mErrors ? pConsoleManagerStep::Bad : pConsoleManagerStep::Good );
			_step->setRoleValue( Qt::DisplayRole, tr( "Command terminated, error(s): %1, warning(s): %2" ).arg( mErrors ).arg( mWarnings ) );
		}
		else {
			_step->setRoleValue( pConsoleManagerStep::TypeRole, pConsoleManagerStep::Bad );
		}
		
		const QModelIndex idx = index( *_step );
		emit dataChanged( idx, idx );
	}
}

void pConsoleManagerStepModel::appendSteps( const pConsoleManagerStepList& steps )
{
	// do a hacky loop for now as this member is not yet used
	foreach ( const pConsoleManagerStep& step, steps ) {
		appendStep( step );
	}
}
