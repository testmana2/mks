#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
//
#include "WorkspacePlugin.h"
#include "gccParser.h"
//
#include <QPointer>

#include <QDockWidget>
#include <QModelIndex>
//
class UIMessageBox;
//
class MessageBox : public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
public:
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	//
public slots:
	void messageBox( const QString& );
	void clearMessageBox();
	void dataAvailable( const QString& );
	void showListBox();
	void showConsole();
	//
private slots:
	void makeGoto();
	//
private:
	QPointer<UIMessageBox> mMessageBox;
	gccParser* parser; //maybe will be more than one parser in the future
	QVector <errStruct*> errPointers;  // pointers to errors currently on messagebox
	int errorsCount; 
	int warningsCount;
	//
signals:
	void gotoLine( const QString&, int );
	//
};
//
#endif // MESSAGEBOX_H
