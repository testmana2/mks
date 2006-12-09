/********************************************************************************************************
 * PROGRAM	  : 
 * DATE - TIME  : lundi 10 avril 2006 - 22:28
 * AUTHOR	   : Anacr0x ( fred.julian at gmail.com )
 * FILENAME	 : InitCompletion.h
 * LICENSE	  : GPL
 * COMMENTARY   : Initialisation class for the icomplete's autocompletion
 ********************************************************************************************************/
#include "InitCompletion.h"
#include "./QIComplete/parse.h"
#include "./QIComplete/readtags.h"
#include "./QIComplete/tree.h"

#include <QDir>
#include <QProcess>

void InitCompletion::setTempFilePath (const QString &Path)
{
	sourceFilePath = Path + '/' + "temp_source_file";
	tagsIncludesPath = Path + '/' + "tags_includes";
	tagsFilePath = Path + '/' + "tags";
	smallTagsFilePath = Path + '/' + "small-tags";
}

void InitCompletion::addIncludes (QStringList includesPath)
{
	QDir dir;
	QFileInfo fileInfo;
	QFileInfoList list;

	for(int i = 0; i < includesPath.size(); i++)
	{
		dir.setPath(includesPath[i]);
		if (dir.exists() && !cpp_includes.contains(includesPath[i]))
			cpp_includes << includesPath[i];

		list = dir.entryInfoList();
		for (int j = 0; j < list.size(); ++j)
		{
			fileInfo = list[j];
			if(fileInfo.isDir() && fileInfo.fileName ()[0]!='.' && !fileInfo.isSymLink())
				includesPath.insert(i+1, fileInfo.absoluteFilePath());
		}
	}
}

void InitCompletion::addMacros (const QStringList &macro)
{
	cpp_macros = macro;
}

/* creates a simple hash for all #include lines of a line */
long InitCompletion::calculateHash(const QString &buf)
{
	long res = 0;

	/* find include'ed files and add them to the cache */
	QRegExp rx("#\\s*include\\s*(<[^>]+>|\"[^\"]+\")");
	QString include;

	int pos = 0;
	while ((pos = rx.indexIn(buf, pos)) != -1)
	{
		include = rx.cap(1);

		include.remove(0, 1);
		include.remove(include.length() - 1, 1);

		for (int i = 0; i < include.length(); i++)
			res += (i + 1) * include[i].toAscii();

		pos += rx.matchedLength();
	}

	return res;
}

/* forks and executes ctags to rebuild a tags file
 * storing cache_value in the tags file */
void InitCompletion::buildTagsFile(long cache_value)
{
	// create an tags_includes file to feed the exuberant-ctags program with filenames
	if(includes)
		includes->freeTree();

	includes = Tree::buildIncludeTree(NULL, sourceFilePath);
	QFile taglist(tagsIncludesPath);
	if (includes && taglist.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		includes->writeTreeToFile(taglist);
		taglist.close();

		/* save the cache information in the tags file */
		QFile tags(tagsFilePath);
		if (tags.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QString newLine;
#ifdef _WIN32

			newLine = "\r\n";
#else

			newLine = "\n";
#endif
			QString head = "!_TAG_FILE_FORMAT	2	/extended format; --format=1 will not append ;\" to lines/" + newLine +
			               "!_TAG_FILE_SORTED	1	/0=unsorted, 1=sorted, 2=foldcase/" + newLine +
			               "!_TAG_PROGRAM_AUTHOR	Darren Hiebert	/dhiebert@users.sourceforge.net/" + newLine +
			               "!_TAG_PROGRAM_NAME	Exuberant Ctags	//" + newLine +
			               "!_TAG_PROGRAM_URL	http://ctags.sourceforge.net	/official site/" + newLine +
			               "!_TAG_PROGRAM_VERSION	5.5.4	//" + newLine +
			               "!_TAG_CACHE\t" + QString::number(cache_value) + newLine;
			tags.write(head.toAscii());
			tags.close();
		}

		QString command =
				ctagsCmdPath + " -f \"" + tagsFilePath +
				"\" --append --language-force=c++ --fields=afiKmsSzn --c++-kinds=cdefgmnpstuvx -L \""
				+ tagsIncludesPath +'\"';
		for (int i = 0; i < cpp_macros.size(); i++)
			command += " -I " + cpp_macros[i];

		// I don't process any user input, so system() should be safe enough
		if (QProcess::execute(command))
			qDebug("exuberant-ctags failed");

		includes->freeTree();
		taglist.remove();
	}
}

QList<Tag> InitCompletion::startParse(const QString &text)
{
	Tree::parent = this;

	long cache_value = calculateHash(text);

	QFile source_file(sourceFilePath);
	if(source_file.open(QIODevice::WriteOnly))
	{
		source_file.write(text.toLocal8Bit());
		source_file.close();
	}

	/* automatic cache mode */
	/* we save a hash value for all included files from the current file
	   in the tags file, if it matches the hash of the current buffer, nothing
	   has changed, and reuse the existing tags file */

	bool build_cache = true;
	QFile fCache(tagsFilePath);
	if(fCache.open(QIODevice::ReadOnly))
	{
		QString newLine;
#ifdef _WIN32

		newLine = "\r\n";
#else

		newLine = "\n";
#endif

		QString pattern = "!_TAG_CACHE\t" + QString::number(cache_value) + newLine;
		for (int i=0; i<10; i++)
			if (fCache.readLine() == pattern)
				build_cache = false;

		fCache.close();
	}
	if (build_cache)
		buildTagsFile(cache_value);

	/* real parsing starts here */
	Expression exp;
	Scope sc;
	Parse parse(ctagsCmdPath, tagsFilePath, sourceFilePath, smallTagsFilePath);
	exp = parse.parseExpression(text, &sc);
	if (exp.access == ParseError)
		return QList <Tag> ();

	/* we have all relevant information, so just list the entries */
	if (exp.access == ParseError)
		return QList <Tag> ();

	source_file.remove();

	return Tree::findEntries(&exp, &sc);
}

void InitCompletion::setCtagsCmdPath (const QString &cmdPath)
{
	ctagsCmdPath = cmdPath;
}

QFile* InitCompletion::getFiledescriptor(const QString &filename, QString &fullname)
{
	QFile *fd = new QFile();

	/* absolute path name */
	if (QFileInfo(filename).isAbsolute())
	{
		fd->setFileName(filename);
		if(fd->open(QIODevice::ReadOnly))
		{
			fullname = QFileInfo(filename).canonicalFilePath();
			return fd;
		}
	}
	else
	{
		/* relative pathname */
		for (int i = 0; i < cpp_includes.size(); i++)
		{
			fd->setFileName(cpp_includes.at(i) + '/' + filename);
			if(fd->open(QIODevice::ReadOnly))
			{
				fullname = QFileInfo(cpp_includes.at(i) + '/' + filename).canonicalFilePath();
				return fd;
			}
		}
	}

	// Nothing was found
	fd->close();
	delete fd;
	return NULL;
}
