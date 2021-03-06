/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
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
    virtual ~XUPProjectItemCacheBackend();
    
    // return the content of a variable
    virtual QStringList guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
    // return the content where each variable is replaced by it's content
    virtual QStringList guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const;
    // update variable in cache. the optional op variable can be used to define the update mode
    virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op = QString::null );
    // recursively scan the project children (and include project) to build the cache
    virtual void recursiveScan( XUPProjectItem* project, XUPItem* root );
    // a hook of the cache system to inform an item has been cached, this can be usefull for load include/sub projects...
    // return true if project was changed, else false!!
    virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );

protected:
    XUPProjectItemCache* mCache;
};

class Q_MONKEY_EXPORT XUPProjectItemCache
{
    friend class XUPProjectItem;
    
public:
    typedef QPointer<XUPProjectItem> ProjectPointer; // tracked project
    typedef QHash<QString, QStringList> HashedVariables; // variable name, variable content
    typedef QHash<ProjectPointer, HashedVariables> ProjectCache; // project, variables content
    
    // return the hashed projects variable content cache
    XUPProjectItemCache::ProjectCache& cachedData() const;
    
    // clear and build/rebuild the cache for project
    void build( XUPProjectItem* project );
    // update the cache for project starting at root
    void update( XUPProjectItem* project, XUPItem* root = 0 );
    // clear the cache for project
    void clear( XUPProjectItem* project );
    
    // return the cached variable content as list, building the cache if needed
    QStringList values( XUPProjectItem* project, const QString& variable ) const;
#ifndef QT_NO_DEBUG
    // debug purpose
    void debug( bool full = true ) const;
#endif

protected:
    mutable XUPProjectItemCache::ProjectCache mCache;
};

// a way to hash a project
Q_MONKEY_EXPORT uint qHash( const XUPProjectItemCache::ProjectPointer& pointer );

#endif // XUPPROJECTITEMCACHE_H
