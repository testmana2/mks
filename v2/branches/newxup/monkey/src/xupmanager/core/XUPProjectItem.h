#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include <QObject>
#include <QFileInfo>

#include "MonkeyExport.h"

#include "XUPItem.h"
#include "ProjectTypesIndex.h"
#include "pCommand.h"

class QAction;

class XUPPlugin;

typedef QList<class XUPProjectItem*> XUPProjectItemList;

#define XUP_VERSION "1.1.0"

class Q_MONKEY_EXPORT XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
	friend class DebugDockWidget;
	
public:
	// target type
	enum TargetType { DefaultTarget = 0, DebugTarget, ReleaseTarget };
	
	// ctor
	XUPProjectItem();
	// dtor
	virtual ~XUPProjectItem();
	
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
	
	/* return the list of all source files for this project
	 * Defautl implementation returns empty list
	 */
	virtual QStringList sourceFiles() const;
	// return the list of all source files for all projects from the root project
	QStringList topLevelProjectSourceFiles() const;
	/* Add files to the project. 
	 * Optional argument 'scope' allows to add files to the particular part of the project, 
	 * not to the project root.
	 * It allows, for example, to add files to the particular scope in the QMake projects (win32, !unix ...)
	 */
	virtual void addFiles( const QStringList& files, XUPItem* scope = NULL ) = 0;
	// Remove file, subproject, or other item
	virtual void removeItem( XUPItem* item ) = 0;
	// return the direct parent proejct if one, else return itself
	XUPProjectItem* parentProject() const;
	// return the most toplevel project ( ie: the model root project )
	XUPProjectItem* topLevelProject() const;
	// return the parent project for include project ( recursive parent project for include project, else return project itself )
	/* FIXME hlamer: this method must be killed from XUP, it is QMake specific. But, I don't know, how to remove */
	XUPProjectItem* rootIncludeProject() const;
	// return children project recursively according to bool
	XUPProjectItemList childrenProjects( bool recursive ) const;
	
	// return the project icons path
	virtual QString iconsPath() const;
	
	// return the display text of a project variable name
	virtual QString variableDisplayText( const QString& variableName ) const;
	// return the display icon name of a project variable name. Used by XUPItem::displayIcon()
	virtual QString variableDisplayIcon( const QString& variableName ) const;
	
	// return a list of QFileInfo having corresponding partial file path
	virtual QFileInfoList findFile( const QString& partialFilePath ) const;
	// return all variable items named variableName until caller is found ( if define ) or until the the complete tree is scanned
	// if recursive is true, then the scan recurse in each item, else not
	virtual XUPItemList getVariables( const XUPItem* root, const QString& variableName, bool recursive = true ) const;
	// return first found variable with name. NULL returned, if not found
	virtual XUPItem* getVariable( const XUPItem* root, const QString& variableName) const;
	// return the project datas as qstring
	virtual QString toXml() const;
	virtual QString toNativeString() const;
		
	// return the project type id
	virtual QString projectType() const = 0;
	// open a project with codec
	virtual bool open( const QString& fileName, const QString& codec );
	// save the project
	virtual bool save();
	// return the project target file, ie the binary / library file path, if allowToAskUser is set to true - user might be asked for it via doalog
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
	
	/* When project is activated (selected as current), some plugins can be also enabled.
	 * for example - PHP-Qt project will activate PHP interpreter plugin.
	 * When project deselected - plugin will be disabled
	 */
	virtual QStringList autoActivatePlugins() const;
	
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();
	
	QString codec() const;
	
	virtual bool edit() = 0;
	
	/* returns a filter of source file names, which can be added to the project.
	   Filter is suitable for QFileDialog
	 */
	QString sourceFileNameFilter() const;
	/** Return the filtered variable list for project type.
	 *  Filtered variables not dispayed in the Project Manager GUI
	 */
	virtual QStringList filteredVariables() const;


protected:
	/** Add pCommand console commands to the main menu. When menu item triggered - console command executed.
	 * If more than one command set for the menu - commands will be executed one by one
	 * XUPProjectItem remembers create QAction's and deletes it by uninstallCommands()
	 */
	virtual void addCommand( pCommand& cmd, const QString& mnu );
	void addCommands( const QString& mnu, const QString& text, pCommandList& cmds );
	
	/* Source file name patterns is list of string pairs. ExamplesPath
		("Python file", "*.py") ("Forms file", "*.ui")
	   This info used for build file name filter for "Add files to the project" dialog (XUPProjectItem::sourceFileNameFilter())
	 */
	virtual Pair_String_StringList_List sourceFileNamePatterns() const = 0;
	
	QDomDocument mDocument;
	/* Action pointers stored here for delete it, when current project changed */
	QList<QAction*> mInstalledActions;
	
	QString mCodec;
	QString mFileName;
	QString mLastError;
protected slots:
	/* Common handler for actions, which execute pCommand.
	   Does few checks, then executes pCommand
	 */
	void internal_projectCustomActionTriggered();
};

#endif // XUPPROJECTITEM_H
