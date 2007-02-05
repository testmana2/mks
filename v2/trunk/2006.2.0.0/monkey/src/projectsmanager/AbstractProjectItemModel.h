#ifndef ABSTRACTPROJECTITEMMODEL_H
#define ABSTRACTPROJECTITEMMODEL_H
//
#include <QStandardItemModel>
#include <QHash>
#include <QDir>
//
#include "MonkeyExport.h"
//
class QsciLexer;
class QsciAPIs;
class AbstractProjectItemModel;
typedef QHash<int, AbstractProjectItemModel*> QHashProjects; // id, project
//
class Q_MONKEY_EXPORT AbstractProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	//
public:
	AbstractProjectItemModel( const QString&, QObject* = 0 );
	virtual ~AbstractProjectItemModel();
	// project id
	int id() const;
	// get project by id
	static AbstractProjectItemModel* byId( int );
	// return a list of all projects
	static QHashProjects all();
	// project open flag
	virtual bool isOpen() const;
	// project modified flag
	virtual bool isModified() const;
	// project lexer
	virtual QsciLexer* lexer() const;
	// project apis
	virtual QsciAPIs* apis() const;
	// the project name
	virtual QString name() const;
	// the project path
	virtual QString path() const;
	// the project file path
	virtual QString filePath() const;
	// a file path according to project file path
	virtual QString filePath( const QString& );
	// open project
	virtual bool open( bool = true ) = 0;
	// the root project item
	virtual QStandardItem* projectItem() const = 0;
	// get all subprojects u must send full file path
	virtual QStringList subProjects() const = 0;
	// get first value by variable name
	virtual QString getValue( const QString& ) const = 0;
	// get all values by variable name
	virtual QStringList getValuesList( const QString& ) const = 0;
	// get all values as string by variable name
	virtual QString getValues( const QString& ) const = 0;
	// return the items for variable name
	virtual QList<QStandardItem*> itemsByName( const QString& ) const = 0;
	// return the first item for variable name
	virtual QStandardItem* itemByName( const QString& ) const = 0;
	//
protected:
	// preapre completion
	virtual void prepareCompletion() = 0;
	// get recursive files
	virtual QStringList recursiveFiles( QDir );
	// parse the project
	virtual bool parse() = 0;
	//
	static int mUniqueId;
	int mId;
	static QHashProjects mProjectsList;
	QString mFilePath;
	bool mIsOpen;
	bool mIsModified;
	QsciLexer* mLexer;
	QsciAPIs* mAPIs;
	//
public slots:
	// set project file path
	virtual void setFilePath( const QString& );
	// set project modified flag
	virtual void setModified( bool );
	// set project lexer
	virtual void setLexer( QsciLexer* );
	// set project lexer
	virtual void setAPIs( QsciAPIs* );
	// close project
	virtual void close() = 0;
	// save project
	virtual void save() = 0;
	//
signals:
	// emit when open state has changed
	void isOpenChanged( bool );
	// emit when modified state has changed
	void isModifiedChanged( bool );
	// emit when project file path has changed
	void filePathChanged( const QString& );
	//
};
//
#endif // ABSTRACTPROJECTITEMMODEL_H
