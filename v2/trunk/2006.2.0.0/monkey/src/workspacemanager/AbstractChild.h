#ifndef ABSTRACTCHILD_H
#define ABSTRACTCHILD_H
//
#include <QWidget>
#include <QFileInfo>
#include <QCloseEvent>
//
#include "MonkeyExport.h"
//
class AbstractProjectProxy;
class MonkeyEditor;
//
class Q_MONKEY_EXPORT AbstractChild : public QWidget
{
	Q_OBJECT
	Q_ENUMS( DocumentMode LayoutMode )
	//
public:
	enum DocumentMode { mNone = 0, mNa, mInsert, mOverwrite, mReadOnly } mDocument;
	enum LayoutMode { lNone = 0, lNormal, lVertical, lHorizontal } mLayout;
	// constructor
	AbstractChild()
	{ mProxy = 0;
		mDocument = mNone;
		mLayout = lNone;
		setAttribute( Qt::WA_DeleteOnClose ); }
	// return child document mode
	virtual AbstractChild::DocumentMode documentMode() const
	{ return mDocument; }
	// set the child document mode
	virtual void setDocumentMode( AbstractChild::DocumentMode m )
	{ if ( mDocument == m )
			return;
		mDocument = m;
		emit documentModeChanged( mDocument );
	}
	// return the child layout mode
	virtual AbstractChild::LayoutMode layoutMode() const
	{ return mLayout; }
	// set the child layout mode
	virtual void setLayoutMode( AbstractChild::LayoutMode m )
	{ if ( mLayout == m )
			return;
		mLayout = m;
		emit layoutModeChanged( mLayout );
	}
	// left corner widget for this child
	virtual QWidget* cornerWidget() { return 0L; }
	// add a new file to the files list that this child manage
	virtual void addFile( const QString& s )
	{ if ( mFiles.contains( s ) )
			return;
		mFiles.append( s );
		loadFile( s ); }
	// return full path + basename of file s
	virtual QString fullPathBaseName( const QString& s )
	{ QFileInfo f( s );
		if ( f.exists() )
			return QString( "%1/%2" ).arg( f.path(), f.baseName() );
		return QString(); }
	// return fullPathBaseName of the first file
	virtual QString fullPathBaseName()
	{ if ( !mFiles.count() )
			return QString();
		return fullPathBaseName( mFiles.at( 0 ) ); }
	// return files that this child manage
	virtual QStringList files() const
	{ return mFiles; }
	// set the project for this child
	virtual void setProxy( AbstractProjectProxy* p )
	{ mProxy = p; }
	// return the project of this child
	virtual AbstractProjectProxy* proxy() const
	{ return mProxy; }
	// return cursor position if available
	virtual QPoint cursorPosition() const = 0;
	// show/focus the file in child
	virtual void showFile( const QString& ) = 0;
	// the current visible / focused file
	virtual QString currentFile() const = 0;
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
	// ask to laod file
	virtual void loadFile( const QString& ) = 0;
	// ask to load these files
	virtual void loadFiles( const QStringList& l )
	{ foreach ( QString s, l )
			addFile( s ); }
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
	//
protected:
	// files list this child manage
	QStringList mFiles;
	// child project
	AbstractProjectProxy* mProxy;
	// for saving modified files
	virtual void closeEvent( QCloseEvent* e )
	{ emit closeEvent( this, e ); }
	//
signals:
	// emit when the child layout mode has changed
	void layoutModeChanged( AbstractChild::LayoutMode );
	// emit when the child document mode has changed
	void documentModeChanged( AbstractChild::DocumentMode );
	// emit when cursor position changed
	void cursorPositionChanged( const QPoint& );
	// emit when current file changed
	void currentFileChanged( const QString& );
	// emit when a file is modified
	void modifiedChanged( bool );
	// emit when undo has changed
	void undoAvailableChanged( bool );
	// emit when undo has changed
	void redoAvailableChanged( bool );
	// emit when a file paste available change
	void pasteAvailableChanged( bool );
	// emit when a file copy available change
	void copyAvailableChanged( bool );
	// emit when search/replace is available
	void searchReplaceAvailableChanged( bool );
	// emit when goto is available
	void goToAvailableChanged( bool );
	// emit when requesting search in editor
	void requestSearchReplace();
	// emit when request go to line
	void requestGoTo();
	// emit when enter child close event
	void closeEvent( AbstractChild*, QCloseEvent* );
	// emit when a child require to update workspace
	void updateWorkspaceRequested();
	// emit when an editor request toggle breakpoint
	void toggleBreakPoint( MonkeyEditor*, int, bool );
};
//
#endif // ABSTRACTCHILD_H
