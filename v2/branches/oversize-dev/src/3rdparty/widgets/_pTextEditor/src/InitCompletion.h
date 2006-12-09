/********************************************************************************************************
 * PROGRAM	  : 
 * DATE - TIME  : lundi 10 avril 2006 - 22:28
 * AUTHOR	   : Anacr0x ( fred.julian at gmail.com )
//  * FILENAME	 : InitCompletion.h
 * LICENSE	  : GPL
 * COMMENTARY   : Initialisation class for the icomplete's autocompletion
 ********************************************************************************************************/
#ifndef _INIT_COMPLETION_H
#define _INIT_COMPLETION_H

#include <QStringList>
#include <QList>
#include <QObject>
#include <QFile>

class Tree;

struct Tag
{
	QString name;
	QString parameters;
	QString longName;

	bool isFunction;
};

class InitCompletion : public QObject
{
	Q_OBJECT

public:
	InitCompletion (QObject *parent = 0) : QObject(parent), includes(NULL) {};

	void setTempFilePath (const QString &Path);
	void setCtagsCmdPath (const QString &cmdPath);
	void addIncludes (QStringList includesPath);
	void addMacros (const QStringList &macro);
	QList<Tag> startParse(const QString &text);

	/*
		* @param: filename is a name like "string.h"
		* @return: the file descriptor (fd) and stores
		*          "/usr/include/string.h" in fullname
	*/
	QFile* getFiledescriptor(const QString &filename, QString &fullname);

	QString tagsFilePath;
	QString tagsIncludesPath;
	QString ctagsCmdPath;
	QString sourceFilePath;
	QString smallTagsFilePath;

private:
	/* creates a simple hash for all #include lines of a line */
	long calculateHash(const QString &buf);
	/* forks and executes ctags to rebuild a tags file
	* storing cache_value in the tags file */
	void buildTagsFile(long cache_value);

	Tree *includes;

	QStringList cpp_includes;
	QStringList cpp_macros;
};

#endif
