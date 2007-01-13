#ifndef TEXT_H
#define TEXT_H
//
#include "ChildPlugin.h"
#include "Workspace.h"
//
class Q_MONKEY_EXPORT Workspace;
//
class Text : public QObject, public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	//
public:
	virtual ~Text();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	virtual QStringList filters() const;
	virtual bool addNewDocument( const QString&, AbstractProjectProxy* = 0 );
	virtual bool openFile( const QString&, AbstractProjectProxy* = 0 );
	//
};
//
#endif // TEXT_H
