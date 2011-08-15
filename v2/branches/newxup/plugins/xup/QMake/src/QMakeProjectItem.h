#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"
#include "XUPProjectItemCache.h"

class CLIToolPlugin;

class QMakeProjectItemCacheBackend : public XUPProjectItemCacheBackend
{
public:
	QMakeProjectItemCacheBackend( XUPProjectItemCache* cache );
	
	virtual QString guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
	virtual QString guessedValue( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const;
	virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QString& op, const QString& value );
	virtual void recursiveScan( XUPProjectItem* project, XUPItem* root );
    virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );
};

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	friend class QMakeProjectItemCacheBackend;
	
public:
	Q_INVOKABLE QMakeProjectItem();
	virtual ~QMakeProjectItem();
    
	virtual QString toNativeString() const;
	virtual QString projectType() const;
	virtual bool open( const QString& fileName, const QString& codec );
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	virtual void installCommands();
	virtual XUPProjectItemCacheBackend* cacheBackend() const;
    virtual bool edit();
	virtual bool editProjectFiles();
	
protected:
	static QMakeProjectItemCacheBackend mCacheBackend;
	
	// handle the inclusion of include files
	bool handleIncludeFile( XUPItem* function );
	// handle inclusion of subdirs projects
	bool handleSubdirs( XUPItem* subdirs );
	
	CLIToolPlugin* builder() const;
};

#endif // QMAKEPROJECTITEM_H
