/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "qCtagsSenseIndexer.h"
#include "qCtagsSenseUtils.h"
#include "qCtagsSenseSQL.h"

#include <ctags.h>

#include <QMutexLocker>
#include <QVariant>
#include <QSqlError>
#include <QTime>
#include <QDebug>

qCtagsSenseIndexer::qCtagsSenseIndexer()
	: QThread()
{
	mStop = false;
	initCtags();
}

qCtagsSenseIndexer::~qCtagsSenseIndexer()
{
	deInitCtags();
	mStop = true;
	wait();
}

void qCtagsSenseIndexer::clear()
{
	QMutexLocker locker( &mMutex );
	
	const bool wasStopped = mStop;
	mFilteredSuffixes.clear();
	mWaitingDeletion.clear();
	mWaitingIndexation.clear();
	mStop = true;
	wait();
	mStop = wasStopped;
}

QStringList qCtagsSenseIndexer::filteredSuffixes() const
{
	QMutexLocker locker( &const_cast<qCtagsSenseIndexer*>( this )->mMutex );
	
	return mFilteredSuffixes;
}

void qCtagsSenseIndexer::addFilteredSuffixes( const QStringList& suffixes )
{
	QMutexLocker locker( &mMutex );
	
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
	QMutexLocker locker( &mMutex );
	
	mFilteredSuffixes = suffixes;
}

void qCtagsSenseIndexer::setFilteredSuffix( const QString& suffix )
{
	setFilteredSuffixes( QStringList( suffix ) );
}

