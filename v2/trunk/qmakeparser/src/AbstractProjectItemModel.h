#ifndef ABSTRACTPROJECTITEMMODEL_H
#define ABSTRACTPROJECTITEMMODEL_H
//
#include <QStandardItemModel>
//
class AbstractProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	//
public:
	AbstractProjectItemModel( const QString&, QObject* = 0 );
	//
	virtual bool isOpen() const;
	virtual QString name() const;
	virtual QString path() const;
	virtual QString filePath() const;
	virtual QString filePath( const QString& );
	virtual bool openProject( bool = true ) = 0;
	//
protected:
	virtual bool parseProject() = 0;
	//
	QString mFilePath;
	bool mIsOpen;
	//
public slots:
	virtual void setFilePath( const QString& );
	virtual void closeProject() = 0;
	//
signals:
	void projectOpened( bool );
	void filePathChanged( const QString& );
	//
};
//
#endif // ABSTRACTPROJECTITEMMODEL_H
