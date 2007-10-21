#include <QFSFileEngine>
#include <string>

#include "Ctags.h"
#include "entry.h"

extern "C" void freeSTagEntryList ( sTagEntryListItem* tag );
extern "C" sTagEntryListItem* parseFile ( const char* fileName, const char* langName);
extern "C" void installLanguageMapDefaults (void);
extern "C" void initializeParsing (void);
extern "C" void freeParserResources (void);

Ctags::Ctags ()
{
	initializeParsing ();
	installLanguageMapDefaults ();	
}


Ctags::~Ctags ()
{
	freeParserResources ();
}

bool Ctags::updateRecord (FileRecord* record) //reparse file if need, or parse first time
{
	QFSFileEngine fsEng ( record->file );
	QDateTime fileTime = fsEng.fileTime ( QAbstractFileEngine::ModificationTime );

	if ( record->time >= fileTime )  //record up to date, not need parse
		return false;
	if ( record->firstTagEntry ) //exist old list, need to free it
			freeTagEntryList ( record->firstTagEntry);

	record->firstTagEntry = get_tags (record->file);
	record->time = fileTime;
	return true;
}

FileRecord* Ctags::GetTagsForFile (QString file )
{
	FileRecord* result = NULL; //need to find record for file we need, or create record
	for ( int index = 0; index< records.size(); index++)//find
	{
		if ( records[index]->file == file)
		{
			result = records[index];
		}
	}
	if ( result == NULL )  //not finded
	{
		result = new FileRecord;
		result->firstTagEntry = NULL;
		result->file = file;
		result->time = QDateTime(); //null Time
		records.append (result);
	}		
	//ok,  now we can fill record, if it's new, or update, if if's already exist
	updateRecord (result);
	return result;
}		

TagEntryListItem* Ctags::get_tags ( QString file )
{
	sTagEntryListItem* tags =  parseFile ( file.toStdString().c_str(), NULL);
	if ( tags == NULL ) 
		return NULL;
	TagEntryListItem* pItem;
	TagEntryListItem* result = NULL; // will be pointer to the first item
	//now need to convert recieved C tag entry list to the C++ tag entry list
	while ( tags != NULL) {

		if (result == NULL ) //first item
		{
			pItem = (TagEntryListItem*) calloc (1, sizeof (TagEntryListItem));
			result = pItem;			
		}
		else
		{
			pItem->next=(TagEntryListItem*) calloc (1, sizeof (TagEntryListItem));
			pItem = pItem->next ;
		}
		pItem->entry.lineNumber = tags->tag.lineNumber;
		pItem->entry.filePosition = tags->tag.filePosition;
		pItem->entry.language = tags->tag.language;
		pItem->entry.sourceFileName = tags->tag.sourceFileName;
		pItem->entry.name = tags->tag.name;
		pItem->entry.kindName = tags->tag.kindName;
		pItem->entry.kind =   tags->tag.kind;
		pItem->entry.access = tags->tag.extensionFields.access;
		pItem->entry.fileScope = tags->tag.extensionFields.fileScope;
		pItem->entry.implementation = tags->tag.extensionFields.implementation;
		pItem->entry.inheritance = tags->tag.extensionFields.inheritance;
		pItem->entry.scope[0] = tags->tag.extensionFields.scope[0];
		pItem->entry.scope[1] = tags->tag.extensionFields.scope[1];
		pItem->entry.signature = tags->tag.extensionFields.signature;
		pItem->entry.typeRef[0] = tags->tag.extensionFields.typeRef[0];
		pItem->entry.typeRef[1] = tags->tag.extensionFields.typeRef[1];

		if ( tags->tag.lineNumberEntry == BTRUE)
			pItem->entry.lineNumberEntry = true;
		else
			pItem->entry.lineNumberEntry = false;

		if ( tags->tag.isFileScope == BTRUE)
			pItem->entry.isFileScope = true;
		else
			pItem->entry.isFileScope = false;

		if ( tags->tag.isFileEntry == BTRUE)
			pItem->entry.isFileEntry = true;
		else
			pItem->entry.isFileEntry = false;

		if ( tags->tag.truncateLine == BTRUE)
			pItem->entry.truncateLine = true;
		else
			pItem->entry.truncateLine = false;

		tags = tags->next;
	}
	freeSTagEntryList ( tags );
	return result;

}


RecordsList* Ctags::GetTagsForAllFiles ()
{
// 	checkForChanges ();
// 	return &records;
}

//void Ctags::checkForChanges ()
//{
// 	QVector<FileRecord*> updatedRecords;
//  	for ( int i = 0; i< records.size(); i++)
// 	{
// 		FileRecord* precord = records.data()+i ; //pointer to records[i]
// 		bool reparsed = updateRecord (  precord ) ;  
// 		if ( reparsed )
// 			updatedRecords.append (precord);
// 	}
// 	if ( updatedRecords.size() >0 )
// 		emit  tagUpdates ( updatedRecords );
//}


void Ctags::freeTagEntryList (TagEntryListItem* item)
{
	while ( item != NULL )
	{
		TagEntry* entry = &item->entry;
		if (entry->language)
			free ( (void*)entry->language );
		if ( entry->sourceFileName)
			free ( (void*)entry->sourceFileName );
		if ( entry->name)
			free ( (void*)entry->name );
		if ( entry->kindName)
			free ( (void*)entry->kindName );
		if ( entry->access)
		{
			free ( (void*)entry->access );
		}
		if ( entry->fileScope)
		{
			free ( (void*)entry->fileScope );
		}
		if ( entry->implementation)
		{
			free ( (void*)entry->implementation  );
		}
		if ( entry->inheritance)
		{
			free ( (void*)entry->inheritance );
		}
		if ( entry->scope[0])
		{
			free ( (void*)entry->scope[0] );
		}
		if ( entry->scope[1])
		{
			free (  (void*)entry->scope[1] );
		}
		if ( entry->typeRef[0])
		{
			free ( (void*)entry->typeRef[0] );
		}
		if ( entry->typeRef[1])
		{
			free ( (void*)entry->typeRef[1] );
		}
		if ( entry->signature)
		{
			free ( (void*)entry->signature );
		}
		TagEntryListItem* temp = item->next;
		free ( item  );
		item = temp;
	}
}