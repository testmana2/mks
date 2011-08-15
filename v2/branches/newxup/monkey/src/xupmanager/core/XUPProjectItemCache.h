#ifndef XUPPROJECTITEMCACHE_H
#define XUPPROJECTITEMCACHE_H

#include <MonkeyExport.h>

#include <QHash>
#include <QPointer>

class XUPProjectItem;
class XUPItem;
class XUPProjectItemCache;

class Q_MONKEY_EXPORT XUPProjectItemCacheBackend
{
public:
	XUPProjectItemCacheBackend( XUPProjectItemCache* cache = 0 );
	
	virtual QString guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
	virtual QString guessedValue( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const;
	virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QString& op, const QString& value );
	virtual void recursiveScan( XUPProjectItem* project, XUPItem* root );
	// a hook of the cache system to inform an item has been cached, this can be usefull for load include/sub projects...
    virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );

protected: // for performance raeson
	XUPProjectItemCache* mCache;
};

class Q_MONKEY_EXPORT XUPProjectItemCache
{
public:
	typedef QPointer<XUPProjectItem> ProjectPointer; // tracked project
	typedef QHash<QString, QString> HashedVariables; // variable name, variable content
	typedef QHash<ProjectPointer, HashedVariables> ProjectCache; // project, variables content
	
	XUPProjectItemCache();
	
	XUPProjectItemCache::ProjectCache& cachedData() const;
	
	void build( XUPProjectItem* project, XUPItem* root = 0 );
	void update( XUPProjectItem* project, XUPItem* root = 0 );
	void clear( XUPProjectItem* project );
	
	QString value( XUPProjectItem* project, const QString& variable ) const;
	QStringList values( XUPProjectItem* project, const QString& variable ) const;
	QString evaluatedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const;
	
	void debug( bool full = true ) const;

protected:
	mutable XUPProjectItemCache::ProjectCache mCache;
	
	QString indentString( int indent ) const;
};

uint qHash( const XUPProjectItemCache::ProjectPointer& pointer );

#endif // XUPPROJECTITEMCACHE_H
