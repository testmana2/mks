#ifndef QTASSISTANT_H
#define QTASSISTANT_H
//
#include "WorkspacePlugin.h"
//
#include <QPointer>
//
class MainWindow;
class QtAssistantChild;
//
class QtAssistant : public QObject, public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
public:
	virtual ~QtAssistant();
	virtual void initialize( Workspace* );
	virtual QString name() const;
	virtual QString description() const;
	virtual bool install();
	virtual bool uninstall();
	//
private:
	QPointer<MainWindow> mMain;
	QPointer<QtAssistantChild> mChild;
	//
};
//
#endif // QTASSISTANT_H
