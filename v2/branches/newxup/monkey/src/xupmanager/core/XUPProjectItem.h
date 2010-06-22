#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include <QObject>
#include <QFileInfo>

#include "MonkeyExport.h"

#include "XUPItem.h"
#include "XUPProjectItemInfos.h"
#include "pCommand.h"

class BuilderPlugin;
class DebuggerPlugin;
class InterpreterPlugin;
class XUPPlugin;

typedef QList<class XUPProjectItem*> XUPProjectItemList;

class Q_MONKEY_EXPORT XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
	
public:
	// project type id
	enum ProjectType { InvalidProject = -1, XUPProject = 0 };
	// target type
	enum TargetType { DefaultTarget = 0, DebugTarget, ReleaseTarget };
	
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
	virtual QStringList sourceFiles() const;
	// return the list of all source files for all projects from the root project
	QStringList topLevelProjectSourceFiles() const;
	/* Add files to the project. 
	 * Optional argument 'scope' allows to add files to the particular part of the project, 
	 * not to the project root.
	 * It allows, for example, to add files to the particular scope in the QMake projects (win32, !unix ...)
	 */
	virtual void addFiles( const QStringList& files, XUPItem* scope = NULL );
	// return the direct parent proejct if one, else return itself
	XUPProjectItem* parentProject() const;
	// return the most toplevel project ( ie: the model root project )
	XUPProjectItem* topLevelProject() const;
	// return the parent project for include project ( recursive parent project for include project, else return project itself )
	XUPProjectItem* rootIncludeProject() const;
	// return children project recursively according to bool
	XUPProjectItemList childrenProjects( bool recursive ) const;
	
	// return icon filename for item
	QString iconFileName( const XUPItem* item ) const;
	// return the project icons path
	QString iconsPath() const;
	
	// return the display text of a project variable name
	QString variableDisplayText( const QString& variableName ) const;
	
	// return the display text for the project item
	QString itemDisplayText( XUPItem* item ); /* TODO drop the method? replace with item->displayText() ? */
	// return the display icon for the project item
	QIcon itemDisplayIcon( XUPItem* item );/* TODO drop the method? replace with item->displayText() ? */
	
	// rebuild the project cache by clearing values and analyzing again the project
	void rebuildCache();
	
	// return the matching path ( from start ) between left and right string or null string if result isa drive on windows, or / on unix like
	QString matchingPath( const QString& left, const QString& right ) const;
	// return the compressed result list of paths list given in parameter
	QStringList compressedPaths( const QStringList& paths ) const;
	// return a list of QFileInfo having corresponding partial file path
	virtual QFileInfoList findFile( const QString& partialFilePath ) const;
	// return all variable items named variableName until caller is found ( if define ) or until the the complete tree is scanned
	// if recursive is true, then the scan recurse in each item, else not
	virtual XUPItemList getVariables( const XUPItem* root, const QString& variableName, bool recursive = true ) const;
	// return first found variable with name. NULL returned, if not found
	virtual XUPItem* getVariable( const XUPItem* root, const QString& variableName) const;
	// return the project datas as qstring
	virtual QString toString() const;
	
	// return the project settings scope, creating it if needed
	XUPItem* projectSettingsScope( bool create ) const;
	// return a project settings value as stringlist or string.
	virtual QStringList projectSettingsValues( const QString& variable, const QStringList& defaultValues = QStringList() ) const;
	virtual QString projectSettingsValue( const QString& variable, const QString& defaultValue = QString() ) const;
	// set a project setting value
	virtual void setProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void setProjectSettingsValue( const QString& variable, const QString& value );
	// add project setting value
	virtual void addProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void addProjectSettingsValue( const QString& variable, const QString& value );
	
	// return the project type id
	virtual int projectType() const;
	// return the textual representation key for target type
	virtual QString targetTypeString( XUPProjectItem::TargetType type ) const;
	// return the current project target type
	virtual XUPProjectItem::TargetType projectTargetType() const;
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
	// return the project target file, ie the binary / library file path, if allowToAskUser is set to true - user might be asked for it via doalog
	/* for PasNox: this method no longer contains targetPlatform parameter, because this parameter never needed.
	   We always use only current platform */
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
	QString targetFilePath( const pCommandTargetExecution& execution );
	
	// return plugin associated with the project
	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;

	// add a pCommand in menu
	virtual void addCommand( pCommand& cmd, const QString& mnu );
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();
	
	QString codec() const;
	
	/* TODO 1: make method =0,
	 */
	virtual XUPPlugin* editorPlugin() {return NULL;};
public slots:

protected:
	QDomDocument mDocument;
	pCommandMap mCommands;
	static XUPProjectItemInfos* mXUPProjectInfos;
	static bool mFoundCallerItem;
	
	QMap<QString, QString> mVariableCache;
	
	QString mCodec;
	QString mFileName;
	QString mLastError;
signals:
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );
};

#endif // XUPPROJECTITEM_H