void qCtagsSenseIndexer::removeFile( const QString& fileName )
{
	if ( mStop )
	{
		return;
	}
	
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

void qCtagsSenseIndexer::indexFile( const QString& fileName, const qCtagsSenseProperties& properties )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	if ( !mWaitingIndexation.contains( fileName ) && QFile::exists( fileName ) )
	{
		mWaitingIndexation[ fileName ] = QPair<QString, qCtagsSenseProperties>(QString::null, properties);
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexFiles( const QStringList& fileNames, const qCtagsSenseProperties& properties )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& fileName, fileNames )
	{
		if ( !mWaitingIndexation.contains( fileName ) && QFile::exists( fileName ) )
		{
			mWaitingIndexation[ fileName ] = QPair<QString, qCtagsSenseProperties>(QString::null, properties);
		}
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexBuffers( const QMap<QString, QString>& buffers, const qCtagsSenseProperties& properties )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& fileName, buffers.keys() )
	{
		if ( !mWaitingIndexation.contains( fileName ) )
		{
			mWaitingIndexation[ fileName ] = QPair<QString, qCtagsSenseProperties>( buffers[ fileName ], properties );
			
			// null buffers are not accepted as they conflict with true files
			if ( mWaitingIndexation[ fileName ].first.isNull() )
			{
				mWaitingIndexation[ fileName ] = QPair<QString, qCtagsSenseProperties>(QString( "" ), properties);
			}
		}
		
		if ( mStop )
		{
			return;
		}
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

// PROTECTED

bool qCtagsSenseIndexer::removeEntries( const QStringList& fileNames )
{
	QSqlQuery q = mSQL->query();
	QString del_sql;
	
	foreach ( QString fileName, fileNames )
	{
		QFileInfo file( fileName );
		
		if ( file.isFile() )
		{
			del_sql = QString( "DELETE FROM Files WHERE filename = '%1'" ).arg( fileName );
		}
		else
		{
			del_sql = QString( "DELETE FROM Files WHERE filename LIKE '%1/%'" ).arg( fileName );
		}
		
		if ( !q.exec( del_sql ) )
		{
			qWarning() << "Can't delete file entry for" << fileName.toLocal8Bit().constData();
			return false;
		}
		
		if ( mStop )
		{
			return false;
		}
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
		
		if ( mStop )
		{
			return false;
		}
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
		
		if ( mStop )
		{
			return false;
		}
	}
	
	tagEntries.clear();
	
	return ok;
}

int qCtagsSenseIndexer::createFileEntry( const QString& fileName, qCtagsSense::Language language )
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
	
	QString privateStr = QString("private");
	QString protectedStr = QString("protected");
	QString publicStr = QString("public");
	QString empty = QString("");
	QVariantList filesId;
	QVariantList lineNumberEntry;
	QVariantList lineNumber;
	QVariantList isFileScope;
	QVariantList isFileEntry;
	QVariantList truncateLine;
	QVariantList name;
	QVariantList kind;
	QVariantList access;
	QVariantList fileScope;
	QVariantList implementation;
	QVariantList inheritance;
	QVariantList scope_Value;
	QVariantList scope_Key;
	QVariantList signature;
	QVariantList type;
	QVariantList type_Name;
	
	while ( item != NULL )
	{
		tagEntryInfo* entry = &item->tag;
		
		if( ( privateStr == entry->extensionFields.access && currentProperty.accessFilter == qCtagsSenseProperties::All )
			|| ( protectedStr == entry->extensionFields.access &&
				( currentProperty.accessFilter == qCtagsSenseProperties::All || currentProperty.accessFilter == qCtagsSenseProperties::Protected ) )
			|| publicStr == entry->extensionFields.access
			|| entry->extensionFields.access == NULL
			|| empty == entry->extensionFields.access)
		{
			filesId << fileId;
			lineNumberEntry << entry->lineNumberEntry - 1;
			lineNumber << QVariant::fromValue( entry->lineNumber - 1 );
			isFileScope << entry->isFileScope;
			isFileEntry << entry->isFileEntry;
			truncateLine << entry->truncateLine;
			name << entry->name;
			kind << qCtagsSenseUtils::kindType( QChar( entry->kind ), entry->language );
			access << entry->extensionFields.access;
			fileScope << entry->extensionFields.fileScope;
			implementation << entry->extensionFields.implementation;
			inheritance << entry->extensionFields.inheritance;
			scope_Value << entry->extensionFields.scope[ 0 ];
			scope_Key << entry->extensionFields.scope[ 1 ];
			signature << entry->extensionFields.signature;
			type << entry->extensionFields.typeRef[ 0 ];
			type_Name << entry->extensionFields.typeRef[ 1 ];
		}
		
		item = item->next;
		
		if ( mStop )
		{
			qDebug() << "create entries bad finish !";
			return false;
		}
	}
	
	q.prepare( sql );
	q.addBindValue( filesId );
	q.addBindValue( lineNumberEntry );
	q.addBindValue( lineNumber );
	q.addBindValue( isFileScope );
	q.addBindValue( isFileEntry );
	q.addBindValue( truncateLine );
	q.addBindValue( name );
	q.addBindValue( kind );
	q.addBindValue( access );
	q.addBindValue( fileScope );
	q.addBindValue( implementation );
	q.addBindValue( inheritance );
	q.addBindValue( scope_Value );
	q.addBindValue( scope_Key );
	q.addBindValue( signature );
	q.addBindValue( type );
	q.addBindValue( type_Name );
	
	if ( !q.execBatch() )
	{
		qWarning() << "Can't create entry";
		qWarning() << q.lastError().text();
	}
	
	return true;
}

bool qCtagsSenseIndexer::indexTags( const QMap<QString, TagEntryListItem*>& tags )
{
	// remove already existing files entries
	if ( !removeEntries( tags.keys() ) )
	{
		qWarning() << "Can't remove entries for" << tags.keys();
		return false;
	}
	
	// index new ones
	foreach ( const QString& fileName, tags.keys() )
	{
		TagEntryListItem* tag = tags[ fileName ];
		
		int fileId;
		
		if(QString::fromLocal8Bit( tag->tag.language ) == "C++")
			fileId = createFileEntry( fileName, qCtagsSense::Cpp );
		else if(QString::fromLocal8Bit( tag->tag.language ) == "C#")
			fileId = createFileEntry( fileName, qCtagsSense::Csharp );
		else
			fileId = createFileEntry( fileName, qCtagsSenseUtils::languageType(QString().fromLocal8Bit(tag->tag.language)) );
		
		if ( fileId == -1 )
		{
			return false;
		}
		
		if ( !createEntries( fileId, tag ) )
		{
			return false;
		}
		
		if ( mStop )
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
	
	{
		QMutexLocker locker( &mMutex );
		
		if ( QDir::match( mFilteredSuffixes, fileName ) )
		{
			// skipping file is not an error
			ok = true;
			return 0;
		}
	}
	
	// files with no suffixes can't be parsed
	if ( QFileInfo( fileName ).suffix().isEmpty() )
	{
		ok = true;
		return 0;
	}
	
	ok = true;
	
	return createTagEntryListItem( fileName.toLocal8Bit(), NULL );
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

	foreach ( const QFileInfo& file, qCtagsSenseUtils::getFiles( dir, QStringList( "*" ), true ) )
	{
		TagEntryListItem* item = tagFileEntry( file.absoluteFilePath(), ok );
		
		if ( mStop )
		{
			ok = false;
		}
		
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

QMap<QString, TagEntryListItem*> qCtagsSenseIndexer::tagFilesEntries( const QStringList& fileNames, bool& ok )
{
	QMap<QString, TagEntryListItem*> entries;
	
	foreach ( const QString& file, fileNames )
	{
		TagEntryListItem* item = tagFileEntry( file, ok );
		
		if ( mStop )
		{
			ok = false;
		}
		
		if ( !ok )
		{
			qWarning() << "Failed to index" << file.toLocal8Bit().constData();
			return entries;
		}
		else if ( ok && item )
		{
			entries[ file ] = item;
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
		
		if ( mStop )
		{
			ok = false;
		}
		
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
	emit indexingStarted();
	
	bool changed = false;
	bool error = false;
	int value = 0;
	int total = 0;
	
	// start transaction
	QString* currentDb = new QString("");
	QTime tracker;

	forever
	{
		QMutexLocker locker( &mMutex );
		
		// copy
		QStringList fileNamesToRemove = mWaitingDeletion;
		QHash<QString, QPair<QString, qCtagsSenseProperties> > fileNamesToIndex = mWaitingIndexation;
		
		// clear
		mWaitingDeletion.clear();
		mWaitingIndexation.clear();
		
		locker.unlock();
		
		tracker.restart();
		// compute files
		foreach ( QString fileName, fileNamesToIndex.keys() )
		{
			if ( QFileInfo( fileName ).isDir() )
			{
				fileNamesToIndex.remove( fileName );
			}
			
			if ( mStop )
			{
				return;
			}
		}
		
		// do count
		total += fileNamesToRemove.count();
		total += fileNamesToIndex.count();
		
		emit indexingProgress( value, total );
		
		// deletion
		if ( !fileNamesToRemove.isEmpty() )
		{
			if ( removeEntries( fileNamesToRemove ) )
			{
				changed = true;
			}
			else if ( !error )
			{
				qWarning() << "Error while removing file";
				error = true;
			}
		}
		
		value += fileNamesToRemove.count();
		emit indexingProgress( value, total );
		
		if ( mStop )
		{
			return;
		}
		
		// indexation
		while ( !fileNamesToIndex.isEmpty() )
		{
			QPair<QString, qCtagsSenseProperties> pair;
			// compute files
			foreach ( QString fileName, fileNamesToIndex.keys() )
			{
				currentProperty = fileNamesToIndex[ fileName ].second;
				
				if( currentDb != currentProperty.connectionName )
				{
					currentDb = new QString(currentProperty.connectionName);
					mSQL = new qCtagsSenseSQL( currentProperty.connectionName );
					mSQL->database().transaction();
					
					if( mSQL->database().tables().isEmpty() )
					{
						mSQL->initializeDatabase( currentProperty.DatabaseFileName, currentProperty.infoStored );
					}
					
					setLanguageKinds( "C++", currentProperty.kindFilter.toLocal8Bit() );
				}
				
				if ( fileNamesToIndex[ fileName ].first.isNull() )
				{
					if ( indexEntry( fileName ) )
					{
						changed = true;
					}
					else if ( !error )
					{
						qWarning() << "Error while indexing files (" << fileName << ")";
						error = true;
					}
				}
				else
				{
					QMap<QString, QString> bufferEntries;
					
					bufferEntries[ fileName ] = fileNamesToIndex[ fileName ].first;
					
					if ( indexEntries( bufferEntries ) )
					{
						changed = true;
					}
					else if ( !error )
					{
						qWarning() << "Error while indexing buffers";
						error = true;
					}
				}
				
				fileNamesToIndex.remove( fileName );
					
				value++;
				emit indexingProgress( value, total );

				if ( mStop )
				{
					return;
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
			}
			
			if ( mStop )
			{
				return;
			}
		}
		
		qWarning() << "Indexation of " << total << " file(s) finished in " << tracker.elapsed() /1000.0;
		
		locker.relock();
		
		if ( mStop )
		{
			return;
		}
		else if ( !mWaitingDeletion.isEmpty() || !mWaitingIndexation.isEmpty() )
		{
			continue;
		}
		
		break;
	}
	
	if ( changed && !error )
	{
		// inform that db has changed
		emit indexingChanged();
	}
	
	emit indexingFinished();
}
