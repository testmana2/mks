#include "MakefileProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include <ProjectTypesIndex.h>
#include <pMonkeyStudio.h>
#include <pFileManager.h>
#include <CLIToolPlugin.h>
#include <UIMain.h>

#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

MakefileProjectItem::MakefileProjectItem()
	: XUPProjectItem()
{
	mDomElement = mDocument.createElement("project");
}

QString MakefileProjectItem::projectType() const
{
	return PLUGIN_NAME;
}

QStringList MakefileProjectItem::autoActivatePlugins() const
{
	return QStringList("GNUMake");
}

void MakefileProjectItem::installCommands()
{
	QStringList targets;
	QFile file(fileName());
	file.open(QIODevice::ReadOnly);
	QRegExp targetRex("^([\\w\\-\\d]+):.*");
	while (! file.atEnd())
	{
		QString line = file.readLine();
		int pos = targetRex.indexIn(line);
		if (-1 != pos)
			targets << targetRex.cap(1);
	}
	CLIToolPlugin* make = MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "GNUMake" );
	if (! make)
	{
		QMessageBox::critical(MonkeyCore::mainWindow(), tr("Makefile Project"),
							  tr("Can't build Makefile projects. GNUMake plugin not enabled"));
		return;
	}
	
	pCommand baseCmd = make->command();
	
	baseCmd.setProject( this );
	baseCmd.setSkipOnError( false );
	baseCmd.setTryAllParsers( true );
	
	QString makeFileArg = QString("-f %1 ").arg(QFileInfo(fileName()).fileName());
	
	foreach(QString target, targets)
	{
		baseCmd.setText( target );
		baseCmd.setArguments( makeFileArg + target );
		addCommand( baseCmd, "mBuilder" );
	}
}

void MakefileProjectItem::addFiles( const QStringList&, XUPItem*)
{
	QMessageBox::information (MonkeyCore::mainWindow(), 
							  tr("Makefile project"),
							  tr("Adding files to the Makefile project does not make sense. "
							    "Press 'Edit current project...' for edit your Makefile"));
}

void MakefileProjectItem::removeItem( XUPItem* )
{
	Q_ASSERT(0);
}

bool MakefileProjectItem::open( const QString& fileName, const QString& codec )
{
	mFileName = fileName;
	mCodec = codec;
	QString parentDirName = QFileInfo(QFileInfo(fileName).path()).fileName();
	setAttribute("name",
				 parentDirName + "/" + QFileInfo(fileName).fileName()); // "directoryname/Makefile"
	return true;
}

bool MakefileProjectItem::edit()
{
	MonkeyCore::fileManager()->openFile(fileName(), codec());
	return false; /* not saved */
}

QString MakefileProjectItem::iconsPath() const
{
	return ":/pyqtitems";
}

Pair_String_StringList_List MakefileProjectItem::sourceFileNamePatterns() const
{
	return Pair_String_StringList_List();
}
