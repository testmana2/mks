#ifndef CPP_H
#define CPP_H
//
#include "ChildPlugin.h"
#include "Workspace.h"
//
class Q_MONKEY_EXPORT Workspace;
//
class Cpp : public QObject, public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	//
public:
	virtual ~Cpp();
	virtual void initialize( Workspace* );
	virtual QString name() const;
	virtual QString description() const;
	virtual bool install();
	virtual bool uninstall();
	virtual QStringList filters() const;
	virtual bool addNewDocument( const QString&, AbstractProject* = 0 );
	virtual bool openFile( const QString&, AbstractProject* = 0 );
	//
};
//
#endif // CPP_H
