#include "qCtagsSenseMembersModel.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSense.h"

#include <QSqlRecord>
#include <QDebug>

class qCtagsSenseMembersThread : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseMembersThread( qCtagsSenseSQL* parent )
		: QThread( parent )
	{
		mSQL = parent;
		mStop = false;
		mRestart = false;
	}
	
	virtual ~qCtagsSenseMembersThread()
	{
		QMutexLocker locker( &mMutex );
		mStop = true;
		locker.unlock();
		wait();
	}

public slots:
	void executeQuery( const QString& sql, const QString& fileName, qCtagsSenseEntry* rootEntry )
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = isRunning();
			mStop = false;
			mQuery = sql;
			mRootEntry = rootEntry;
			mFileName = fileName;
		}
		
		if ( !isRunning() )
		{
			start();
		}
	}

protected:
	QMutex mMutex;
	qCtagsSenseSQL* mSQL;
	bool mStop;
	QString mQuery;
	QString mFileName;
	qCtagsSenseEntry* mRootEntry;
	bool mRestart;
	
	qCtagsSenseEntry* entryForRecord( const QSqlRecord& rec )
	{
		qCtagsSenseEntry* entry = new qCtagsSenseEntry;
		
		entry->lineNumberEntry = rec.value( "line_number_entry" ).toBool();
		entry->lineNumber = rec.value( "line_number" ).value<ulong>();
		entry->isFileScope = rec.value( "is_file_scope" ).toBool();
		entry->isFileEntry = rec.value( "is_file_entry" ).toBool();
		entry->truncateLine = rec.value( "truncate_line" ).toBool();
		entry->name = rec.value( "name" ).toString();
		entry->kind = (qCtagsSense::Kind)rec.value( "kind" ).toInt();
		// extended fields
		entry->access = rec.value( "access" ).toString();
		entry->fileScope = rec.value( "file_scope" ).toString();
		entry->implementation = rec.value( "implementation" ).toString();
		entry->inheritance = rec.value( "inerithance" ).toString();
		entry->scope.first = rec.value( "scope_value" ).toString();
		entry->scope.second = rec.value( "scope_key" ).toString();
		entry->signature = rec.value( "signature" ).toString();
		entry->typeRef.first = rec.value( "type" ).toString();
		entry->typeRef.second = rec.value( "type_name" ).toString();
		
		return entry;
	}
	
	virtual void run()
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = false;
		}
		
		forever
		{
			delete mRootEntry;
			mRootEntry = 0;
			
			QSqlQuery q = mSQL->query();
			q.setForwardOnly( true );
			
			if ( !q.exec( mQuery ) )
			{
				qWarning() << "Can't get entries for population";
				return;
			}
			
			mRootEntry = new qCtagsSenseEntry;
			QMap<QString, qCtagsSenseEntry*> scopeItems;
			scopeItems[ QString::null ] = mRootEntry;
			QList<qCtagsSenseEntry*> entries;
			
			mRootEntry->name = mFileName;
			
			while ( q.next() )
			{
				qCtagsSenseEntry* entry = entryForRecord( q.record() );
				entries << entry;
				
				switch ( entry->kind )
				{
					case qCtagsSense::Class:
					case qCtagsSense::Enum:
					case qCtagsSense::Structure:
					{
						QString scope = entry->scope.second;
						
						if ( !scope.isEmpty() )
						{
							scope += "::";
						}
						
						scope += entry->name;
						scopeItems[ scope ] = entry;
						
						break;
					}
					default:
						break;
				}
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						return;
					}
					else if ( mRestart )
					{
						break;
					}
				}
			}
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mRestart )
				{
					mRestart = false;
					locker.unlock();
					qDeleteAll( entries );
					continue;
				}
			}
			
			foreach ( qCtagsSenseEntry* entry, entries )
			{
				entries.removeAll( entry );
				
				qCtagsSenseEntry* parentEntry = scopeItems.value( entry->scope.second );
				
				if ( !parentEntry )
				{
					parentEntry = scopeItems.value( QString::null );
				}
				
				entry->parent = parentEntry;
				parentEntry->children << entry;
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						return;
					}
					else if ( mRestart )
					{
						break;
					}
				}
			}
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mRestart )
				{
					mRestart = false;
					locker.unlock();
					qDeleteAll( entries );
					continue;
				}
			}
			
			emit queryFinished( mRootEntry );
			break;
		}
	}

signals:
	void queryFinished( qCtagsSenseEntry* rootEntry );
};

QMap<QString, QPixmap> qCtagsSenseMembersModel::mPixmaps;

qCtagsSenseMembersModel::qCtagsSenseMembersModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
	mRootEntry = 0;
	mThread = new qCtagsSenseMembersThread( mSQL );
	
	connect( mThread, SIGNAL( queryFinished( qCtagsSenseEntry* ) ), this, SLOT( queryFinished( qCtagsSenseEntry* ) ) );
}

qCtagsSenseMembersModel::~qCtagsSenseMembersModel()
{
}

int qCtagsSenseMembersModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseMembersModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( mRootEntry && section == 0 && orientation == Qt::Horizontal )
	{
		if ( role == Qt::DisplayRole )
		{
			return QFileInfo( mRootEntry->name ).fileName();
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mRootEntry->name;
		}
	}
	
	return QVariant();
}

