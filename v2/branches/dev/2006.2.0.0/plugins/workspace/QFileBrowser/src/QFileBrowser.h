#ifndef QFILEBROWSER_H
#define QFILEBROWSER_H

#include "WorkspacePlugin.h"

class QFileBrowser : public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )

public:
	virtual ~QFileBrowser();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
};

#endif
