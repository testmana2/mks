#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
//
#include "WorkspacePlugin.h"
#include "ConsoleCommandParser.h"
//
#include "MonkeyActGroup.h"
#include "MonkeyAction.h"
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
	void newErrorAvailable( const ConsoleCommandParser::Message& );
	//
private slots:
	void makeGoto();
	//
private:
	QPointer<UIMessageBox> mMessageBox;
    MonkeyActGroup* actGroup;
    MonkeyAction* aShowListBox;
    MonkeyAction* aShowConsole;
	//
signals:
	void gotoFile( const QString&, const QPoint& );
	//
};
//
#endif // MESSAGEBOX_H