QVariant qCtagsSenseMembersModel::data( const QModelIndex& index, int role ) const
{
	const QModelIndex parentIndex = index.parent();
	
	if ( !index.isValid() || index.row() > rowCount( parentIndex ) || index.column() > columnCount( parentIndex ) )
	{
		return QVariant();
	}
	
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	
	if ( role == Qt::DisplayRole )
	{
		QString display = entry->name;
		
		if ( entry->parent == mRootEntry )
		{
			if ( !entry->scope.second.isEmpty() )
			{
				display.prepend( QString( "%1::" ).arg( entry->scope.second ) );
			}
		}
		
		switch ( entry->kind )
		{
			case qCtagsSense::Class:
			case qCtagsSense::Structure:
			case qCtagsSense::Union:
				break;
			case qCtagsSense::Macro:
				break;
			case qCtagsSense::Function:
			case qCtagsSense::Prototype:
				display.append( entry->signature );
				break;
			case qCtagsSense::LocalVariable:
			case qCtagsSense::Member:
			case qCtagsSense::Variable:
			case qCtagsSense::ExternVariable:
				if ( !entry->typeRef.first.isEmpty() )
				{
					display.append( QString( ": %1" ).arg( entry->typeRef.first ) );
				}
				break;
		}
		
		return display;
	}
	else if ( role == Qt::ToolTipRole )
	{
		return data( index, Qt::DisplayRole );
	}
	else if ( role == Qt::DecorationRole )
	{
		return entryPixmap( entry );
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseMembersModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( row >= 0 && row < rowCount( parent ) && column >= 0 && column < columnCount( parent ) )
	{
		if ( parent.isValid() )
		{
			qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
			
			return createIndex( row, column, parentEntry->children.value( row ) );
		}
		else
		{
			return createIndex( row, column, mRootEntry->children.value( row ) );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseMembersModel::parent( const QModelIndex& index ) const
{
	if ( index.isValid() )
	{
		qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
		qCtagsSenseEntry* parentEntry = entry->parent;
		
		if ( parentEntry == mRootEntry )
		{
			return QModelIndex();
		}
		
		const int row = parentEntry->parent->children.indexOf( parentEntry );
		
		return createIndex( row, index.column(), parentEntry );
	}
	
	return QModelIndex();
}

int qCtagsSenseMembersModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
		
		return parentEntry->children.count();
	}
	
	return mRootEntry ? mRootEntry->children.count() : 0;
}

bool qCtagsSenseMembersModel::hasChildren( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
		
		return !parentEntry->children.isEmpty();
	}
	
	return mRootEntry ? !mRootEntry->children.isEmpty() : false;
}

void qCtagsSenseMembersModel::populateFromFile( const QString& fileName )
{
	const QString sql = QString(
		"SELECT entries.*, language FROM entries "
		"INNER JOIN files ON files.id = entries.file_id "
		"AND files.filename = '%1'"
	).arg( fileName );
	
	qCtagsSenseEntry* root = mRootEntry;
	mRootEntry = 0;
	
	reset();
	mThread->executeQuery( sql, fileName, root );
}

QPixmap qCtagsSenseMembersModel::entryPixmap( qCtagsSenseEntry* entry )
{
	qCtagsSense::Kind kind = entry->kind;
	QString access = entry->access;
	QString name;
	
	if ( access == "public" )
	{
		access.clear();
	}
	
	switch ( kind )
	{
		case qCtagsSense::Class:
			name = "class";
			access.clear();
			break;
		case qCtagsSense::Macro:
			name = "macro";
			access.clear();
			break;
		case qCtagsSense::Enumerator:
			name = "enumerator";
			access.clear();
			break;
		case qCtagsSense::Function:
		case qCtagsSense::Prototype:
			name = "function";
			break;
		case qCtagsSense::Enum:
			name = "enum";
			access.clear();
			break;
		case qCtagsSense::LocalVariable:
		case qCtagsSense::Member:
		case qCtagsSense::Variable:
		case qCtagsSense::ExternVariable:
			name = "variable";
			break;
		case qCtagsSense::Namespace:
			name = "namespace";
			access.clear();
			break;
		case qCtagsSense::Structure:
			name = "structure";
			access.clear();
			break;
		case qCtagsSense::Typedef:
			name = "typedef";
			access.clear();
			break;
		case qCtagsSense::Union:
			name = "union";
			break;
		case qCtagsSense::Unknow:
			name = "unknow";
			break;
	}
	
	QString fn = name;
	
	if ( !access.isEmpty() )
	{
		fn += "_" +access;
	}
	
	fn += ".png";
	fn = QString( ":/icons/%1" ).arg( fn );
	
	//qWarning() << fn;
	
	if ( !mPixmaps.contains( fn ) )
	{
		mPixmaps[ fn ] = QPixmap( fn );
	}
	
	return mPixmaps.value( fn );
}

void qCtagsSenseMembersModel::queryFinished( qCtagsSenseEntry* rootEntry )
{
	mRootEntry = rootEntry;
	
	reset();
	emit ready();
}

#include "qCtagsSenseMembersModel.moc"
