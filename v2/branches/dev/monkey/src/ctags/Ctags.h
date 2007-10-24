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

struct FileRecord {
	sTagEntryListItem*  firstTagEntry;
	QDateTime time;
};

class Ctags: public QObject, public QSingleton<Ctags>
{
Q_OBJECT
private:
	/* ClassBrouser and other objects will use pointers to the "QString file" of FileRecord for economy of memory, 
	*so, if record created it must NEVER be deleted while monkeyDS working.
	If need free some memory, possible to free TagEntryList and set time to QDateTime::null
	*/
	QHash<QString, FileRecord*> fileRecords;  //records for already parsed files

	//reparse file if need, or parse first time
	//returns true if record was parsed/reparsed, or false, if record up to date
	//record for file must already exist in the hash
	bool updateFileRecord (QString file);

	//internal function for call exuberant ctags
	sTagEntryListItem* get_tags ( QString file );
	void freeTagEntryList (sTagEntryListItem*);
public:
	Ctags ();
	~Ctags ();

	FileRecord* GetTagsForFile (QString);
//	RecordsList* GetTagsForAllFiles ();
 	//void checkForChanges ();   //not realised yet
signals:
	// void tagUpdates ( QVector<FileRecord*> ); //not realised yet
};

#endif //CTAGS_H
