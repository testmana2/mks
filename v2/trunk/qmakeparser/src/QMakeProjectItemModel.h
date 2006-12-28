#ifndef QMAKEPROJECTITEMMODEL_H
#define QMAKEPROJECTITEMMODEL_H
//
#include "AbstractProjectItemModel.h"
//
class QMakeProjectItem;
//
class QMakeProjectItemModel : public AbstractProjectItemModel
{
	Q_OBJECT
	//
public:
	QMakeProjectItemModel( const QString&, QObject* = 0 );
	~QMakeProjectItemModel();
	// return the simple keyword to show
	static QStringList simpleModelVariables();
	// the root project item
	virtual QStandardItem* projectItem() const;
	// return child project
	virtual QStringList subProjects() const;
	// get first value by path
	virtual QString getValue( const QString& ) const;
	// get all values by path
	virtual QStringList getValuesList( const QString& ) const;
	// get all values as string by path
	virtual QString getValues( const QString& ) const;
	// open project
	virtual bool open( bool = true );
	//
protected:
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
