#include "qCtagsSenseIndexer.h"
#include "qCtagsSense.h"
#include "qCtagsSenseSQL.h"

#include <QMutexLocker>
#include <QDebug>

#warning remove me and use pMonkeyStudio::getFiles

QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
			files << file;
		else if ( file.isDir() && recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters, recursive );
			fromDir.cdUp();
		}
	}
	return files;
}


qCtagsSenseIndexer::qCtagsSenseIndexer( qCtagsSenseSQL* parent )
	: QThread( parent )
{
	mSQL = parent;
	mFilteredSuffixes
		<< "*.gif" << "*.png" << "*.mng" << "*.jpg" << "*.jpeg" << "*.tiff" << "*.ico" << "*.icns"
		<< "*.pri" << "*.pro" << "*.qrc" << "*.ui" << "*.ts" << "*.qm" << "*.qch" << "*.xup" << "*.mks"
		<< "*.txt" << "*.iss" << "*.api" << "*.sip" << "*.ini" << "*.css" << "*.bak" << "*.old"
		<< "*.db" << "*.so" << "*.a" << "*.desktop"  << "*.gpl";
}

qCtagsSenseIndexer::~qCtagsSenseIndexer()
{
}

QStringList qCtagsSenseIndexer::filteredSuffixes() const
{
	return mFilteredSuffixes;
}

void qCtagsSenseIndexer::addFilteredSuffixes( const QStringList& suffixes )
{
	foreach ( const QString& suffix, suffixes )
	{
		if ( !mFilteredSuffixes.contains( suffix ) )
		{
			mFilteredSuffixes << suffix;
		}
	}
}

void qCtagsSenseIndexer::addFilteredSuffix( const QString& suffix )
{
	addFilteredSuffixes( QStringList( suffix ) );
}

void qCtagsSenseIndexer::setFilteredSuffixes( const QStringList& suffixes )
{
	mFilteredSuffixes = suffixes;
}

void qCtagsSenseIndexer::setFilteredSuffix( const QString& suffix )
{
	setFilteredSuffixes( QStringList( suffix ) );
}

