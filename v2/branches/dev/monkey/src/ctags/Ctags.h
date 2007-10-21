#ifndef CTAGS_H
#define CTAGS_H	
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QList>
#include<QVector>
#include "QSingleton.h"
#include "entry.h"


/*record in the table, in wich are file names and pointers to the tag list for it*/

/*C++ analog of sTagEntryInfo structure in the entry.h*/
/*We need to create new structure, because there are problems with using ctags native sTagEntryInfo, C language haven't bool type */
struct TagEntry {
	bool     lineNumberEntry;  /* pattern or line number entry */
	unsigned long lineNumber;     /* line number of tag */
	fpos_t      filePosition;     /* file position of line containing tag */
	const char* language;         /* language of source file */
	bool     isFileScope;      /* is tag visibile only within source file? */
	bool     isFileEntry;      /* is this just an entry for a file name? */
	bool     truncateLine;     /* truncate tag line at end of tag name? */
	const char *sourceFileName;   /* name of source file */
	const char *name;             /* name of the tag */
	const char *kindName;         /* kind of tag */
	char        kind;             /* single character representation of kind */
	const char* access;
	const char* fileScope;
	const char* implementation;
	const char* inheritance;
	const char* scope [2];    /* value and key */
	const char* signature;
	/* type (union/struct/etc.) and name for a variable or typedef. */
	const char* typeRef [2];  /* e.g., "struct" and struct name */
} ;

/*C++ analog of sTagEntryListItem structure from entry.h*/
struct TagEntryListItem {
	TagEntry entry;
	struct TagEntryListItem *next;
	}; 

struct FileRecord {
	QString file;
	TagEntryListItem*  firstTagEntry;
	QDateTime time;
	};

typedef QVector<FileRecord*> RecordsList;

class Ctags: public QObject, public QSingleton<Ctags>
{
Q_OBJECT
private:
	/* ClassBrouser and other objects will use pointers to the "QString file" of FileRecord for economy of memory, 
	*so, if record created it must NEVER be deleted while monkeyDS working.
	If need free some memory, possible to free TagEntryList and set time to QDateTime::null
	*/
	RecordsList records;  //records for already parsed files

	//reparse file if need, or parse first time
	//returns true if record was parsed/reparsed, or false, if record up to date
	bool updateRecord (FileRecord* fileRecord); 

	//internal function for call exuberant ctags
	TagEntryListItem* get_tags ( QString file );
	void freeTagEntryList (TagEntryListItem*);
	static Ctags* mSelf;	
public:
	Ctags ();
	~Ctags ();
	FileRecord* GetTagsForFile (QString);
	RecordsList* GetTagsForAllFiles ();
 	//void checkForChanges ();   //not realised yet
signals:
	// void tagUpdates ( QVector<FileRecord*> ); //not realised yet
};

#endif //CTAGS_H
