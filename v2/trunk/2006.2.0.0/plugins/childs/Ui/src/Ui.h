#ifndef UI_H
#define UI_H
//
#include "ChildPlugin.h"
#include "Workspace.h"
//
class Ui : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	//
public:
    Ui();
	virtual ~Ui();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	virtual QStringList filters() const;
	virtual bool addNewDocument( const QString&, AbstractProjectProxy* = 0 );
	virtual bool openFile( const QString&, AbstractProjectProxy* = 0 );
	//
};
//
#endif // UI_H