void qCtagsSenseIndexer::removeFile( const QString& fileName )
{
	QMutexLocker locker( &mMutex );

	if ( !mWaitingDeletion.contains( fileName ) )
	{
		mWaitingDeletion << fileName;
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexFile( const QString& fileName )
{
	QMutexLocker locker( &mMutex );
	
	if ( !mWaitingIndexation.contains( fileName ) )
	{
		mWaitingIndexation[ fileName ] = QString::null;
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexBuffers( const QMap<QString, QString>& buffers )
{
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& fileName, buffers.keys() )
	{
		if ( !mWaitingIndexation.contains( fileName ) )
		{
			mWaitingIndexation[ fileName ] = buffers[ fileName ];
			
			// null buffers are not accepted as they conflict with true files
			if ( mWaitingIndexation[ fileName ].isNull() )
			{
				mWaitingIndexation[ fileName ] = QString( "" );
			}
		}
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

// PROTECTED

bool qCtagsSenseIndexer::removeEntry( const QString& fileName )
{	
	QSqlQuery q = mSQL->query();
	QString del_sql = QString( "DELETE FROM Files WHERE filename = '%1'" ).arg( fileName );
	
	if ( !q.exec( del_sql ) )
	{
		qWarning() << "Can't delete file entry for" << fileName.toLocal8Bit().constData();
		return false;
	}
	
	return true;
}

bool qCtagsSenseIndexer::indexEntry( const QString& fileName )
{
	QMap<QString, TagEntryListItem*> entries;
	bool ok = false;
	QFileInfo file( fileName );
	
	// tag files
	if ( file.isFile() )
	{
		TagEntryListItem* item = tagFileEntry( fileName, ok );
		
		if ( ok && item )
		{
			entries[ fileName ] = item;
		}
	}
	else if ( file.isDir() )
	{
		entries = tagPathEntries( fileName, ok );
	}
	
	// index tags
	ok = ok && indexTags( entries );
	
	// free tags
	foreach ( TagEntryListItem* item, entries )
	{
		freeTagEntryListItem( item );
	}
	
	entries.clear();
	
	return ok;
}

bool qCtagsSenseIndexer::indexEntries( const QMap<QString, QString>& entries )
{
	bool ok = false;
	QMap<QString, TagEntryListItem*> tagEntries = tagBuffersEntries( entries, ok );

	// index tags
	ok = ok && indexTags( tagEntries );
	
	// free tags
	foreach ( TagEntryListItem* item, tagEntries )
	{
		freeTagEntryListItem( item );
	}
	
	tagEntries.clear();
	
	return ok;
}

int qCtagsSenseIndexer::createFileEntry( const QString& fileName, const QString& language )
{
	QSqlQuery q = mSQL->query();
	const QString set_sql = "INSERT INTO files (filename, language) VALUES( ?, ? )";
	
	q.prepare( set_sql );
	q.addBindValue( fileName );
	q.addBindValue( language );
	
	if ( !q.exec() )
	{
		qWarning() << "Can't create file id for" << fileName.toLocal8Bit().constData();
		return -1;
	}
	
	return q.lastInsertId().toInt();
}

bool qCtagsSenseIndexer::createEntries( int fileId, TagEntryListItem* item )
{
	QSqlQuery q = mSQL->query();
	const QString sql = QString(
		"INSERT INTO entries "
		"( file_id, line_number_entry, line_number, is_file_scope, is_file_entry, truncate_line, "
		"name, kind, access, file_scope, implementation, inheritance, scope_value, "
		"scope_key, signature, type, type_name ) "
		"VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )"
	);
	
	while ( item != NULL )
	{
		tagEntryInfo* entry = &item->tag;
		
		q.prepare( sql );
		q.addBindValue( fileId );
		q.addBindValue( entry->lineNumberEntry );
		q.addBindValue( QVariant::fromValue( entry->lineNumber ) );
		q.addBindValue( entry->isFileScope );
		q.addBindValue( entry->isFileEntry );
		q.addBindValue( entry->truncateLine );
		q.addBindValue( entry->name );
		q.addBindValue( qCtagsSense::kindType( QChar( entry->kind ) ) );
		q.addBindValue( entry->extensionFields.access );
		q.addBindValue( entry->extensionFields.fileScope );
		q.addBindValue( entry->extensionFields.implementation );
		q.addBindValue( entry->extensionFields.inheritance );
		q.addBindValue( entry->extensionFields.scope[ 0 ] );
		q.addBindValue( entry->extensionFields.scope[ 1 ] );
		q.addBindValue( entry->extensionFields.signature );
		q.addBindValue( entry->extensionFields.typeRef[ 0 ] );
		q.addBindValue( entry->extensionFields.typeRef[ 1 ] );
		
		if ( !q.exec() )
		{
			qWarning() << "Can't create entry for" << entry->name;
			qWarning() << q.lastError().text();
			return false;
		}
		
		item = item->next;
	}
	
	return true;
}

bool qCtagsSenseIndexer::indexTags( const QMap<QString, TagEntryListItem*>& tags )
{
	// remove already existing files entries
	QString sql = QString(
		"DELETE FROM files WHERE filename in ( %1 )"
	);
	
	QStringList list = QStringList( tags.keys() ).replaceInStrings( QRegExp( "^(.*)$" ), "'\\1'" );
	QSqlQuery q = mSQL->query();
	sql = sql.arg( list.join( ", " ) );
	
	if ( !q.exec( sql ) )
	{
		qWarning() << "Can't remove entries for" << list;
		return false;
	}
	
	foreach ( const QString& fileName, tags.keys() )
	{
		TagEntryListItem* tag = tags[ fileName ];
		
		int fileId = createFileEntry( fileName, QString::fromLocal8Bit( tag->tag.language ) );
		
		if ( fileId == -1 )
		{
			return false;
		}
		
		if ( !createEntries( fileId, tag ) )
		{
			return false;
		}
	}
	
	return true;
}

TagEntryListItem* qCtagsSenseIndexer::tagFileEntry( const QString& fileName, bool& ok )
{
	if ( !QFile::exists( fileName ) )
	{
		qWarning() << "File does not exists" << fileName.toLocal8Bit().constData();
		ok = false;
		return 0;
	}
	
	if ( QDir::match( mFilteredSuffixes, fileName ) )
	{
		// skipping file is not an error
		ok = true;
		return 0;
	}
	
	// files with no suffixes can't be parsed
	if ( QFileInfo( fileName ).suffix().isEmpty() )
	{
		ok = true;
		return 0;
	}
	
	ok = true;
	return createTagEntryListItem( fileName.toLocal8Bit().constData(), 0 );
}

QMap<QString, TagEntryListItem*> qCtagsSenseIndexer::tagPathEntries( const QString& pathName, bool& ok )
{
	QMap<QString, TagEntryListItem*> entries;
	
	if ( !QFile::exists( pathName ) )
	{
		qWarning() << "Path does not exists" << pathName.toLocal8Bit().constData();
		ok = false;
		return entries;
	}
	
	QDir dir( pathName );

	foreach ( const QFileInfo& file, getFiles( dir, QStringList( "*" ), true ) )
	{
		TagEntryListItem* item = tagFileEntry( file.absoluteFilePath(), ok );
		
		if ( !ok )
		{
			qWarning() << "Failed to index" << file.absoluteFilePath().toLocal8Bit().constData();
			qWarning() << "Failed to index" << pathName.toLocal8Bit().constData();
			return entries;
		}
		else if ( ok && item )
		{
			entries[ file.absoluteFilePath() ] = item;
		}
	}
	
	ok = true;
	return entries;
}

QMap<QString, TagEntryListItem*> qCtagsSenseIndexer::tagBuffersEntries( const QMap<QString, QString>& entries, bool& ok )
{
	QMap<QString, TagEntryListItem*> tagEntries;
	
	foreach ( const QString& fileName, entries.keys() )
	{
		QFile  file( QString( "%1/mks_ctags_%2" ).arg( QDir::tempPath() ).arg( QFileInfo( fileName ).fileName() ) );
		
		if ( !file.open( QIODevice::WriteOnly ) )
		{
			qWarning() << "Failed to create temporary file" << fileName.toLocal8Bit().constData();
			ok = false;
			return tagEntries;
		}
		
		file.resize( 0 );
		
		if ( file.write( entries[ fileName ].toUtf8() ) == -1 )
		{
			qWarning() << "Failed to write in temporary file" << fileName.toLocal8Bit().constData();
			ok = false;
			file.close();
			file.remove();
			return tagEntries;
		}
		
		file.close();
		
		TagEntryListItem* item = tagFileEntry( file.fileName(), ok );
		
		file.remove();
		
		if ( !ok )
		{
			qWarning() << "Failed to index" << fileName.toLocal8Bit().constData();
			return tagEntries;
		}
		else if ( ok && item )
		{
			tagEntries[ fileName ] = item;
		}
	}
	
	ok = true;
	return tagEntries;
}

void qCtagsSenseIndexer::run()
{
	bool changed = false;
	bool error = false;
	
	// start transaction
	mSQL->database().transaction();
	
	forever
	{
		QMutexLocker locker( &mMutex );
		
		// copy
		QStringList fileNamesToRemove = mWaitingDeletion;
		QMap<QString, QString> fileNamesToIndex = mWaitingIndexation;
		
		// clear
		mWaitingDeletion.clear();
		mWaitingIndexation.clear();
		
		locker.unlock();
		
		// deletion
		while ( !fileNamesToRemove.isEmpty() )
		{
			if ( removeEntry( fileNamesToRemove.takeFirst() ) )
			{
				changed = true;
			}
			else if ( !error )
			{
				qWarning() << "Error while removing file";
				error = true;
			}
		}
		
		// indexation
		while ( !fileNamesToIndex.isEmpty() )
		{
			foreach ( const QString& fileName, fileNamesToIndex.keys() )
			{
				if ( fileNamesToIndex[ fileName ].isNull() )
				{
					if ( indexEntry( fileName ) )
					{
						changed = true;
					}
					else if ( !error )
					{
						qWarning() << "Error while indexing files";
						error = true;
					}
					
					fileNamesToIndex.remove( fileName );
				}
			}
			
			if ( !fileNamesToIndex.isEmpty() )
			{
				if ( indexEntries( fileNamesToIndex ) )
				{
					changed = true;
				}
				else if ( !error )
				{
					qWarning() << "Error while indexing buffers";
					error = true;
				}
				
				fileNamesToIndex.clear();
			}
		}
		
		locker.relock();
		
		if ( !mWaitingDeletion.isEmpty() || !mWaitingIndexation.isEmpty() )
		{
			continue;
		}
		
		break;
	}
	
	if ( error )
	{
		// rollback transaction
		mSQL->database().rollback();
	}
	else
	{
		// commit transaction
		mSQL->database().commit();
	}
	
	if ( changed && !error )
	{
		// inform that db has changed
		emit indexChanged();
	}
}
