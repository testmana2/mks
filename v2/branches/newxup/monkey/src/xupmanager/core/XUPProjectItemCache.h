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
	XUPProjectItemCacheBackend( XUPProjectItemCache* cache = 0, const QString& separator = QChar( 11 ) ); // use unvisible char as separator
	
	QString multiLineSeparator() const;
	
	// return the content of a variable
	virtual QString guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
	// return the content where each variable is replaced by it's content
	virtual QString guessedValue( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const;
	// update variable in cache. the optional op variable can be used to define the update mode
	virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QString& value, const QString& op = QString::null );
	// recursively scan the project children (and include project) to build the cache
	virtual void recursiveScan( XUPProjectItem* project, XUPItem* root );
	// a hook of the cache system to inform an item has been cached, this can be usefull for load include/sub projects...
	// return true if project was changed, else false!!
    virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );

protected: // for performance raeson
	XUPProjectItemCache* mCache;
	QString mMultiLineSeparator;
};

class Q_MONKEY_EXPORT XUPProjectItemCache
{
public:
	typedef QPointer<XUPProjectItem> ProjectPointer; // tracked project
	typedef QHash<QString, QString> HashedVariables; // variable name, variable content
	typedef QHash<ProjectPointer, HashedVariables> ProjectCache; // project, variables content
	
	// return the hashed projects variable content cache
	XUPProjectItemCache::ProjectCache& cachedData() const;
	
	// clear and build/rebuild the cache for project
	void build( XUPProjectItem* project );
	// update the cache for project starting at root
	void update( XUPProjectItem* project, XUPItem* root = 0 );
	// clear the cache for project
	void clear( XUPProjectItem* project );
	
	// return the cached variable content, building cache if needing
	QString value( XUPProjectItem* project, const QString& variable ) const;
	// return the cached variable content as list, building the cache if needed
	QStringList values( XUPProjectItem* project, const QString& variable ) const;
	// return an evaluated content (ie: a content where each variable in the content are repalced with cached content)
	QString evaluatedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const;
#ifndef QT_NO_DEBUG
	// debug purpose
	void debug( bool full = true ) const;
#endif

protected:
	mutable XUPProjectItemCache::ProjectCache mCache;
	// return an indented string having same character repeated indent times
	QString indentString( int indent ) const;
};

// a way to hash a project
uint qHash( const XUPProjectItemCache::ProjectPointer& pointer );

#endif // XUPPROJECTITEMCACHE_H
