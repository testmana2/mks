#ifndef TEXTCHILD_H
#define TEXTCHILD_H
//
#include "AbstractChild.h"
//
#include <QUrl>
//
//class Q_MONKEY_EXPORT AbstractChild;
class Workspace;
class MainWindow;
class HelpWindow;
//
class QtAssistantChild : public AbstractChild
{
	Q_OBJECT
	//
public:
	static QtAssistantChild* self( Workspace* = 0, MainWindow* = 0 );
	// browsers list
	QList<HelpWindow*> browsersList() const;
	// return files that this child manage
	virtual QStringList files() const;
	// return cursor position if available
	virtual QPoint cursorPosition() const;
	// show the current file in child
	virtual void showFile( const QString& );
	// the current visible / focused file
	virtual QString currentFile() const;
	// return the current file modified flag
	virtual bool isModified() const;
	// return the current file undo flag
	virtual bool isUndoAvailable() const;
	// return the current file redo flag
	virtual bool isRedoAvailable() const;
	// return the current file paste available
	virtual bool isPasteAvailable() const;
	// return the current file copy available
	virtual bool isCopyAvailable() const;
	// return the modified state of file
	virtual bool isModified( const QString& ) const;
	// ask to save file
	virtual void saveFile( const QString& );
	// ask to save all files
	virtual void saveFiles();
	// ask to laod file
	virtual void loadFile( const QString& );
	// ask to print this file
	virtual void printFile( const QString& );
	// ask to quick print this file
	virtual void quickPrintFile( const QString& );
	// undo
	virtual void undo();
	// redo
	virtual void redo();
	// cut
	virtual void cut();
	// copy
	virtual void copy();
	// paste
	virtual void paste();
	// search/replace
	virtual void searchReplace();
	// go to
	virtual void goTo();
	// search/replace available
	virtual bool isSearchReplaceAvailable() const;
	// goto available
	virtual bool isGoToAvailable() const;
	// print available
	virtual bool isPrintAvailable() const;
	//
private:
	QtAssistantChild( Workspace*, MainWindow* );
	//
	static QtAssistantChild* mSelf;
	Workspace* mWorkspace;
	MainWindow* mMain;
	//
protected:
	virtual void closeEvent( QCloseEvent* );
	//
private slots:
	void helpWindow_destroyed( QObject* );
	void showLink( const QString& = QString() );
	void showLink( const QUrl& );
	//
};
//
#endif // TEXTCHILD_H
