#include "entry.h"

/*procedure will return list for  file. Every time new list will be generated. For every generated list memory must be freed using freeTagEntryList
*/

extern TagEntryListItem* parseFile ( const char* fileName, const char* langName);

extern void freeSTagEntryList ( TagEntryListItem* tagListItem );