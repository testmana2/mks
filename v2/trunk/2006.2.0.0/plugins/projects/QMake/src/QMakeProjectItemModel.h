#ifndef QMAKEPROJECTITEMMODEL_H
#define QMAKEPROJECTITEMMODEL_H
//
#include "AbstractProjectItemModel.h"
//
#include <QDir>
//
class QMakeProjectItem;
//
class QMakeProjectItemModel : public AbstractProjectItemModel
{
	Q_OBJECT
	friend class QMake;
	//
public:
	QMakeProjectItemModel( const QString&, AbstractProjectItemModel* = 0 );
	virtual ~QMakeProjectItemModel();
	// return the simple keyword to show
	static QStringList simpleModelVariables();
	// the root project item
	virtual QStandardItem* projectItem() const;
	// return child project
	virtual QStringList subProjects() const;
	// get first value by variable name
	virtual QString getValue( const QString& ) const;
	// get all values by variable name
	virtual QStringList getValuesList( const QString& ) const;
	// get all values as string by variable name
	virtual QString getValues( const QString& ) const;
	// return the items for variable name
	virtual QList<QStandardItem*> itemsByName( const QString& ) const;
	// return the first item for variable name
	virtual QStandardItem* itemByName( const QString& ) const;
	// open project
	virtual bool open( bool = true );
	//
protected:
	// prepare the completion list
	virtual QStringList prepareCompletionFilesList();
	// preapre completion
	virtual void prepareCompletion();
	// parse project
	virtual bool parse();
	//
	void appendRow( QStandardItem*, QStandardItem* = 0 );
	QStringList parseFunctionParameters( const QString& );
	void parseLine( const QString&, QMakeProjectItem* = 0 );
	void parseBlock( const QString&, QMakeProjectItem* = 0 );
	void setItemIcon( QMakeProjectItem* );
	QMakeProjectItem* getFolder( const QString&, QMakeProjectItem* = 0 );
	//
	QMakeProjectItem* iProject;
	//
public slots:
	virtual void close();
	virtual void save();
	//
};
//
#endif // QMAKEPROJECTITEMMODEL_H
