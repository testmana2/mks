#ifndef CPP_H
#define CPP_H
//
#include "ChildPlugin.h"
#include "Workspace.h"
//
class Cpp : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	//
public:
	virtual ~Cpp();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	virtual QStringList filters() const;
	virtual bool addNewDocument( const QString&, AbstractProjectProxy* = 0 );
	virtual bool open( const QString&, AbstractProjectProxy* = 0 );
	virtual bool openFile( const QString&, AbstractProjectProxy* = 0 );
    virtual void saveCodeCoverage(const QString &name);
	//
};
//
#endif // CPP_H
