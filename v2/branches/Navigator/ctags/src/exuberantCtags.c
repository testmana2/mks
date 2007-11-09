#include "exuberantCtags.h"

#include "keyword.h"
#include "main.h"
#include "options.h"
#include "read.h"
#include "routines.h"

#include <string.h> // strlen
#include <stdio.h> // printf
//#include <fopen.h>

//  Global pointers to entrys in the list
tagEntryListItem* firstTagEntry;  //for return it as result
tagEntryListItem* lastTagEntry;   //for adding entryes 
langType lang;
parserDefinition* language;
unsigned int passCount;
boolean retried;

// get extern vars
extern parserDefinition** LanguageTable;

void addEntryToList (const tagEntryInfo *const tag) 
{
	tagEntryListItem* newEntry = calloc ( 1,sizeof (tagEntryListItem ));
	// copying entry
	memcpy ( &newEntry->tag, tag, sizeof (tagEntryInfo));
	if ( tag->language )
	{
		newEntry->tag.language = malloc ( strlen (tag->language) + 1 );
		strcpy ( (void*)newEntry->tag.language, tag->language );
	}
	if ( tag->sourceFileName )
	{
		newEntry->tag.sourceFileName = malloc ( strlen (tag->sourceFileName) + 1 );
		strcpy ( (void*)newEntry->tag.sourceFileName, tag->sourceFileName ) ;
	}
	if ( tag->name)
	{
		newEntry->tag.name = malloc ( strlen ( tag->name ) + 1 );
		strcpy ( (void*)newEntry->tag.name, tag->name );
	}
	if ( tag->kindName)
	{
		newEntry->tag.kindName = malloc ( strlen (tag->kindName ) + 1 );
		strcpy ( (void*)newEntry->tag.kindName, tag->kindName );
	}
	if ( tag->extensionFields.access)
	{
		newEntry->tag.extensionFields.access = malloc ( strlen (tag->extensionFields.access ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.access, tag->extensionFields.access );
	}
	if ( tag->extensionFields.fileScope)
	{
		newEntry->tag.extensionFields.fileScope = malloc ( strlen (tag->extensionFields.fileScope ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.fileScope, tag->extensionFields.fileScope );
	}
	if ( tag->extensionFields.implementation)
	{
		newEntry->tag.extensionFields.implementation = malloc ( strlen (tag->extensionFields.implementation ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.implementation, tag->extensionFields.implementation );
	}
	if ( tag->extensionFields.inheritance)
	{
		newEntry->tag.extensionFields.inheritance = malloc ( strlen (tag->extensionFields.inheritance ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.inheritance, tag->extensionFields.inheritance );
	}
	if ( tag->extensionFields.scope[0])
	{
		newEntry->tag.extensionFields.scope[0] = malloc ( strlen (tag->extensionFields.scope[0] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.scope[0], tag->extensionFields.scope[0] );
	}
	if ( tag->extensionFields.scope[1])
	{
		newEntry->tag.extensionFields.scope[1] = malloc ( strlen (tag->extensionFields.scope[1] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.scope[1], tag->extensionFields.scope[1] );
	}
	if ( tag->extensionFields.typeRef[0])
	{
		newEntry->tag.extensionFields.typeRef[0] = malloc ( strlen (tag->extensionFields.typeRef[0] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.typeRef[0], tag->extensionFields.typeRef[0] );
	}
	if ( tag->extensionFields.typeRef[1])
	{
		newEntry->tag.extensionFields.typeRef[1] = malloc ( strlen (tag->extensionFields.typeRef[1] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.typeRef[1], tag->extensionFields.typeRef[1] );
	}
	if ( tag->extensionFields.signature)
	{
		newEntry->tag.extensionFields.signature = malloc ( strlen (tag->extensionFields.signature ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.signature, tag->extensionFields.signature );
	}
	if ( firstTagEntry == NULL )//it's will be first entry
		firstTagEntry = newEntry;
	else //it's not a first entry
		lastTagEntry->next = newEntry;
	lastTagEntry = newEntry;
}

extern tagEntryListItem* parseLanguageFile ( const char* fileName, const char* langName)
{
	firstTagEntry = NULL; // generate new list
	if ( (langName != NULL ) && (strlen ( langName) != 0 ))
		lang = getNamedLanguage ( langName );
	else
		lang = getFileLanguage ( fileName) ;
	if ( lang  <=0)
	{
		printf ( "Will not parse %s\n",fileName);
		return NULL;
	}
	language = LanguageTable [lang];
	if ( language == NULL)
		return NULL;
	if (!fileOpen (fileName, lang))
		return NULL;
	
	passCount = 0;
	retried = FALSE;
	if (language->parser != NULL)
		language->parser ();
	else if (language->parser2 != NULL)
		do
		{
			retried = language->parser2 (++passCount);
		}while ( retried);
	return firstTagEntry;
}
