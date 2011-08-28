#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"
#include "XUPProjectItemCache.h"

class CLIToolPlugin;

class QMakeProjectItemCacheBackend : public XUPProjectItemCacheBackend
{
public:
	QMakeProjectItemCacheBackend( XUPProjectItemCache* cache );
	
	virtual QStringList guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
	virtual QStringList guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const;
	virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op = QString::null );
	virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );
};

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	friend class QMakeProjectItemCacheBackend;
	
public:
	Q_INVOKABLE QMakeProjectItem();
	virtual ~QMakeProjectItem();
    
	virtual void removeValue( XUPItem* item, bool deleteFiles );
	virtual QString quoteString() const;
	virtual QString defaultOperator() const;
	virtual QString toNativeString() const;
	virtual QString projectType() const;
	virtual bool open( const QString& fileName, const QString& codec );
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
	virtual void installCommands();
	virtual XUPProjectItemCacheBackend* cacheBackend() const;
	
protected:
	static QMakeProjectItemCacheBackend mCacheBackend;
	
	virtual UIXUPEditor* newEditDialog() const;
	
	// guess subdirs project fileChanged
	QString guessSubProjectFilePath( const QString& subdirsValue ) const;
	// handle the inclusion of include files
	bool handleIncludeFile( XUPItem* function );
	// handle inclusion of subdirs projects
	bool handleSubdirs( XUPItem* subdirs );
	
	CLIToolPlugin* builder() const;
};

#endif // QMAKEPROJECTITEM_H
