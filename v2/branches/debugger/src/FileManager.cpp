#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMdiSubWindow>
#include <QVariant>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

#include <Qsci/qsciscintilla.h>

#include <QDebug>
#include "FileManager.h"

FileManager::FileManager(QObject* parent, QMdiArea* mdiArea)
  : QObject( parent ),
	mMdiArea (mdiArea)
{
}

FileManager::~FileManager()
{	
}

void FileManager::gotoFileLine (const QString& fileName, int line)
{
	QsciScintilla* editor = NULL;
	foreach (QMdiSubWindow* window, mMdiArea->subWindowList())
	{
		QsciScintilla* sci = dynamic_cast<QsciScintilla*>(window->widget());
		if (sci->property ("fileName").toString() == fileName)
		{
			editor = sci;
			break;
		}
	}
	if (NULL == editor)
	{
		editor = openFile (fileName);
	}
	if (NULL != editor)
	{
		editor->setCursorPosition (line-1, 0);
	}
}

QsciScintilla* FileManager::openFile (const QString& fileName)
{
	QsciScintilla* sci = new QsciScintilla();
	QApplication::setOverrideCursor( Qt::WaitCursor );
	
	// open file
	QFile f( fileName );
	if ( !f.open( QFile::ReadOnly ) )
	{
		QMessageBox::critical (NULL, tr("Open file"), tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg( f.errorString()));
		return NULL;
	}
	
	// remember filename
	sci->setProperty("fileName", fileName);
	sci->setWindowTitle (QFileInfo (fileName).fileName());

	// configure
	sci->setMarginLineNumbers (1, true);
	sci->setMarginWidth (1, 50);
	sci->setIndentationWidth (4);
	sci->setCaretLineVisible (true);
	// set lexer and apis
	//setLexer( pMonkeyStudio::lexerForFileName( fileName ) );

	// load file
	QString datas = f.readAll();
	sci->setText( datas );
	
	mMdiArea->addSubWindow (sci);
	sci->showMaximized();

	QApplication::restoreOverrideCursor();
	
	return sci;
}
