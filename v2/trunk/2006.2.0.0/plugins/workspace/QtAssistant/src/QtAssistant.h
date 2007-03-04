#ifndef QTASSISTANT_H
#define QTASSISTANT_H
//
#include "WorkspacePlugin.h"
//
class QtAssistant : public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
public:
	virtual ~QtAssistant();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	//
    virtual void saveCodeCoverage(const QString &name);
};
//
#endif // QTASSISTANT_H
