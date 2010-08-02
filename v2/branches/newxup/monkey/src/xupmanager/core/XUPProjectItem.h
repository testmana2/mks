#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include <QObject>
#include <QFileInfo>

#include "MonkeyExport.h"

#include "XUPItem.h"
#include "ProjectTypesIndex.h"
#include "pCommand.h"

class XUPPlugin;

typedef QList<class XUPProjectItem*> XUPProjectItemList;

class Q_MONKEY_EXPORT XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
	
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
	
	// return the project icons path
	virtual QString iconsPath() const;
	
	// return the display text of a project variable name
	virtual QString variableDisplayText( const QString& variableName ) const;
	// return the display icon name of a project variable name. Used by XUPItem::displayIcon()
	virtual QString variableDisplayIcon( const QString& variableName ) const;
	
	// rebuild the project cache by clearing values and analyzing again the project
	virtual void rebuildCache() {};
	
	// return the matching path ( from start ) between left and right string or null string if result isa drive on windows, or / on unix like
	QString matchingPath( const QString& left, const QString& right ) const;
	// return a list of QFileInfo having corresponding partial file path
	virtual QFileInfoList findFile( const QString& partialFilePath ) const;
	// return all variable items named variableName until caller is found ( if define ) or until the the complete tree is scanned
	// if recursive is true, then the scan recurse in each item, else not
	virtual XUPItemList getVariables( const XUPItem* root, const QString& variableName, bool recursive = true ) const;
	// return first found variable with name. NULL returned, if not found
	virtual XUPItem* getVariable( const XUPItem* root, const QString& variableName) const;
	// return the project datas as qstring
	virtual QString toString() const;
		
	// return the project type id
	virtual QString projectType() const;
	// return a new instance of this kind of projecttype
	// FIXME AK in future I think XUPProject will be abstract class
	inline virtual XUPProjectItem* newProject() const { return new XUPProjectItem(); }
	// open a project with codec
	virtual bool open( const QString& fileName, const QString& codec );
	// save the project
	virtual bool save();
	// return the project target file, ie the binary / library file path, if allowToAskUser is set to true - user might be asked for it via doalog
	/* for PasNox: this method no longer contains targetPlatform parameter, because this parameter never needed.
	   We always use only current platform */
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
	QString targetFilePath( const pCommandTargetExecution& execution );
	
	/* When project is activated (selected as current), some plugins can be also enabled.
	 * for example - PHP-Qt project will activate PHP interpreter plugin.
	 * When project deselected - plugin will be disabled
	 */
	virtual QStringList autoActivatePlugins() const;

	// add a pCommand in menu
	virtual void addCommand( pCommand& cmd, const QString& mnu );
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();
	
	QString codec() const;
	
	virtual bool edit() {return false;}; // TODO =0
	
	/* returns a filter of source file names, which can be added to the project.
	   Filter is suitable for QFileDialog
	 */
	QString sourceFileNameFilter() const;
	// return the filtered variable list for project type
	virtual QStringList filteredVariables() const;


protected:
	/* Source file name patterns is list of string pairs. ExamplesPath
		("Python file", "*.py") ("Forms file", "*.ui")
	   This info used for build file name filter for "Add files to the project" dialog (XUPProjectItem::sourceFileNameFilter())
	 */
	virtual StringStringListList sourceFileNamePatterns() const { return StringStringListList(); }; // FIXME = 0 
	
	QDomDocument mDocument;
	pCommandMap mCommands;
	static bool mFoundCallerItem;
	
	QString mCodec;
	QString mFileName;
	QString mLastError;
signals:
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );
};

#endif // XUPPROJECTITEM_H
