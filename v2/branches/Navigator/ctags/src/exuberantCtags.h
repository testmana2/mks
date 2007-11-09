#ifndef EXUBERANTCTAGS_H
#define EXUBERANTCTAGS_H

#include "general.h"
#include "entry.h"

typedef struct  TagEntryListItem {
	tagEntryInfo tag;
	struct TagEntryListItem* next;
} tagEntryListItem; 

//
void addEntryToList (const tagEntryInfo *const tag) ;

// procedure will return list for  file. Every time new list will be generated. For every generated list memory must be freed using Ctags::freeTagEntryList
extern tagEntryListItem* parseLanguageFile ( const char* fileName, const char* langName);

#endif // EXUBERANTCTAGS_H
