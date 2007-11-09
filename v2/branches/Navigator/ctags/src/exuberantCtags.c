#include "exuberantCtags.h"
#include "entry.h"

#include "debug.h"
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
//tagEntryListItem* lastTagEntry;   //for adding entryes 

// get extern vars
extern parserDefinition** LanguageTable;

extern tagEntryListItem* parseLanguageFile ( const char* fileName, const char* langName)
{
	firstTagEntry = NULL; // generate new list
	langType lang; 
	if ( (langName != NULL ) && (strlen ( langName) != 0 ))
		lang = getNamedLanguage ( langName );
	else
		lang = getFileLanguage ( fileName) ;
	if ( lang  <=0)
	{
		printf ( "Will not parse %s\n",fileName);
		return NULL;
	}
	const parserDefinition* const language = LanguageTable [lang];
	if ( language == NULL)
		return NULL;
	if (!fileOpen (fileName, lang))
		return NULL;
	
	unsigned int passCount = 0;
	boolean retried = FALSE;
	if (language->parser != NULL)
		language->parser ();
	else if (language->parser2 != NULL)
		do
		{
			retried = language->parser2 (++passCount);
		}while ( retried);
	return firstTagEntry;
}
