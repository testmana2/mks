/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : ProjectItem
** FileName  : ProjectItem.h
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "MonkeyExport.h"
#include "ProjectsModel.h"

#include <QObject>
#include <QStandardItem>

class QTextCodec;
class ProjectPlugin;

class ProjectItem;
class QMenu;
class QAction;

typedef QList<ProjectItem*> ProjectItemList;

/*
For compile project I was need to implement all  =0 functions with some body.
I marked this functions with    //FIXME
Possible it's error, need to think, how to fix
hlamer
*/
class Q_MONKEY_EXPORT ProjectItem : public QObject, public QStandardItem
{
	Q_OBJECT
	friend class UIProjectsManager;
	
public:
enum BuildTargetType
{
    aBuild = 0,
    aReBuild,
    aClean,
    aDistClean,
    aExecute,
    aBuildExecute,
    aLast
};

	ProjectItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 );
	
	virtual ~ProjectItem () {};
	// item type
	virtual int type() const;
// 	// plugin name
// 	virtual QString pluginName() const = 0;   //use parentPlugin()->infos.Name;
	// set data
	virtual void setData( const QVariant&, int = Qt::UserRole +1 );
	// type of the item
	virtual void setType( ProjectsModel::NodeType );
	virtual ProjectsModel::NodeType getType();
	// item operator, only variable
	virtual void setOperator( const QString& );
	virtual QString getOperator() const;
	// item value
	virtual void setValue( const QString& );
	virtual QString getValue() const;
	// item is multiline, only variable
	virtual void setMultiLine( bool );
	virtual bool getMultiLine() const;
	// item content
	virtual void setComment( const QString& );
	virtual QString getComment() const;
	// item filepath
	virtual void setFilePath( const QString& );
	virtual QString getFilePath() const;
	// filtered mode sort
	virtual void setFilteredView( int );
	virtual int getFilteredView() const;
	// original mode sort
	virtual void setOriginalView( int );
	virtual int getOriginalView() const;
	// item modified flag
	virtual void setModified( bool );
	virtual bool getModified() const;
	// item read only, only project
	virtual void setReadOnly( bool );
	virtual bool getReadOnly() const;
	// item indent
	virtual QString getIndent() const 
		{return QString::null;};
	// item eol
	virtual QString getEol() const
		{return QString::null;};
	// tell if item is the first child of its parent
	virtual bool isFirst() const
		{return true;};  // FIXME
	// tell if item is the last child of its parent
	virtual bool isLast() const
		{return true;};  // FIXME
	// scope of item
	virtual QString scope() const 
		{return QString::null;};  // FIXME
	// check scope
	virtual QString checkScope( const QString& ) const
		{return QString::null;};  // FIXME
	// check equals scope
	virtual bool isEqualScope( const QString& s1, const QString& s2 ) const 
		{return (s1 == s2);};  // FIXME
	// item model
	virtual ProjectsModel* model() const;
	// item parent
	virtual ProjectItem* parent() const;
	// parents number
	virtual int parentCount() const;
	// child item
	virtual ProjectItem* child( int, int = 0 ) const;
	// get children, recursively according to bool and same project according to bool
	virtual ProjectItemList children( bool = false, bool = false ) const;
	// append item
	virtual void appendRow( ProjectItem* )
		{;};  // FIXME
	// insert item
	virtual void insertRow( int, ProjectItem* )
		{;};  // FIXME
	// moving item
	virtual bool swapRow( int, int )
		{return false;};  // FIXME	
	virtual bool moveRowUp( int ) 
		{return false;};  // FIXME	
	virtual bool moveRowDown( int )
		{return false;};  // FIXME	
	virtual bool moveUp()
		{return false;};  // FIXME	
	virtual bool moveDown()
		{return false;};  // FIXME	
	// remove itself
	virtual void remove()
		{;};  // FIXME	
	// redo internal layout, to filter / sort items
	virtual void refresh();
	
	// the project item
	virtual ProjectItem* project() const;
	// parent project in case it s a subproject
	virtual ProjectItem* parentProject() const;
	// get direct child project, or all according to bool
	virtual ProjectItemList childrenProjects( bool = true ) const;
	// get all project items
	virtual ProjectItemList projectItems() const;
	// get all scope from project
	virtual ProjectItemList projectScopes() const;
	// get all unique scope list from project
	virtual QStringList projectScopesList() const;
	// item direct scopes, or recursively according to bool
	virtual ProjectItemList childrenScopes( bool = false) const;
	// canonical project filepath
	virtual QString canonicalFilePath() const;
	// canonical file path according to project path
	virtual QString canonicalFilePath( const QString& ) const;
	// canonical project path
	virtual QString canonicalPath() const;
	// relative path according to project path
	virtual QString canonicalPath( const QString& ) const;
	// relative file path according to project path
	virtual QString relativeFilePath( const QString& ) const;
	// filename of filepath
	virtual QString fileName( const QString& );
	// complete basename of filename
	virtual QString completeBaseName( const QString& );
	// name of project
	virtual QString name() const;
	// close the project
	virtual void close() {}; // FIXME
	// save project, asking user according to bool
	virtual void save( bool = true ) {}; //FIXME
	// save project including all children projects
	virtual void saveAll( bool = true ) {}; //FIXME
	// show the content of items 
	virtual void debug() {}; //FIXME
	
	// the compiler this project use
	virtual QString compiler() const
	{ return "GNUMake"; }
	// the debugger this project use
	virtual QString debugger() const
	{ return "GNUDebugger"; }
	// the interpreter this project use
	virtual QString interpreter()
	{ return QString::null; }
	
	// get index list
	virtual ProjectItemList match( int, const QVariant& ) const;
	// get all items matching
	virtual ProjectItemList getItemList( ProjectsModel::NodeType, const QString&, const QString&, const QString& ) const;
	// get item scope, creating it if needed
	virtual ProjectItem* getItemScope( const QString&, bool ) const
			{return NULL;};  // FIXME	
	// get all variable content as modelindex list for project index
	virtual ProjectItemList getItemListValues( const QString&, const QString&, const QString& ) const;
	// get a variable index
	virtual ProjectItem* getItemVariable( const QString&, const QString&, const QString& ) const;
	
	// get variable content as stringlist for project index
	virtual QStringList getListValues( const QString&, const QString& = "=", const QString& = QString::null ) const 
		{return QStringList();};  // FIXME
	// get variable content as string for project index
	virtual QString getStringValues( const QString&, const QString& = "=", const QString& = QString::null ) const
			{return QString::null;};  // FIXME	
	// set variable content as stringlist for project index
	virtual void setListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null )
			{;};  // FIXME	
	// get variable content as string for project index
	virtual void setStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null )
			{return ;};  // FIXME	
	// add variable content as stringlist for project index
	virtual void addListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null )
			{;};  // FIXME	
	// add variable content as string for project index
	virtual void addStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null )
			{;};  // FIXME	
	//returns true, if plugin of this project type is enabled
	virtual bool isEnabled();
	//returns pointer to the plugin, that are managing this project type
	virtual ProjectPlugin* getParentPlugin ()
			{return NULL;};  // FIXME	
	
	virtual bool openProject( const QString&) {return false;}; //FIXME

    virtual void removeSelfFromMenu (QMenu* menu){};
    virtual void addSelfToMenu (QMenu* menu){};

public slots:
	virtual void editSettings()
			{;};  // FIXME	
	virtual void buildMenuTriggered () {};
	
protected:
	QString mBuffer;
	virtual void redoLayout( ProjectItem* = 0 ) {}; //FIXME
	virtual void writeProject() {}; //FIXME
	virtual void writeItem( ProjectItem* ) {}; //FIXME

};

#endif // PROJECTITEM_H
