/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pAbstractChild.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PABSTRACTCHILD_H
#define PABSTRACTCHILD_H

#include <QWidget>
#include <QFileInfo>
#include <QTextCodec>

#include "MonkeyExport.h"

class pAbstractProjectProxy;

class Q_MONKEY_EXPORT pAbstractChild : public QWidget
{
	Q_OBJECT
	Q_ENUMS( DocumentMode LayoutMode )

public:
	enum DocumentMode { mNone = 0, mNa, mInsert, mOverwrite, mReadOnly } mDocument;
	enum LayoutMode { lNone = 0, lNormal, lVertical, lHorizontal } mLayout;

	// constructor
	pAbstractChild()
	{
		mProxy = 0;
		mDocument = mNone;
		mLayout = lNone;
		setAttribute( Qt::WA_DeleteOnClose );
	}

public slots:
	// return child document mode
	virtual pAbstractChild::DocumentMode documentMode() const
	{ return mDocument; }

	// set the child document mode
	virtual void setDocumentMode( pAbstractChild::DocumentMode m )
	{
		if ( mDocument == m )
			return;
		mDocument = m;
		emit documentModeChanged( mDocument );
	}

	// return the child layout mode
	virtual pAbstractChild::LayoutMode layoutMode() const
	{ return mLayout; }

	// set the child layout mode
	virtual void setLayoutMode( pAbstractChild::LayoutMode m )
	{
		if ( mLayout == m )
			return;
		mLayout = m;
		emit layoutModeChanged( mLayout );
	}

	// set window title
	virtual void setWindowTitle( const QString& s )
	{
		QWidget::setWindowTitle( QFileInfo( s.isEmpty() ? currentFile() : s ).fileName() );
	}

	// return files that this child manage
	virtual QStringList files() const
	{ return mFiles; }

	// set the project for this child
	virtual void setProxy( pAbstractProjectProxy* p )
	{ mProxy = p; }

	// return the project of this child
	virtual pAbstractProjectProxy* proxy() const
	{ return mProxy; }

	// return cursor position if available
	virtual QPoint cursorPosition() const = 0;
	// show/focus the file in child
	virtual void showFile( const QString& ) = 0;
	// the current visible / focused file
	virtual QString currentFile() const = 0;
	// the current visible / focused file name ( without path )
	virtual QString currentFileName() const = 0;
	// return the current file modified flag
	virtual bool isModified() const = 0;
	// return the current file undo flag
	virtual bool isUndoAvailable() const = 0;
	// undo
	virtual void undo() = 0;
	// return the current file redo flag
	virtual bool isRedoAvailable() const = 0;
	// redo
	virtual void redo() = 0;
	// cut
	virtual void cut() = 0;
	// copy
	virtual void copy() = 0;
	// paste
	virtual void paste() = 0;
	// search/replace in the current child
	virtual void searchReplace() = 0;
	// go to in the current child
	virtual void goTo() = 0;
	// go to position for file and highlight line according to bool
	virtual void goTo( const QString&, const QPoint&, bool = false ) = 0;
	// return the current file copy available
	virtual bool isCopyAvailable() const = 0;
	// return the current file paste available
	virtual bool isPasteAvailable() const = 0;
	// return is search/replace is available
	virtual bool isSearchReplaceAvailable() const = 0;
	// return is goto is available
	virtual bool isGoToAvailable() const = 0;
	// return the modified state of file
	virtual bool isModified( const QString& ) const = 0;
	// return if print is available
	virtual bool isPrintAvailable() const = 0;
	// ask to save file
	virtual void saveFile( const QString& ) = 0;
	// ask to save the current file
	virtual void saveCurrentFile()
	{ saveFile( currentFile() ); }
	// ask to save all files
	virtual void saveFiles() = 0;
	// ask to load file
	virtual void openFile( const QString&, const QPoint& = QPoint(), QTextCodec* = 0 ) = 0;
	// ask to load these files
	virtual void openFiles( const QStringList& l )
	{
		foreach ( QString s, l )
			openFile( s, QPoint() );
	}
	// ask to close file
	virtual void closeFile( const QString& ) = 0;
	// ask to close the current file
	virtual void closeCurrentFile()
	{ closeFile( currentFile() ); }
	// ask to close all files
	virtual void closeFiles() = 0;
	// ask to print this file
	virtual void printFile( const QString& ) = 0;
	// ask to quick print this file
	virtual void quickPrintFile( const QString& ) = 0;
	// ask to print the current file
	virtual void printCurrentFile()
	{ printFile( currentFile() ); }
	// ask to quick print the current file
	virtual void quickPrintCurrentFile()
	{ quickPrintFile( currentFile() ); }

protected:
	// files list this child manage
	QStringList mFiles;
	// child project
	pAbstractProjectProxy* mProxy;

signals:
	// emit when a file is opened
	void fileOpened( const QString&, pAbstractProjectProxy* ); // ok
	// emit when a file is closed
	void fileClosed( const QString&, pAbstractProjectProxy* ); // ok
	// emit when the child layout mode has changed
	void layoutModeChanged( pAbstractChild::LayoutMode );
	// emit when the child document mode has changed
	void documentModeChanged( pAbstractChild::DocumentMode );
	// emit when cursor position changed
	void cursorPositionChanged( const QPoint& ); // ok
	// emit when current file changed
	void currentFileChanged( const QString& );
	// emit when a file is modified
	void modifiedChanged( bool ); // ok
	// emit when undo has changed
	void undoAvailableChanged( bool ); // ok
	// emit when undo has changed
	void redoAvailableChanged( bool ); // ok
	// emit when a file paste available change
	void pasteAvailableChanged( bool ); // ok
	// emit when a file copy available change
	void copyAvailableChanged( bool ); // ok
	// emit when search/replace is available
	//void searchReplaceAvailableChanged( bool );
	// emit when goto is available
	//void goToAvailableChanged( bool );
	// emit when requesting search in editor
	//void requestSearchReplace();
	// emit when request go to line
	//void requestGoTo();
	// emit when a child require to update workspace
	//void updateWorkspaceRequested();

};

#endif // PABSTRACTCHILD_H
