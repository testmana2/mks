#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
//
#include "WorkspacePlugin.h"
#include "ConsoleCommandParser.h"
//
#include <QPointer>
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
	// parser
	void newErrorAvailable( const ConsoleCommandParser::ErrorInfos& );
	//
private slots:
	void makeGoto();
	//
private:
	QPointer<UIMessageBox> mMessageBox;
	//
signals:
	void gotoLine( const QString&, int );
	//
};
//
#endif // MESSAGEBOX_H
