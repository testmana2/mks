#include "pCompleterIndexer.h"

#include <QString>
#include <QStringList>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>

#include <pFileManager.h>
#include <qCtagsSense.h>

#include "MonkeyCore.h"
#include "MkSShellInterpreter.h"

pCompleterIndexer* pCompleterIndexer::pInstance = 0;

pCompleterIndexer::pCompleterIndexer()
{
	qDebug() << "Instance created";
	if(connect(MonkeyCore::fileManager(), SIGNAL(opened(XUPProjectItem*)), this, SLOT(opened(XUPProjectItem*))))
		qDebug() << "Indexer connected";
}

pCompleterIndexer::~pCompleterIndexer()
{
	disconnect(MonkeyCore::fileManager(), SIGNAL(opened(XUPProjectItem*)), this, SLOT(opened(XUPProjectItem*)));
}

pCompleterIndexer* pCompleterIndexer::instance()
{
	if(pInstance == 0)
		pInstance = new pCompleterIndexer();
	
	return pInstance;
}

void pCompleterIndexer::opened(XUPProjectItem* project)
{
	int result;
	const QStringList files = project->topLevelProjectSourceFiles();
	const QString projectPath = project->path();
	const QString version = project->projectSettingsValue( "QT_VERSION" );
	QString xml = MonkeyCore::interpreter()->interpret(QString("qtversion xml \"%1\"").arg( version ), &result);
	
	qDebug() << projectPath;
	qDebug() << xml;
	
	QRegExp rx(".*<Version value=\\\"([^<]*)\\\"\\s*/>.*");
	QString effectiveVersion = "";
	
	if(rx.exactMatch(xml))
	{
		effectiveVersion = rx.cap(1);
	}
	
	rx.setPattern(".*<Path value=\\\"([^<]*)\\\"\\s*/>.*");
	QString qtSrcPath;
	
	if(rx.exactMatch(xml))
	{
		//TODO check directory separator
		qtSrcPath = QDir::cleanPath((rx.cap(1)).append("/include/qt4"));
	}
	
	if(effectiveVersion != "")
	{
		effectiveVersion.replace(" ", "_");
		effectiveVersion.replace(".", "-");
		qDebug() << effectiveVersion;
		qDebug() << qtSrcPath;
		
		QString completerStorage = QDir::toNativeSeparators(QDir::cleanPath(QString("%1/%2_completer.sqlite3").arg(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).arg(effectiveVersion)));
		
		qDebug() << completerStorage;
		
		if(!QFile::exists(completerStorage))
		{
			const QStringList suffixes = QStringList()
			<< /*"*.cpp" << "*.c" << */"*.h"/* << "*.jpg" << "*.jpeg" << "*.tiff" << "*.ico" << "*.icns"
			<< "*.pri" << "*.pro" << "*.qrc" << "*.ui" << "*.ts" << "*.qm" << "*.qch" << "*.xup" << "*.mks"
			<< "*.txt" << "*.iss" << "*.api" << "*.sip" << "*.ini" << "*.css" << "*.bak" << "*.old"
			<< "*.db" << "*.so" << "*.a" << "*.desktop"  << "*.gpl"*/;
		
			qCtagsSenseProperties properties;
			
			qDebug() << "Processing properties...";
		
			properties.SystemPaths = QStringList();
			properties.FilteredSuffixes = QStringList();
			properties.UsePhysicalDatabase = true;
			properties.DatabaseFileName = completerStorage;
			
			qDebug() << "Create sense...";
			qCtagsSense* sense = new qCtagsSense(0, QString("systemCompleter"));
			
			qDebug() << "Add filters...";
			sense->setAccessFilter(qCtagsSense::Protected);
			
			qDebug() << "Add properties...";
			sense->setProperties(properties);
			
			qDebug() << "Processing files entries...";
			
			QStringList filesList;
			
			//qtSrcPath = QDir::toNativeSeparators(qtSrcPath);
			//qDebug() << qtSrcPath;
			
			recursiveFileEntries(suffixes, qtSrcPath, &filesList);
			
			qDebug() << "Finish !";
			
			foreach(QString file, filesList)
			{
				qDebug() << file;
			}
			
			sense->tagEntries(filesList);
			
			qDebug() << "Tag finish !";
		}
	}
}

void pCompleterIndexer::recursiveFileEntries(const QStringList &suffixes, const QString &path, QStringList* list)
{
	foreach(QString file, QDir(path).entryList(suffixes, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable))
	{
		list->append(QString("%1/%2").arg(path).arg(file));
	}
	
	foreach(QString subDir, QDir(path).entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
	{
		recursiveFileEntries(suffixes, QString("%1/%2").arg(path).arg(subDir), list);
	}
}
