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
	//
	virtual bool openProject( bool = true );
	//
protected:
	bool parseProject();
	//
	QStringList parseFunctionParameters( const QString& );
	void parseLine( const QString&, QMakeProjectItem* );
	void parseBlock( const QString&, QMakeProjectItem* );
	void setItemIcon( QMakeProjectItem* );
	//
public slots:
	virtual void closeProject();
	//
};
//
#endif // QMAKEPROJECTITEMMODEL_H
