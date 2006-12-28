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
	//
	static QStringList simpleModelVariables();
	QStandardItem* projectItem() const;
	//
	virtual bool open( bool = true );
	//
protected:
	bool parse();
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
