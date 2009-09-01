#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include <QObject>
#include <QFileInfo>

#include "MonkeyExport.h"
#include "XUPItem.h"
#include "XUPProjectItemInfos.h"
#include "pCommand.h"

class BuilderPlugin;
class CompilerPlugin;
class DebuggerPlugin;
class InterpreterPlugin;

typedef QList<class XUPProjectItem*> XUPProjectItemList;

class Q_MONKEY_EXPORT XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
	
public:
	// project type id
	enum ProjectType { InvalidProject = -1, XUPProject = 0 };
	
	// ctor
	XUPProjectItem();
	// dtor
	virtual ~XUPProjectItem();
	
	// return the global static proejcts types informations
	static XUPProjectItemInfos* projectInfos();
	
	// the variable cache
	QMap<QString, QString>& variableCache();
	
	// set last encounter error
	void setLastError( const QString& error );
	// return the last encounter error
	QString lastError() const;
	
	// return the project absolute filename
	QString fileName() const;
	// return the project absolute path
	QString path() const;
	// return an absolute file name according to project path
	QString filePath( const QString& fileName ) const;
	// return a filepath relative to project path
	QString relativeFilePath( const QString& fileName ) const;
	// return the list of all source files for this project
	QStringList sourceFiles() const;
	// return the list of all source files for all projects from the root project
	QStringList topLevelProjectSourceFiles() const;
	
	// return the direct parent proejct if one, else return itself
	XUPProjectItem* parentProject() const;
	// return the most toplevel project ( ie: the model root project )
	XUPProjectItem* topLevelProject() const;
	// return the parent project for include project ( recursive parent project for include project, else return project itself )
	XUPProjectItem* rootIncludeProject() const;
	// return children project recursively according to bool
	XUPProjectItemList childrenProjects( bool recursive ) const;
	
	// return icon filename for item
	QString iconFileName( XUPItem* item ) const;
	// return the project icons path
	QString iconsPath() const;
	
	// return the display text of a project variable name
	QString variableDisplayText( const QString& variableName ) const;
	
	// return the display text for the project item
	QString itemDisplayText( XUPItem* item );
	// return the display icon for the project item
	QIcon itemDisplayIcon( XUPItem* item );
	
	// split a multi line value into QStringList
	QStringList splitMultiLineValue( const QString& value ) const;
	// return the matching path ( from start ) between left and right string or null string if result isa drive on windows, or / on unix like
	QString matchingPath( const QString& left, const QString& right ) const;
	// return the compressed result list of paths list given in parameter
	QStringList compressedPaths( const QStringList& paths ) const;
	// return a list of QFileInfo having corresponding partial file path
	virtual QFileInfoList findFile( const QString& partialFilePath ) const;
	// return all variable items named variableName until caller is found ( if define ) or until the the complete tree is scanned
	// if recursive is true, then the scan recurse in each item, else not
	virtual XUPItemList getVariables( const XUPItem* root, const QString& variableName, const XUPItem* callerItem = 0, bool recursive = true ) const;
	// return the project datas as qstring
	virtual QString toString() const;
	
	// return the project settings scope, creating it if needed
	XUPItem* projectSettingsScope( bool create ) const;
	// return a project settings value
	virtual QStringList projectSettingsValues( const QString& variable, const QStringList& defaultValues = QStringList() ) const;
	virtual QString projectSettingsValue( const QString& variable, const QString& defaultValue = QString() ) const 
	{ return projectSettingsValues( variable, defaultValue.isEmpty() ? QStringList() : QStringList( defaultValue ) ).join( " " ); }
	// set a project setting value
	virtual void setProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void setProjectSettingsValue( const QString& variable, const QString& value ) 
	{ setProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) ); }
	// add project setting value
	virtual void addProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void addProjectSettingsValue( const QString& variable, const QString& value ) 
	{ addProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) ); }
	
	// return the project type id
	inline virtual int projectType() const { return XUPProjectItem::XUPProject; }
	// register the project type
	virtual void registerProjectType() const;
	// unregister the project type
	virtual void unRegisterProjectType() const;
	// return a new instance of this kind of projecttype
	// FIXME AK in future I think XUPProject will be abstract class
	inline virtual XUPProjectItem* newProject() const { return new XUPProjectItem(); }
	// get a variable content in the project at the call instant
	virtual QString getVariableContent( const QString& variableName );
	// interpret the content, ie, replace variables by their content
	virtual QString interpretContent( const QString& content );
	// handle the inclusion of include files
	virtual bool handleIncludeFile( XUPItem* function );
	// analyze a project for caching the variables keys
	virtual bool analyze( XUPItem* item );
	// open a project with codec
	virtual bool open( const QString& fileName, const QString& codec );
	// save the project
	virtual bool save();
	
	// return plugin associated with the project
	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual CompilerPlugin* compiler( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;

	// add a pCommand in menu
	virtual void addCommand( pCommand& cmd, const QString& mnu );
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();

public slots:
	// called when a watched path of a DynamicFolder have changed
	void directoryChanged( const QString& path );

protected:
	QDomDocument mDocument;
	pCommandMap mCommands;
	static XUPProjectItemInfos* mXUPProjectInfos;
	static bool mFoundCallerItem;
	
	QMap<QString, QString> mVariableCache;

signals:
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );
};

#endif // XUPPROJECTITEM_H
